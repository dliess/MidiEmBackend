#include "SoundDevicesRpc.h"

#include "MusicDeviceContainer.h"
#include "SoundHandler.h"

using namespace uiadapter::capnzero;
using base::musicDevice::sound::SoundHandler;

SoundDevicesRpc::SoundDevicesRpc(
    base::musicDevice::MusicDeviceContainer& rMusicDeviceContainer) :
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
}

void SoundDevicesRpc::noteOn(const ::capnzero::SpanCL<16>& uuid,
                             ::capnzero::Int16 voiceIdx, ::capnzero::Int16 note,
                             ::capnzero::Float32 velocity)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.noteOn(voiceIdx, note, velocity);
   });
}

void SoundDevicesRpc::noteOff(const ::capnzero::SpanCL<16>& uuid,
                              ::capnzero::Int16 voiceIdx,
                              ::capnzero::Int16 note,
                              ::capnzero::Float32 velocity)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.noteOff(voiceIdx, note, velocity);
   });
}

void SoundDevicesRpc::pitchBend(const ::capnzero::SpanCL<16>& uuid,
                                ::capnzero::Int16 voiceIdx,
                                ::capnzero::Float32 value)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.pitchBend(voiceIdx, value);
   });
}

void SoundDevicesRpc::afterTouchPoly(const ::capnzero::SpanCL<16>& uuid,
                                     ::capnzero::Int16 voiceIdx,
                                     ::capnzero::Int16 note,
                                     ::capnzero::Float32 value)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.afterTouchPoly(voiceIdx, note, value);
   });
}

void SoundDevicesRpc::afterTouch(const ::capnzero::SpanCL<16>& uuid,
                                 ::capnzero::Int16 voiceIdx,
                                 ::capnzero::Float32 value)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.afterTouch(voiceIdx, value);
   });
}

void SoundDevicesRpc::registerForParameterChange(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 paramIdx)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      if(!soundHandler.checkValidity(voiceIdx, paramIdx))
      {
         spdlog::error("SoundDevicesRpc: invalid Parameters: voiceIdx {}, parameterIdx {}", voiceIdx, paramIdx);
         return;
      }
      soundHandler.uiShowsInterestInParameter(voiceIdx, paramIdx);
   });
}

void SoundDevicesRpc::unregisterForParameterChange(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 paramIdx)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.uiLoosesInterestInParameter(voiceIdx, paramIdx);
   });
}

void SoundDevicesRpc::incrementParameterValue(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 paramIdx, ::capnzero::Float32 increment)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.incCommandedValue(voiceIdx, paramIdx, increment, false);
      soundHandler.calcActualVal(voiceIdx, paramIdx);
   });
}

void SoundDevicesRpc::setParameterValue(const ::capnzero::SpanCL<16>& uuid,
                                        ::capnzero::Int16 voiceIdx,
                                        ::capnzero::Int16 paramIdx,
                                        ::capnzero::Float32 value)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.setCommandedValue(voiceIdx, paramIdx, value);
      soundHandler.calcActualVal(voiceIdx, paramIdx);
   });
}

void SoundDevicesRpc::blankVoiceParameter(const ::capnzero::SpanCL<16>& uuid,
                                          ::capnzero::Int16 voiceIdx,
                                          ::capnzero::Int16 paramIdx)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.blankVoiceParameter(voiceIdx, paramIdx);
      soundHandler.calcActualVal(voiceIdx, paramIdx);
   });
}

void SoundDevicesRpc::blankVoiceParameters(const ::capnzero::SpanCL<16>& uuid,
                                           ::capnzero::Int16 voiceIdx)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.blankVoiceParameters(voiceIdx);
      soundHandler.calcActualValuesForVoice(voiceIdx);
   });
}

void SoundDevicesRpc::blankAllVoiceParameters(
    const ::capnzero::SpanCL<16>& uuid)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.blankAllVoiceParameters();
      soundHandler.calcAllActualValues();
   });
}

void SoundDevicesRpc::setLFOWaveform(const ::capnzero::SpanCL<16>& uuid,
                                     ::capnzero::Int16 voiceIdx,
                                     ::capnzero::Int16 paramIdx,
                                     ::capnzero::MidiEmRt::LFOWaveform waveform)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.setLFOWaveform(
          voiceIdx, paramIdx,
          static_cast<base::musicDevice::sound::lfo::Waveform>(waveform));
      soundHandler.calcActualVal(voiceIdx, paramIdx);
   });
}

void SoundDevicesRpc::incLFOWaveform(const ::capnzero::SpanCL<16>& uuid,
                                     ::capnzero::Int16 voiceIdx,
                                     ::capnzero::Int16 paramIdx,
                                     ::capnzero::Int16 increment)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.incLFOWaveform(voiceIdx, paramIdx, increment, false);
      soundHandler.calcActualVal(voiceIdx, paramIdx);
   });
}

void SoundDevicesRpc::setLFOAmplitude(const ::capnzero::SpanCL<16>& uuid,
                                      ::capnzero::Int16 voiceIdx,
                                      ::capnzero::Int16 paramIdx,
                                      ::capnzero::Float32 amplitude)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.setLFOAmplitude(voiceIdx, paramIdx, amplitude);
      soundHandler.calcActualVal(voiceIdx, paramIdx);
   });
}

void SoundDevicesRpc::incLFOAmplitude(const ::capnzero::SpanCL<16>& uuid,
                                      ::capnzero::Int16 voiceIdx,
                                      ::capnzero::Int16 paramIdx,
                                      ::capnzero::Float32 increment)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.incLFOAmplitude(voiceIdx, paramIdx, increment);
      soundHandler.calcActualVal(voiceIdx, paramIdx);
   });
}

