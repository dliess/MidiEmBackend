#include "Base.h"

#include <sys/timerfd.h>

#include <cassert>
#include <exception>
#include <loguru.hpp>

#include "BeatTick.h"
#include "FdSet.h"
#include "RtServer.h"
#include "LoaderServer.h"
#include "ThreadHelpers.h"
#include "UsbMidiPortNotifier.h"

// ----- Time measuring -----
#include "CyclicDataOutputterThread.h"
#include "Histogram.h"
#include "Measurer.h"
#include "OutputterDestinationsZmq.h"

using TenthMs = std::chrono::duration<int, std::ratio<1, 10000>>;
using DataHolderTenthMs = TimeMeasure::Histogram<TenthMs>;

template <unsigned int Id>
using MeasurerTenthMs = TimeMeasure::Measurer<Id, DataHolderTenthMs>;

TimeMeasure::CyclicDataOutputterThread<DataHolderTenthMs,
                                       TimeMeasure::Destination::Zmq>
    outThreadZmq({
        &MeasurerTenthMs<0>::instance().dataHolder(),
        &MeasurerTenthMs<1>::instance().dataHolder(),
    });
// --------------------------


base::Base::Base(const std::string &configDir) :
    musicDeviceHolder(),
    musicDeviceFactory(musicDeviceHolder, configDir),
    transportControl(musicDeviceHolder.musicDevices),
    instruments(musicDeviceHolder.musicDevices),
    instrumentsFactory(instruments, musicDeviceHolder),
    midiRouter(musicDeviceHolder.midiHolder)
{
   // TODO: Remove Dummy
   instruments.load("relDir", "filename", "section");
}

void base::Base::start()
{
   MeasurerTenthMs<0>::instance().dataHolder().setHistogramRange(1000);
   MeasurerTenthMs<1>::instance().dataHolder().setHistogramRange(1000);

   outThreadZmq.destination().bind("tcp://*:12341");
   outThreadZmq.startThread(500);
   if (!midi::PortNotifiers::instance().init())
   {
      // TODO: put this code to Midi lib
      throw std::runtime_error("midi::PortNotifiers::instance().init() failed");
   }

   m_mainRtThread = std::make_unique<util::Thread>(
       [this](const std::atomic<bool> &terminateRequest) {
          mainRtThreadFunction(terminateRequest);
       });

   m_portNotifierThread = std::make_unique<util::Thread>(
       [this](const std::atomic<bool> &terminateRequest) {
          loaderThreadFunction(terminateRequest);
       });
}

void base::Base::waitForEnd()
{
   assert(m_mainRtThread);
   m_mainRtThread->join();
}

void base::Base::mainRtThreadFunction(const std::atomic<bool> &terminateRequest)
{
   uiadapter::capnzero::RtServer rtServer(m_zmqContext, instruments,
                                          musicDeviceHolder.musicDevices,
                                          transportControl, midiRouter);

   int timerFd           = timerfd_create(CLOCK_MONOTONIC, 0);
   constexpr auto Period = std::chrono::milliseconds(1);
   constexpr auto PeriodNs =
       std::chrono::duration_cast<std::chrono::nanoseconds>(Period);
   itimerspec t(
       {.it_interval = {0, PeriodNs.count()}, .it_value = {0, 1000000}});
   timerfd_settime(timerFd, 0, &t, NULL);

   utils::FdSet fdSet;
   fdSet.AddFd(timerFd, [this](int fd) {
      std::array<uint8_t, 8> buf;
      read(fd, buf.data(), buf.size());
      loopFn(); 
   });
   fdSet.AddFd(rtServer.getFd(), [&rtServer](int fd) {
      rtServer.processNextRequestAllNonBlock();
   });
   fdSet.AddFd(rtServer.signals().getFd(), [&rtServer](int fd) {
      rtServer.signals().handleAllSubscriptions();
   });

   tempo::BeatTick::instance().start();
   while (!terminateRequest) { fdSet.Select(); }
}

void base::Base::loaderThreadFunction(const std::atomic<bool> &terminateRequest)
{
   uiadapter::capnzero::LoaderServer loaderServer(m_zmqContext, musicDeviceFactory);
   int timerFd           = timerfd_create(CLOCK_MONOTONIC, 0);
   constexpr auto Period = std::chrono::seconds(1);
   itimerspec t(
       {.it_interval = {Period.count(), 0}, .it_value = {1, 0}});
   timerfd_settime(timerFd, 0, &t, NULL);

   utils::FdSet fdSet;
   fdSet.AddFd(timerFd,
               [this](int fd) {
      std::array<uint8_t, 8> buf;
      read(fd, buf.data(), buf.size());
      midi::PortNotifiers::instance().update();
   });
   fdSet.AddFd(loaderServer.getFd(), [&loaderServer](int fd) {
      loaderServer.processNextRequestAllNonBlock();
   });
   fdSet.AddFd(loaderServer.signals().getFd(), [&loaderServer](int fd) {
      loaderServer.signals().handleAllSubscriptions();
   });
   while (!terminateRequest) { fdSet.Select(); }
}

void base::Base::loopFn()
{
   // LOG_SCOPE_FUNCTION(INFO);

   // VLOG_SCOPE_F(0, "Base::mainRtThreadFunction()");
   //{
   // VLOG_SCOPE_F(1, "PortNotifier update");
   // ... some code to measure ...
   //}

   tempo::BeatTick::instance().nextTimeSlot();
   {
      MeasurerTenthMs<0>::Guard guard;
      musicDeviceHolder.midiHolder.midiClock();
      musicDeviceHolder.midiHolder.processMidiInBuffers();
      musicDeviceHolder.musicDevices.updateSoundParameterActualValues();
   }

   musicDeviceHolder.musicDevices.updateSoundParameterUI();
   musicDeviceFactory.invokeInserterQueueActions();
   MeasurerTenthMs<1>::instance().sample();
}
