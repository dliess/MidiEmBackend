#ifndef MUSIC_DEVICE_SEQUENCER_H
#define MUSIC_DEVICE_SEQUENCER_H

#include "Midi1Output.h"
#include <memory>

namespace base::musicDevice
{

class Sequencer
{
public:
   void initMidiOutHandler(std::shared_ptr<midi::Midi1Output> pMidiOut) noexcept;

   void startSequencer() noexcept;
   void stopSequencer() noexcept;
private:
   std::shared_ptr<midi::Midi1Output> m_midiOutHandler;
   bool m_masked{false};
};

} // namespace base::musicDevice

#endif