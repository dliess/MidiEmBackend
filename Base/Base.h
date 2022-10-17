#ifndef BASE_H
#define BASE_H

#include <memory>
#include <string>
#include <vector>
#include <zmq.hpp>

#include "ControllerEventRouter.h"
#include "Instruments.h"
#include "InstrumentsFactory.h"
#include "KitInstrument.h"
#include "MidiRouter.h"
#include "MusicDeviceFactory.h"
#include "MusicDeviceHolder.h"
#include "ParameterSceneContainer.h"
#include "ThreadHelpers.h"
#include "TransportControl.h"
#include "Tracks.h"

namespace base
{
struct Base
{
public:
   Base(const std::string &configDir, 
        std::string rtRpcBindAddr,
        std::string rtSignalBindAddr,
        std::string loaderRpcBindAddr,
        std::string loaderSignalBindAddr);
   ~Base() noexcept;
   musicDevice::Holder musicDeviceHolder;
   musicDevice::factory::Factory musicDeviceFactory;
   TransportControl transportControl;
   instruments::Instruments instruments;
   instruments::InstrumentsFactory instrumentsFactory;
   midifriends::Router midiRouter;
   musicDevice::controller::EventRouter controllerEventRouter;
   musicDevice::sound::ParameterSceneContainer parameterSceneContainer;
   session::Tracks tracks;
   void start();
   void waitForEnd();

private:
   zmq::context_t m_zmqContext;
   const std::string m_rtRpcBindAddr;
   const std::string m_rtSignalBindAddr;
   const std::string m_loaderRpcBindAddr;
   const std::string m_loaderSignalBindAddr;
   std::unique_ptr<util::Thread> m_mainRtThread;
   std::unique_ptr<util::Thread> m_portNotifierThread;
   void mainRtThreadFunction(const std::atomic<bool> &terminateRequest);
   void loaderThreadFunction(const std::atomic<bool> &terminateRequest);
   void loopFn();
   void setRtScheduling();
};

}   // namespace base

#endif