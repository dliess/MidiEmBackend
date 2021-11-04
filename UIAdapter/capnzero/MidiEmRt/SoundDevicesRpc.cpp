#include "SoundDevicesRpc.h"

#include "MusicDeviceContainer.h"

using namespace uiadapter::capnzero;

SoundDevicesRpc::SoundDevicesRpc(
    base::musicDevice::MusicDeviceContainer& rMusicDeviceContainer) :
    m_rMusicDeviceContainer(rMusicDeviceContainer)
{
}

void SoundDevicesRpc::noteOn(const ::capnzero::SpanCL<16>& uuid,
                             ::capnzero::Int8 voiceIdx, ::capnzero::UInt8 note,
                             ::capnzero::Float32 velocity)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->noteOn(voiceIdx, note, velocity);
   }
}

void SoundDevicesRpc::noteOff(const ::capnzero::SpanCL<16>& uuid,
                              ::capnzero::Int8 voiceIdx, ::capnzero::UInt8 note,
                              ::capnzero::Float32 velocity)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->noteOff(voiceIdx, note, velocity);
   }
}

void SoundDevicesRpc::pitchBend(const ::capnzero::SpanCL<16>& uuid,
                                ::capnzero::Int8 voiceIdx,
                                ::capnzero::Float32 value)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->pitchBend(voiceIdx, value);
   }
}

void SoundDevicesRpc::afterTouchPoly(const ::capnzero::SpanCL<16>& uuid,
                                     ::capnzero::Int8 voiceIdx,
                                     ::capnzero::UInt8 note,
                                     ::capnzero::Float32 value)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->afterTouchPoly(voiceIdx, note, value);
   }
}

void SoundDevicesRpc::afterTouch(const ::capnzero::SpanCL<16>& uuid,
                                 ::capnzero::Int8 voiceIdx,
                                 ::capnzero::Float32 value)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->afterTouch(voiceIdx, value);
   }
}

void SoundDevicesRpc::registerForParameterChange(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx,
    ::capnzero::Int16 paramIdx)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->uiShowsInterestInParameter(voiceIdx,
                                                             paramIdx);
   }
}

void SoundDevicesRpc::unregisterForParameterChange(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx,
    ::capnzero::Int16 paramIdx)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->uiLoosesInterestInParameter(voiceIdx,
                                                              paramIdx);
   }
}

void SoundDevicesRpc::incrementParameterValue(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx,
    ::capnzero::Int16 paramIdx, ::capnzero::Float32 increment)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->incrementParameterValue(voiceIdx, paramIdx,
                                                          increment);
   }
}

void SoundDevicesRpc::setParameterValue(const ::capnzero::SpanCL<16>& uuid,
                                        ::capnzero::Int8 voiceIdx,
                                        ::capnzero::Int16 paramIdx,
                                        ::capnzero::Float32 value)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->setParameterValue(voiceIdx, paramIdx, value);
   }
}

void SoundDevicesRpc::blankVoiceParameter(const ::capnzero::SpanCL<16>& uuid,
                                          ::capnzero::Int8 voiceIdx,
                                          ::capnzero::Int16 paramIdx)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->blankVoiceParameter(voiceIdx, paramIdx);
   }
}

void SoundDevicesRpc::blankVoiceParameters(const ::capnzero::SpanCL<16>& uuid,
                                           ::capnzero::Int8 voiceIdx)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->blankVoiceParameters(voiceIdx);
   }
}

void SoundDevicesRpc::blankAllVoiceParameters(
    const ::capnzero::SpanCL<16>& uuid)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->blankAllVoiceParameters();
   }
}

void SoundDevicesRpc::setLFOWaveform(const ::capnzero::SpanCL<16>& uuid,
                                     ::capnzero::Int8 voiceIdx,
                                     ::capnzero::Int16 paramIdx,
                                     ::capnzero::MidiEmRt::LFOWaveform waveform)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->setLFOWaveform(
          voiceIdx, paramIdx,
          static_cast<base::musicDevice::sound::LFO::Waveform>(waveform));
   }
}