void SoundDevicesRpc::setLFOFrequency(const ::capnzero::SpanCL<16>& uuid,
                                      ::capnzero::Int16 voiceIdx,
                                      ::capnzero::Int16 paramIdx,
                                      ::capnzero::Float32 frequency)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.setLFOFrequency(voiceIdx, paramIdx, frequency);
      soundHandler.calcActualVal(voiceIdx, paramIdx);
   });
}

void SoundDevicesRpc::incLFOFrequency(const ::capnzero::SpanCL<16>& uuid,
                                      ::capnzero::Int16 voiceIdx,
                                      ::capnzero::Int16 paramIdx,
                                      ::capnzero::Float32 increment)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.incLFOFrequency(voiceIdx, paramIdx, increment);
      soundHandler.calcActualVal(voiceIdx, paramIdx);
   });
}

void SoundDevicesRpc::setLFOMultiplierExp(const ::capnzero::SpanCL<16>& uuid,
                                          ::capnzero::Int16 voiceIdx,
                                          ::capnzero::Int16 paramIdx,
                                          ::capnzero::UInt32 exponent)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.setLFOMultiplierExp(voiceIdx, paramIdx, exponent);
      soundHandler.calcActualVal(voiceIdx, paramIdx);
   });
}

void SoundDevicesRpc::incLFOMultiplierExp(const ::capnzero::SpanCL<16>& uuid,
                                          ::capnzero::Int16 voiceIdx,
                                          ::capnzero::Int16 paramIdx,
                                          ::capnzero::Int32 increment)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.incLFOMultiplierExp(voiceIdx, paramIdx, increment, false);
      soundHandler.calcActualVal(voiceIdx, paramIdx);
   });
}

void SoundDevicesRpc::stageParameterValues(const ::capnzero::SpanCL<16>& uuid,
                                           ::capnzero::Int16 voiceIdx)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.presetHandler().stageCurrentState(voiceIdx);
   });
}

void SoundDevicesRpc::restoreToStagedParameterValues(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.presetHandler().resetToStaged(voiceIdx);
   });
}

void SoundDevicesRpc::restoreToLastActualPreset(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.presetHandler().resetToActualSoundPreset(voiceIdx);
   });
}

void SoundDevicesRpc::saveAsPreset(const ::capnzero::SpanCL<16>& uuid,
                                   ::capnzero::Int16 voiceIdx,
                                   const ::capnzero::TextView& presetName,
                                   const ::capnzero::TextView& category,
                                   const ::capnzero::TextView& genre)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.presetHandler().storeAsSoundPreset(
          voiceIdx, std::string(presetName),
          base::musicDevice::sound::preset::create_Category(
              std::string(category)),
          base::musicDevice::sound::preset::create_Genre(std::string(genre)));
      soundHandler.presetHandler().selectSoundPreset(voiceIdx,
                                                     std::string(presetName));
   });
}

void SoundDevicesRpc::setActualPreset(const ::capnzero::SpanCL<16>& uuid,
                                      ::capnzero::Int16 voiceIdx,
                                      const ::capnzero::TextView& presetName)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.presetHandler().selectSoundPreset(voiceIdx,
                                                     std::string(presetName));
   });
}

void SoundDevicesRpc::sendParameterDumpRequest(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.doParameterDumpRequest();
   });
}

void SoundDevicesRpc::setArpeggiatorBypass(const ::capnzero::SpanCL<16>& uuid,
                                           ::capnzero::Int16 voiceIdx,
                                           ::capnzero::Int16 on)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.arpeggiators().at(voiceIdx).bypass(static_cast<bool>(on));
   });
}

void SoundDevicesRpc::setArpeggiatorStepLength(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Float32 stepLength)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.arpeggiators().at(voiceIdx).setStepLength(stepLength);
   });
}

void SoundDevicesRpc::setArpeggiatorGateFill(const ::capnzero::SpanCL<16>& uuid,
                                             ::capnzero::Int16 voiceIdx,
                                             ::capnzero::Float32 gateFill)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.arpeggiators().at(voiceIdx).setGateFill(gateFill);
   });
}

void SoundDevicesRpc::setArpeggiatorAlgorithm(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::MidiEmRt::ArpeggiatorAlgorithm algorithm)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.arpeggiators().at(voiceIdx).setAlgorithm(
          static_cast<base::arp::Algorithm>(algorithm));
   });
}

void SoundDevicesRpc::setArpeggiatorRange(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::MidiEmRt::ArpeggiatorRangeType rangeType,
    ::capnzero::Int32 range)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.arpeggiators().at(voiceIdx).setRange(
          static_cast<base::arp::RangeType>(rangeType), range);
   });
}

void SoundDevicesRpc::setArpeggiatorHoldNotes(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 on)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.arpeggiators().at(voiceIdx).setHoldNotes(on);
   });
}

void SoundDevicesRpc::setArpeggiatorFeedMode(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::MidiEmRt::ArpeggiatorFeedMode feedMode)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.arpeggiators().at(voiceIdx).setFeedMode(
          static_cast<base::arp::FeedMode>(feedMode));
   });
}

void SoundDevicesRpc::arpeggiatorSeqInsertPause(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.arpeggiators().at(voiceIdx).seqInsertPause();
   });
}

void SoundDevicesRpc::arpeggiatorSeqRemoveLastNote(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx)
{
   withSoundHandlerDo(uuid, [=](SoundHandler& soundHandler) {
      soundHandler.arpeggiators().at(voiceIdx).seqRemoveLastNote();
   });
}
