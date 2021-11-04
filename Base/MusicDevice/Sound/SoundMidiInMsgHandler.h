#ifndef SOUND_MIDI_IN_MSG_HANDLER_H
#define SOUND_MIDI_IN_MSG_HANDLER_H

#include <functional>
#include <unordered_map>

#include "MidiMessage.h"
#include "MidiMessageIds.h"
#include "MidiMessageIdsExtension.h"
#include "SoundSection.h"
#include "SoundMidiInSysExDumpHandler.h"

namespace base::musicDevice
{
namespace sound
{
template <typename MidiInIfPtr> class MidiInMsgHandler
{
public:
   using Cb = std::function<void(int voiceId, int parameterId, float value)>;
   MidiInMsgHandler(MidiInIfPtr pMidiInIf,
                    const description::sound::Section& rSoundSection,
                    uint8_t midiVoiceOffset, Cb cb) noexcept;

   void changeMapping(int voiceIdx,
      const std::string& compNamePrev, const std::string& compNameNew) noexcept;

private:
   MidiInSysExDumpHandler m_sysExHandler;
   MidiInIfPtr m_pMidiInIf;
   const description::sound::Section& m_rSoundSection;
   using Map =
       std::unordered_map<midi::MidiMessageId, description::sound::ParameterId>;
   std::vector<Map> m_maps;
   static std::string cache2Str(const Map& map);

   std::optional<int> getVoiceIdFromMidiMsg(const midi::MidiMessage& midiMsg) const noexcept;
   float getValueBy(
       const midi::MidiMessage& midiMsg,
       const description::sound::ParameterId& parameterId) const noexcept;

   uint8_t m_midiVoiceOffset{0};
   Cb m_drainCb;
   void initCacheBySoundSection() noexcept;
};

}   // namespace sound
}   // namespace base::musicDevice
#include "SoundMidiInMsgHandler.tcc"

#endif