void SoundDevicesRpc::incLFOWaveform(const ::capnzero::SpanCL<16>& uuid,
                                     ::capnzero::Int8 voiceIdx,
                                     ::capnzero::Int16 paramIdx,
                                     ::capnzero::Int8 increment)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->incLFOWaveform(voiceIdx, paramIdx, increment);
   }
}

void SoundDevicesRpc::setLFOAmplitude(const ::capnzero::SpanCL<16>& uuid,
                                      ::capnzero::Int8 voiceIdx,
                                      ::capnzero::Int16 paramIdx,
                                      ::capnzero::Float32 amplitude)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->setLFOAmplitude(voiceIdx, paramIdx,
                                                  amplitude);
   }
}

void SoundDevicesRpc::incLFOAmplitude(const ::capnzero::SpanCL<16>& uuid,
                                      ::capnzero::Int8 voiceIdx,
                                      ::capnzero::Int16 paramIdx,
                                      ::capnzero::Float32 increment)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->incLFOAmplitude(voiceIdx, paramIdx,
                                                  increment);
   }
}

void SoundDevicesRpc::setLFOFrequency(const ::capnzero::SpanCL<16>& uuid,
                                      ::capnzero::Int8 voiceIdx,
                                      ::capnzero::Int16 paramIdx,
                                      ::capnzero::Float32 frequency)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->setLFOFrequency(voiceIdx, paramIdx,
                                                  frequency);
   }
}

void SoundDevicesRpc::incLFOFrequency(const ::capnzero::SpanCL<16>& uuid,
                                      ::capnzero::Int8 voiceIdx,
                                      ::capnzero::Int16 paramIdx,
                                      ::capnzero::Float32 increment)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->incLFOFrequency(voiceIdx, paramIdx,
                                                  increment);
   }
}

void SoundDevicesRpc::setLFOMultiplierExp(const ::capnzero::SpanCL<16>& uuid,
                                          ::capnzero::Int8 voiceIdx,
                                          ::capnzero::Int16 paramIdx,
                                          ::capnzero::UInt32 exponent)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->setLFOMultiplierExp(voiceIdx, paramIdx,
                                                      exponent);
   }
}

void SoundDevicesRpc::incLFOMultiplierExp(const ::capnzero::SpanCL<16>& uuid,
                                          ::capnzero::Int8 voiceIdx,
                                          ::capnzero::Int16 paramIdx,
                                          ::capnzero::Int32 increment)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->incLFOMultiplierExp(voiceIdx, paramIdx,
                                                      increment);
   }
}

void SoundDevicesRpc::stageParameterValues(const ::capnzero::SpanCL<16>& uuid,
                                           ::capnzero::Int8 voiceIdx)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->presetHandler().stageCurrentState(voiceIdx);
   }
}

void SoundDevicesRpc::restoreToStagedParameterValues(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->presetHandler().resetToStaged(voiceIdx);
   }
}

void SoundDevicesRpc::restoreToLastActualPreset(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->presetHandler().resetToActualSoundPreset(
          voiceIdx);
   }
}

void SoundDevicesRpc::saveAsPreset(const ::capnzero::SpanCL<16>& uuid,
                                   ::capnzero::Int8 voiceIdx,
                                   const ::capnzero::TextView& presetName)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->presetHandler().storeAsSoundPreset(
          voiceIdx, std::string(presetName));
   }
}

void SoundDevicesRpc::setActualPreset(const ::capnzero::SpanCL<16>& uuid,
                                      ::capnzero::Int8 voiceIdx,
                                      const ::capnzero::TextView& presetName)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->presetHandler().selectSoundPreset(
          voiceIdx, std::string(presetName));
   }
}

void SoundDevicesRpc::sendParameterDumpRequest(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->doParameterDumpRequest();
   }
}
