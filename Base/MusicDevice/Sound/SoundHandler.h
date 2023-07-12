#ifndef MUSIC_DEVICE_SOUND_HANDLER_H
#define MUSIC_DEVICE_SOUND_HANDLER_H

#include <memory>
#include <string>

#include "Arpeggiator.h"
#include "CallbackSignal.h"
#include "SoundHandlerTypes.h"
#include "SoundParameterStorage.h"
#include "SoundPresetHandler.h"
#include "SoundSection.h"

// namespace midi { template<typename MessageDrain> class Midi1Input; }

// clang-format off
namespace base::musicDevice::description::sound { struct Section; }
namespace base::musicDevice::sound { template <typename T> class MidiInMsgHandler; }
namespace base::musicDevice::sound { template <typename T> class MidiOutMsgHandler; }
namespace base::musicDevice::sound::preset { class DevicePresets; }
// clang-format on

namespace base::musicDevice::sound
{
    
class SoundHandler
{
public:
   SoundHandler(
       std::string deviceName, const description::sound::Section& rSoundSection,
       std::shared_ptr<preset::DevicePresets> soundPresets,
       std::shared_ptr<std::vector<std::string>> pActualPresetNames) noexcept;
   ~SoundHandler();
   SoundHandler(const SoundHandler& other) = delete;
   SoundHandler& operator=(const SoundHandler& other) = delete;
   SoundHandler(SoundHandler&& other) noexcept;

   void initMidiInHandler(std::shared_ptr<MidiInput> pMidiIn,
                          uint8_t midiVoiceOffset) noexcept;
   void initMidiOutHandler(std::shared_ptr<MidiOutput> pMidiOut,
                           uint8_t midiVoiceOffset) noexcept;
   void initEvdevHandler();
   void noteOn(int voiceIdx, int note, float velocity) noexcept;
   void noteOff(int voiceIdx, int note, float velocity) noexcept;
   void pitchBend(int voiceIdx, float value) noexcept;
   void afterTouchPoly(int voiceIdx, int note, float value) noexcept;
   void afterTouch(int voiceIdx, float value) noexcept;
   void setParameterValue(int voiceId, int parameterId,
                          ParameterAttr parameterAttr, float value) noexcept;
   void setRelativeParameterValue(
       int voiceIdx, int parameterId,
       musicDevice::sound::ParameterAttr parameterAttr, float relValue) const;
   [[nodiscard]] std::optional<float> getParameterValue(
       int voiceId, int parameterId,
       ParameterAttr parameterAttr = ParameterAttr::Commanded) const noexcept;
   [[nodiscard]] ValueRangeEnd getParameterRangeEnd(
       int voiceId, int parameterId,
       ParameterAttr parameterAttr = ParameterAttr::Commanded) const;
   [[nodiscard]] float fromNormalizedValue(
       int voiceId, int parameterId, ParameterAttr parameterAttr,
       float percentageValue) const noexcept;
   [[nodiscard]] const description::sound::Parameter& parameterDescription(
       int voiceIdx, int parameterIdx) const;
   void incrementParameterValue(int voiceId, int parameterId,
                                ParameterAttr parameterAttr, float increment,
                                bool roundRobin = false) noexcept;
   void incrementParameterValueEventBound(int voiceId, int parameterId,
                                          ParameterAttr parameterAttr,
                                          float increment,
                                          bool roundRobin = false) noexcept;
   void updateActualSoundStorageValues() noexcept;
   inline std::optional<std::string> getActualPresetOfVoice(
       int voiceId) const noexcept;
   std::shared_ptr<preset::DevicePresets> presets() const noexcept;
   constexpr static int ALL = ParameterStorage::ALL;
   void uiShowsInterestInParameter(int voiceId, int parameterId = ALL) noexcept;
   void uiLoosesInterestInParameter(int voiceId,
                                    int parameterId = ALL) noexcept;
   template <typename Cb> void forEachParameter(Cb&& cb) const noexcept;
   template <typename Cb> void forEachParameter(Cb&& cb) noexcept;

