#include "Base.h"

#include <sys/timerfd.h>

#include <cassert>
#include <exception>
#include <loguru.hpp>

#include "BeatTick.h"
#include "FdSet.h"
#include "LoaderServer.h"
#include "RtClient.h"
#include "RtServer.h"
#include "ThreadHelpers.h"
#include "UsbMidiPortNotifier.h"

// ----- Time measuring -----
#include "CyclicDataOutputterThread.h"
#include "Histogram.h"
#include "Measurer.h"
#include "OutputterDestinationsZmq.h"

using TenthMs           = std::chrono::duration<int, std::ratio<1, 10000>>;
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
    transportControl(musicDeviceHolder),
    instruments(musicDeviceHolder.musicDevices),
    instrumentsFactory(instruments, musicDeviceHolder),
    midiRouter(musicDeviceHolder.midiHolder)
{
   // TODO: Remove Dummy
   instruments.load("relDir", "filename", "section");
}

base::Base::~Base() noexcept = default;

void base::Base::start()
{
   MeasurerTenthMs<0>::instance().dataHolder().setHistogramRange(1000);
   MeasurerTenthMs<1>::instance().dataHolder().setHistogramRange(1000);

   outThreadZmq.destination().bind("tcp://*:55570");
   outThreadZmq.startThread(500);
   if (!midi::PortNotifiers::instance().init())
   {
      // TODO: put this code to Midi lib
      throw std::runtime_error("midi::PortNotifiers::instance().init() failed");
   }

   tempo::BeatTick::instance().abletonLink().enable(true);
   tempo::BeatTick::instance().abletonLink().onStartStopChanged(
       [this](bool start) {
          if (start)
          {
             transportControl.start();
          }
          else
          {
             transportControl.stop();
          }
       });

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

/*
/etc/security/limits.conf
<username> hard rtprio 99
<username> soft rtprio 99
logout, login
*/
void base::Base::setRtScheduling()
{
   sched_param schedParam;
   schedParam.sched_priority = 40;
   int policy                = SCHED_FIFO;
   if (sched_setscheduler(0, policy, &schedParam) == -1)
   {
      LOG_F(ERROR, "sched_setscheduler failed: {}", strerror(errno));
   }
   pthread_setname_np(pthread_self(), "MidiemBackend Main RT");
}

void base::Base::mainRtThreadFunction(const std::atomic<bool> &terminateRequest)
{
   setRtScheduling();
   uiadapter::capnzero::RtServer rtServer(
       m_zmqContext, instruments, musicDeviceHolder,
       transportControl, tempo::BeatTick::instance().abletonLink(), midiRouter);

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

   while (!terminateRequest) { fdSet.Select(); }
}

void base::Base::loaderThreadFunction(const std::atomic<bool> &terminateRequest)
{
   uiadapter::capnzero::LoaderServer loaderServer(m_zmqContext,
                                                  musicDeviceFactory);
   uiadapter::capnzero::RtClient rtClient(m_zmqContext, loaderServer.signals(),
                                          musicDeviceFactory);
   int timerFd           = timerfd_create(CLOCK_MONOTONIC, 0);
   constexpr auto Period = std::chrono::seconds(1);
   itimerspec t({.it_interval = {Period.count(), 0}, .it_value = {1, 0}});
   timerfd_settime(timerFd, 0, &t, NULL);

   utils::FdSet fdSet;
   fdSet.AddFd(timerFd, [this](int fd) {
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
   fdSet.AddFd(rtClient.getFd(), [&rtClient](int fd) {
      rtClient.handleIncomingSignalAllNonBlock();
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

   const auto [deltaBeats, deltaTime] = tempo::BeatTick::instance().nextTick();
   {
      MeasurerTenthMs<0>::Guard guard;
      transportControl.update();
      musicDeviceHolder.midiHolder.midiClock(deltaBeats, deltaTime);
      musicDeviceHolder.midiHolder.processMidiInBuffers();
      musicDeviceHolder.musicDevices.updateSoundParameterActualValues();
   }

   musicDeviceHolder.musicDevices.updateSoundParameterUI();
   musicDeviceFactory.invokeInserterQueueActions();
   MeasurerTenthMs<1>::instance().sample();
}
