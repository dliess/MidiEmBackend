#ifndef MUSIC_DEVICE_SOUND_HANDLER_H
#define MUSIC_DEVICE_SOUND_HANDLER_H

#include <memory>
#include <string>

#include "DoubleBufferedMessageDrain.h"
#include "Midi1Input.h"
#include "SoundParameterStorage.h"
#include "SoundPresetHandler.h"
#include "SoundSection.h"

// namespace midi { template<typename MessageDrain> class Midi1Input; }
namespace midi
{
class Midi1Output;
}

namespace base::musicDevice
{
namespace description::sound
{
struct Section;
}
namespace sound
{
template<typename T>
class MidiInMsgHandler;
template<typename T>
class MidiOutMsgHandler;
class SoundPresets;

class SoundHandler
{
public:
   SoundHandler(std::string deviceName,
                const description::sound::Section& rSoundSection,
                std::shared_ptr<SoundPresets> soundPresets) noexcept;
   ~SoundHandler();
   SoundHandler(const SoundHandler& other) = delete;
   SoundHandler& operator=(const SoundHandler& other) = delete;
   SoundHandler(SoundHandler&& other) noexcept;

   using MidiInput  = midi::Midi1Input<midi::DoubleBufferedMessageDrain>;
   using MidiOutput = midi::Midi1Output;
   void initMidiInHandler(std::shared_ptr<MidiInput> pMidiIn, uint8_t midiVoiceOffset) noexcept;
   void initMidiOutHandler(std::shared_ptr<MidiOutput> pMidiOut, uint8_t midiVoiceOffset) noexcept;
   void initEvdevHandler();
   void noteOn(int voiceIndex, int note, float velocity) noexcept;
   void noteOff(int voiceIndex, int note, float velocity) noexcept;
   void pitchBend(int voiceIndex, float value) noexcept;
   void afterTouchPoly(int voiceIndex, int note, float value) noexcept;
   void afterTouch(int voiceIndex, float value) noexcept;
   void setParameterValue(int voiceId, int parameterId, float value) noexcept;
   void incrementParameterValue(int voiceId, int parameterId, float increment) noexcept;
   void updateActualSoundStorageValues() noexcept;
   std::shared_ptr<SoundPresets> presets() const noexcept;
   constexpr static int ALL = ParameterStorage::ALL;
   void uiShowsInterestInParameter(int voiceId, int parameterId = ALL) noexcept;
   void uiLoosesInterestInParameter(int voiceId, int parameterId = ALL) noexcept;
   template<typename Cb>
   void forEachParameter(Cb&& cb) const noexcept;
   template<typename Cb>
   void forEachParameter(Cb&& cb) noexcept;

   uint8_t getMidiVoiceOffset() const noexcept;

   void blankVoiceParameter(int voiceId, int paramIdx) noexcept;
   void blankVoiceParameters(int voiceId) noexcept;
   void blankAllVoiceParameters() noexcept;

   // TODO: do we need this?
   // float getParameterValue(int voiceId, int parameterId) const noexcept;
   // inline SoundPresetHandler* soundPresetHandler() noexcept;
   // inline const SoundPresetHandler* soundPresetHandler() const noexcept;

private:
   std::string m_deviceName;
   const description::sound::Section& m_rSoundSection;
   using MidiInMsgHandlerT  = MidiInMsgHandler<std::shared_ptr<MidiInput>>;
   using MidiOutMsgHandlerT = MidiOutMsgHandler<std::shared_ptr<MidiOutput>>;
   int m_midiVoiceOffset{0};
   std::unique_ptr<MidiInMsgHandlerT> m_midiInMsgHandler;
   std::unique_ptr<MidiOutMsgHandlerT> m_midiOutHandler;
   ParameterStorage m_paramStorage;
   PresetHandler m_presetHandler;

   void doParameterDumpRequest() noexcept;
};



template<typename Cb>
void SoundHandler::forEachParameter(Cb&& cb) const noexcept
{
   m_paramStorage.forEachParameter(cb);
}

template<typename Cb>
void SoundHandler::forEachParameter(Cb&& cb) noexcept
{
   m_paramStorage.forEachParameter(cb);
}

} // namespace sound
} // namespace base::musicDevice

#endif