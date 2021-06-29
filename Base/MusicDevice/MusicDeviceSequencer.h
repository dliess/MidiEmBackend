#ifndef MUSIC_DEVICE_SEQUENCER_H
#define MUSIC_DEVICE_SEQUENCER_H

#include <memory>

#include "Midi1Output.h"

namespace base::musicDevice
{
class Sequencer
{
public:
   ~Sequencer();
   void initMidiOutHandler(
       std::shared_ptr<midi::Midi1Output> pMidiOut) noexcept;

   using StartedChangedCb = std::function<void(bool)>;
   void registerStartedChangedCb(StartedChangedCb cb);
   using TransportMaskChangedCb = std::function<void(bool)>;
   void registerTransportMaskChangedCb(TransportMaskChangedCb cb);

   void start() noexcept;
   void stop() noexcept;
   void toggleEnabled() noexcept;

private:
   std::shared_ptr<midi::Midi1Output> m_midiOutHandler;
   std::vector<StartedChangedCb> m_startedChangedCbs;
   std::vector<TransportMaskChangedCb> m_transportMaskChangedCbs;
};

}   // namespace base::musicDevice

#endif