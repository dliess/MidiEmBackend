#include "Base.h"

#include <cassert>
#include <exception>
#include <loguru.hpp>

#include "BeatTick.h"
#include "ThreadedLoop.h"
#include "UsbMidiPortNotifier.h"

// ----- Time measuring -----
#include "CyclicDataOutputterThread.h"
#include "Histogram.h"
#include "Measurer.h"

using DataHolderUs = TimeMeasure::Histogram<std::chrono::microseconds>;
using DataHolderMs = TimeMeasure::Histogram<std::chrono::milliseconds>;

template<unsigned int Id>
using MeasurerMs = TimeMeasure::Measurer<Id, DataHolderMs>;
template<unsigned int Id>
using MeasurerUs = TimeMeasure::Measurer<Id, DataHolderUs>;

TimeMeasure::CyclicDataOutputterThread<DataHolderUs,
                                       TimeMeasure::Destination::Udp>
   outThreadUdp({
      &MeasurerUs<0>::instance().dataHolder(),
      &MeasurerUs<1>::instance().dataHolder(),
   });
// --------------------------

base::Base::Base(const std::string& configDir) :
   musicDeviceHolder(), musicDeviceFactory(musicDeviceHolder, configDir),
   instruments(musicDeviceHolder.musicDevices),
   instrumentsFactory(instruments, musicDeviceHolder)
{
   // TODO: Remove Dummy
   instruments.load("relDir", "filename", "section");
}

void base::Base::start()
{
   MeasurerMs<0>::instance().dataHolder().setHistogramRange(100);
   MeasurerUs<0>::instance().dataHolder().setHistogramRange(100);
   MeasurerUs<1>::instance().dataHolder().setHistogramRange(100);
   
   if (!outThreadUdp.destination().connect("127.0.0.1", 12341))
   {
      exit(1);
   }
   outThreadUdp.startThread(1000);

   if (!midi::PortNotifiers::instance().init())
   {
      // TODO: put this code to Midi lib
      throw std::runtime_error("midi::PortNotifiers::instance().init() failed");
   }
   m_mainRtThread = std::make_unique<util::ThreadedLoop>(
      std::chrono::milliseconds(200), [this]() { mainRtThreadFunction(); });

   m_portNotifierThread = std::make_unique<util::ThreadedLoop>(
      std::chrono::milliseconds(2), [this]() {
         midi::PortNotifiers::instance().update();
      });
}

void base::Base::waitForEnd()
{
   assert(m_mainRtThread);
   m_mainRtThread->join();
}

void base::Base::mainRtThreadFunction()
{
   // LOG_SCOPE_FUNCTION(INFO);

   // VLOG_SCOPE_F(0, "Base::mainRtThreadFunction()");
   //{
      // VLOG_SCOPE_F(1, "PortNotifier update");
      // ... some code to measure ...
   //}

   tempo::BeatTick::instance().nextTimeSlot();
   {
      MeasurerUs<0>::Guard guard;
      musicDeviceHolder.midiHolder.midiClock();
   }
   musicDeviceHolder.midiHolder.processMidiInBuffers();
   {
      MeasurerUs<1>::Guard guard;
      musicDeviceHolder.musicDevices.updateSoundParameterActualValues();
   }
   musicDeviceHolder.musicDevices.updateSoundParameterUI();
   musicDeviceFactory.invokeInserterQueueActions();
   //MeasurerMs<0>::instance().sample();
}