   uint8_t getMidiVoiceOffset() const noexcept;

   void blankVoiceParameter(int voiceId, int paramIdx) noexcept;
   void blankVoiceParameters(int voiceId) noexcept;
   void blankAllVoiceParameters() noexcept;

   void setCommandedValue(int voiceId, int parameterId, float value) noexcept;
   void setLFOWaveform(int voiceId, int paramIdx,
                       lfo::Waveform waveform) noexcept;
   void setLFOAmplitude(int voiceIdx, int paramIdx, float amplitude) noexcept;
   void setLFOFrequency(int voiceIdx, int paramIdx, float frequency) noexcept;
   void setLFOMultiplierExp(int voiceIdx, int paramIdx,
                            int multiplExp) noexcept;

   void incCommandedValue(int voiceId, int parameterId, float increment,
                          bool roundRobin) noexcept;
   void incLFOWaveform(int voiceId, int paramIdx, int increment,
                       bool roundRobin) noexcept;
   void incLFOAmplitude(int voiceIdx, int paramIdx, float increment) noexcept;
   void incLFOFrequency(int voiceIdx, int paramIdx, float increment) noexcept;
   void incLFOMultiplierExp(int voiceIdx, int paramIdx, int increment,
                            bool roundRobin) noexcept;

   void applyModifier(int voiceIdx, int paramIdx, ParameterAttr parameterAttr,
                      float destValue, float intensity) noexcept;
   void resetModifier(int voiceIdx, int paramIdx, ParameterAttr parameterAttr);
   void calcActualVal(int voiceIdx, int paramIdx);

   // TODO: do we need this?
   // inline SoundPresetHandler* soundPresetHandler() noexcept;
   // inline const SoundPresetHandler* soundPresetHandler() const noexcept;

   void doParameterDumpRequest() noexcept;

   const preset::PresetHandler& presetHandler() const noexcept;
   preset::PresetHandler& presetHandler() noexcept;

   std::vector<arp::Arpeggiator>& arpeggiators() noexcept;

   [[nodiscard]] bool checkValidity(int voiceIdx,
                                    int parameterIdx) const noexcept;

   void triggerUICallbacks() noexcept;
   CB_SIGNAL(SoundDevParamChanged, int, int, float, float);
   CB_SIGNAL(LFOWaveformChanged, int, int, lfo::Waveform);
   CB_SIGNAL(LFOAmplitudeChanged, int, int, float);
   CB_SIGNAL(LFOFrequencyChanged, int, int, float);
   CB_SIGNAL(LFOMultiplierExpChanged, int, int, uint32_t);
   CB_SIGNAL(ActualPresetChanged, int, const std::string&);

   void* lastplayerId{nullptr};

private:
   std::string m_deviceName;
   const description::sound::Section& m_rSoundSection;
   int m_midiVoiceOffset{0};
   std::unique_ptr<MidiInMsgHandlerT> m_midiInMsgHandler;
   std::unique_ptr<MidiOutMsgHandlerT> m_midiOutHandler;
   ParameterStorage m_paramStorage;
   preset::PresetHandler m_presetHandler;
   std::vector<arp::Arpeggiator> m_arpeggiators;

   void handleComponentSelectorParamChange(
       const description::sound::Parameter& paramDescr, int voiceIdx,
       float oldVal, float newVal);
};

template <typename Cb>
void SoundHandler::forEachParameter(Cb&& cb) const noexcept
{
   m_paramStorage.forEachParameter(cb);
}

template <typename Cb> void SoundHandler::forEachParameter(Cb&& cb) noexcept
{
   m_paramStorage.forEachParameter(cb);
}

inline std::optional<std::string> SoundHandler::getActualPresetOfVoice(
    int voiceId) const noexcept
{
   return m_paramStorage.getActualPresetOfVoice(voiceId);
}

}   // namespace base::musicDevice::sound

#endif