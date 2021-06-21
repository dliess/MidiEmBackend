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

/*
using DataHolderUs = TimeMeasure::Histogram<std::chrono::microseconds>;
using DataHolderMs = TimeMeasure::Histogram<std::chrono::milliseconds>;

template <unsigned int Id>
using MeasurerMs = TimeMeasure::Measurer<Id, DataHolderMs>;
template <unsigned int Id>
using MeasurerUs = TimeMeasure::Measurer<Id, DataHolderUs>;

TimeMeasure::CyclicDataOutputterThread<DataHolderUs,
                                       TimeMeasure::Destination::Udp>
    outThreadUdp({
        &MeasurerUs<0>::instance().dataHolder(),
        &MeasurerUs<1>::instance().dataHolder(),
    });
// --------------------------
*/

base::Base::Base(const std::string &configDir) :
    musicDeviceHolder(),
    musicDeviceFactory(musicDeviceHolder, configDir),
    instruments(musicDeviceHolder.musicDevices),
    instrumentsFactory(instruments, musicDeviceHolder) /*,
    transportControl(musicDeviceHolder.midiHolder)*/
{
   // TODO: Remove Dummy
   instruments.load("relDir", "filename", "section");
}

void base::Base::start()
{
   /*
     MeasurerMs<0>::instance().dataHolder().setHistogramRange(100);
     MeasurerUs<0>::instance().dataHolder().setHistogramRange(100);
     MeasurerUs<1>::instance().dataHolder().setHistogramRange(100);

     if (!outThreadUdp.destination().connect("127.0.0.1", 12341)) {
       exit(1);
     }
     outThreadUdp.startThread(1000);
   */
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
                                          musicDeviceHolder.musicDevices);

   int timerFd           = timerfd_create(CLOCK_MONOTONIC, 0);
   constexpr auto Period = std::chrono::milliseconds(1);
   constexpr auto PeriodNs =
       std::chrono::duration_cast<std::chrono::nanoseconds>(Period);
   itimerspec t(
       {.it_interval = {0, PeriodNs.count()}, .it_value = {0, 1000000}});
   timerfd_settime(timerFd, 0, &t, NULL);

   utils::FdSet fdSet;
   fdSet.AddFd(timerFd, [this](int fd) { loopFn(); });
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
   uiadapter::capnzero::LoaderServer loaderServer(m_zmqContext, musicDeviceFactory);
   int timerFd           = timerfd_create(CLOCK_MONOTONIC, 0);
   constexpr auto Period = std::chrono::milliseconds(1000);
   constexpr auto PeriodNs =
       std::chrono::duration_cast<std::chrono::nanoseconds>(Period);
   itimerspec t(
       {.it_interval = {0, PeriodNs.count()}, .it_value = {0, 1000000}});
   timerfd_settime(timerFd, 0, &t, NULL);

   utils::FdSet fdSet;
   fdSet.AddFd(timerFd,
               [this](int fd) { midi::PortNotifiers::instance().update(); });
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
      // MeasurerUs<0>::Guard guard;
      musicDeviceHolder.midiHolder.midiClock();
   }
   musicDeviceHolder.midiHolder.processMidiInBuffers();
   {
      // MeasurerUs<1>::Guard guard;
      musicDeviceHolder.musicDevices.updateSoundParameterActualValues();
   }

   musicDeviceHolder.musicDevices.updateSoundParameterUI();
   musicDeviceFactory.invokeInserterQueueActions();
   // MeasurerMs<0>::instance().sample();
}
