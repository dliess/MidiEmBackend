#ifndef MUSIC_DEVICE_SOUND_HANDLER_H
#define MUSIC_DEVICE_SOUND_HANDLER_H

#include <memory>
#include <string>

#include "SoundHandlerTypes.h"
#include "SoundParameterStorage.h"
#include "SoundPresetHandler.h"
#include "SoundSection.h"
#include "Arpeggiator.h"
#include "CallbackSignal.h"

// namespace midi { template<typename MessageDrain> class Midi1Input; }
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
namespace preset
{
class DevicePresets;
} // namespace preset
class SoundHandler
{
public:
   SoundHandler(std::string deviceName,
                const description::sound::Section& rSoundSection,
                std::shared_ptr<preset::DevicePresets> soundPresets) noexcept;
   ~SoundHandler();
   SoundHandler(const SoundHandler& other) = delete;
   SoundHandler& operator=(const SoundHandler& other) = delete;
   SoundHandler(SoundHandler&& other) noexcept;

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
   std::shared_ptr<preset::DevicePresets> presets() const noexcept;
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

   void setLFOWaveform(int voiceId, int paramIdx, lfo::Waveform waveform) noexcept;
   void setLFOAmplitude(int voiceIndex, int paramIdx, float amplitude) noexcept;
   void setLFOFrequency(int voiceIndex, int paramIdx, float frequency) noexcept;
   void setLFOMultiplierExp(int voiceIndex, int paramIdx, int multiplExp) noexcept;
   void incLFOWaveform(int voiceId, int paramIdx, int increment) noexcept;
   void incLFOAmplitude(int voiceIndex, int paramIdx, float increment) noexcept;
   void incLFOFrequency(int voiceIndex, int paramIdx, float increment) noexcept;
   void incLFOMultiplierExp(int voiceIndex, int paramIdx, int increment) noexcept;

   // TODO: do we need this?
   // float getParameterValue(int voiceId, int parameterId) const noexcept;
   // inline SoundPresetHandler* soundPresetHandler() noexcept;
   // inline const SoundPresetHandler* soundPresetHandler() const noexcept;

   void doParameterDumpRequest() noexcept;

   const preset::PresetHandler& presetHandler() const noexcept;
   preset::PresetHandler& presetHandler() noexcept;

   std::vector<arp::Arpeggiator>& arpeggiators() noexcept;

   CB_SIGNAL(LFOWaveformChanged, int, int, lfo::Waveform);
   CB_SIGNAL(LFOAmplitudeChanged, int, int, float);
   CB_SIGNAL(LFOFrequencyChanged, int, int, float);
   CB_SIGNAL(LFOMultiplierExpChanged, int, int, uint32_t);
   CB_SIGNAL(ActualPresetChanged, int, const std::string&);
private:
   std::string m_deviceName;
   const description::sound::Section& m_rSoundSection;
   int m_midiVoiceOffset{0};
   std::unique_ptr<MidiInMsgHandlerT> m_midiInMsgHandler;
   std::unique_ptr<MidiOutMsgHandlerT> m_midiOutHandler;
   ParameterStorage m_paramStorage;
   preset::PresetHandler m_presetHandler;
   std::vector<arp::Arpeggiator> m_arpeggiators;
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