#ifndef BASE_H
#define BASE_H

#include <memory>
#include <vector>
#include <string>
#include <zmq.hpp>

#include "ThreadHelpers.h"
#include "Instruments.h"
#include "InstrumentsFactory.h"
#include "KitInstrument.h"
#include "MusicDeviceFactory.h"
#include "MusicDeviceHolder.h"
#include "TransportControl.h"

namespace base
{
struct Base
{
public:
   Base(const std::string& configDir);
   musicDevice::Holder musicDeviceHolder;
   musicDevice::Factory musicDeviceFactory;
   instruments::Instruments instruments;
   instruments::InstrumentsFactory instrumentsFactory;
   //TransportControl transportControl;
   
   void start();
   void waitForEnd();

private:
   zmq::context_t m_zmqContext;
   std::unique_ptr<util::Thread> m_mainRtThread;
   std::unique_ptr<util::Thread> m_portNotifierThread;
   void mainRtThreadFunction(const std::atomic<bool>& terminateRequest);
   void loaderThreadFunction(const std::atomic<bool>& terminateRequest);
   void loopFn();
};

} // namespace base

#endif