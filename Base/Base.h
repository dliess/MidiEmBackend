#ifndef BASE_H
#define BASE_H

#include <memory>
#include <vector>
#include <string>
#include <thread>

#include "Instruments.h"
#include "InstrumentsFactory.h"
#include "KitInstrument.h"
#include "MusicDeviceFactory.h"
#include "MusicDeviceHolder.h"
#include "ThreadedLoop.h"

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

   void start();
   void waitForEnd();

private:
   std::unique_ptr<util::Thread> m_mainRtThread;
   std::unique_ptr<util::ThreadedLoop> m_portNotifierThread;
   void mainRtThreadFunction(const std::atomic<bool>& terminateRequest);
   void loopFn();
};

} // namespace base

#endif