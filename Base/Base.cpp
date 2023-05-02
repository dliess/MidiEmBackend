#include "Base.h"

#include <spdlog/spdlog.h>
#include <sys/timerfd.h>

#include <cassert>
#include <exception>

#include "BeatTick.h"
#include "FdSet.h"
#include "InstrumentsMDChangeHandler.h"
#include "LoaderServer.h"
#include "ModifiersApplyer.h"
#include "ReplaceAsteriskToLocalhost.h"
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

base::Base::Base(const std::string &configDir, std::string rtRpcBindAddr,
                 std::string rtSignalBindAddr, std::string loaderRpcBindAddr,
                 std::string loaderSignalBindAddr) :
    m_rtRpcBindAddr(std::move(rtRpcBindAddr)),
    m_rtSignalBindAddr(std::move(rtSignalBindAddr)),
    m_loaderRpcBindAddr(std::move(loaderRpcBindAddr)),
    m_loaderSignalBindAddr(std::move(loaderSignalBindAddr)),
    musicDeviceHolder(),
    musicDeviceFactory(musicDeviceHolder, configDir),
    transportControl(musicDeviceHolder),
    instruments(musicDeviceFactory.dataHolder()),
    midiRouter(musicDeviceHolder.midiHolder),
    tracks(instruments),
    controllerEventRouter(instruments, musicDeviceHolder.musicDevices)
{
   // m_zmqContext.set(zmq::ctxopt::io_threads, 1);
   m_zmqContext.set(zmq::ctxopt::thread_name_prefix, 1);
   // m_zmqContext.set(zmq::ctxopt::thread_sched_policy, 4);
   // m_zmqContext.set(zmq::ctxopt::thread_priority, 1);
   transportControl.onStartedChanged([this](bool started) {
      if (started)
      {
         tracks.start();
      }
      else
      {
         tracks.stop();
      }
   });
   musicDeviceHolder.musicDevices.onControllerDevEventOccured(
       [this](const util::Identifiable::UUID uuid,
              const musicDevice::controller::Event &event) {
          controllerEventRouter.onControllerDevEventOccured(uuid, event);
       });
   musicDeviceFactory.dataHolder().onMusicDeviceAdded([this](auto md) {
      instruments::InstrumentsMDChangeHandler(instruments).add(md);
   });
   musicDeviceFactory.dataHolder().onMusicDeviceAboutToRemove([this](auto md) {
      instruments::InstrumentsMDChangeHandler(instruments).remove(md);
   });
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
   musicDeviceFactory.createVirtualMidiDevices();

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
   if (0 != pthread_setname_np(m_mainRtThread->native_handle(), "NMBE-Main-RT"))
   {
      spdlog::error("Could not set thread name: NMBE-Main-RT");
   }

   m_portNotifierThread = std::make_unique<util::Thread>(
       [this](const std::atomic<bool> &terminateRequest) {
          loaderThreadFunction(terminateRequest);
       });
   if (0 !=
       pthread_setname_np(m_portNotifierThread->native_handle(), "NMBE-Loader"))
   {
      spdlog::error("Could not set thread name: NMBE-Loader");
   }
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
      spdlog::error("sched_setscheduler failed: {}", strerror(errno));
   }
}

void base::Base::mainRtThreadFunction(const std::atomic<bool> &terminateRequest)
{
   setRtScheduling();
   uiadapter::capnzero::RtServer rtServer(
       m_zmqContext, m_rtRpcBindAddr, m_rtSignalBindAddr, instruments,
       musicDeviceHolder, transportControl,
       tempo::BeatTick::instance().abletonLink(), midiRouter,
       parameterSceneContainer, tracks);

   int timerFd           = timerfd_create(CLOCK_MONOTONIC, 0);
   constexpr auto Period = std::chrono::milliseconds(1);
   constexpr auto PeriodNs =
       std::chrono::duration_cast<std::chrono::nanoseconds>(Period);
   itimerspec t(
       {.it_interval = {0, PeriodNs.count()}, .it_value = {0, 1000000}});
   timerfd_settime(timerFd, 0, &t, NULL);

   int timerFdUIUpdate           = timerfd_create(CLOCK_MONOTONIC, 0);
   constexpr auto PeriodUIUpdate = std::chrono::milliseconds(50);
   constexpr auto PeriodNsUIUpdate =
       std::chrono::duration_cast<std::chrono::nanoseconds>(PeriodUIUpdate);
   itimerspec tUIUpdate({.it_interval = {0, PeriodNsUIUpdate.count()},
                         .it_value    = {0, 1000000}});
   timerfd_settime(timerFdUIUpdate, 0, &tUIUpdate, NULL);

   utils::FdSet fdSet;
   fdSet.AddFd(timerFd, [this](int fd) {
      std::array<uint8_t, 8> buf;
      read(fd, buf.data(), buf.size());
      loopFn();
   });
   fdSet.AddFd(timerFdUIUpdate, [this](int fd) {
      std::array<uint8_t, 8> buf;
      read(fd, buf.data(), buf.size());
      musicDeviceHolder.musicDevices.updateMDParameterUI();
      tracks.updateActiveClipBeatsUI();
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
   uiadapter::capnzero::LoaderServer loaderServer(
       m_zmqContext, m_loaderRpcBindAddr, m_loaderSignalBindAddr,
       musicDeviceFactory, instruments, controllerEventRouter);
   uiadapter::capnzero::RtClient rtClient(
       m_zmqContext, util::replaceAsteriskToLocalhost(m_rtRpcBindAddr),
       util::replaceAsteriskToLocalhost(m_rtSignalBindAddr),
       loaderServer.signals(), musicDeviceFactory);
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
      tracks.update();
      base::musicDevice::ModifiersApplyer(parameterSceneContainer,
                                          musicDeviceHolder.musicDevices)();
      musicDeviceHolder.musicDevices.updateSoundParameterActualValues();
   }
   musicDeviceFactory.musicDeviceInserter().invokeQueueActions();
   MeasurerTenthMs<1>::instance().sample();
}
