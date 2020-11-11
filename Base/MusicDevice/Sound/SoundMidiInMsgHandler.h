#ifndef SOUND_MIDI_IN_MSG_HANDLER_H
#define SOUND_MIDI_IN_MSG_HANDLER_H

#include <unordered_map>
#include <functional>

#include "MidiMessage.h"
#include "MidiMessageIds.h"
#include "SoundSection.h"
#include "MidiMessageIdsExtension.h"

namespace base::musicDevice
{
namespace sound
{
template<typename MidiInIfPtr>
class MidiInMsgHandler
{
public:
   using Cb = std::function<void(int voiceId, int parameterId, float value)>;
   MidiInMsgHandler(MidiInIfPtr pMidiInIf,
                    const description::sound::Section& rSoundSection,
                    Cb cb) noexcept;
private:
   MidiInIfPtr m_pMidiInIf;
   const description::sound::Section& m_rSoundSection;
   using Map =
      std::unordered_map<midi::MidiMessageId, description::sound::ParameterId>;
   Map m_map;
   static std::string cache2Str(const Map& map);
   void handleSoundDevParameterRouting(
      const midi::MidiMessage& midiMsg,
      const description::sound::ParameterId& id) const noexcept;

   int midiChannelNr2VoiceId(int midiChannel, int engineId) const noexcept;
   Cb m_drainCb;
   void initCacheBySoundSection() noexcept;
};

} // namespace sound
} // namespace base::musicDevice
#include "SoundMidiInMsgHandler.tcc"

#endif