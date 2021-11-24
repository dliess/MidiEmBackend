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
          static_cast<base::musicDevice::sound::lfo::Waveform>(waveform));
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
                                   const ::capnzero::TextView& presetName,
                                   const ::capnzero::TextView& category,
                                   const ::capnzero::TextView& genre)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      iter->second->soundHandler->presetHandler().storeAsSoundPreset(
          voiceIdx, std::string(presetName),
          base::musicDevice::sound::preset::create_Category(
              std::string(category)),
          base::musicDevice::sound::preset::create_Genre(std::string(genre)));
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

void SoundDevicesRpc::setArpeggiatorBypass(const ::capnzero::SpanCL<16>& uuid,
                                           ::capnzero::Int8 voiceIdx,
                                           ::capnzero::Int8 on)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      try
      {
         iter->second->soundHandler->arpeggiators().at(voiceIdx).bypass(
             static_cast<bool>(on));
      }
      catch (std::exception& e)
      {
         LOG_F(ERROR, "SoundDevicesRpc::setArpeggiatorBypass {}", e.what());
      }
   }
}

void SoundDevicesRpc::setArpeggiatorStepLength(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx,
    ::capnzero::Int32 stepLength)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      try
      {
         iter->second->soundHandler->arpeggiators().at(voiceIdx).setStepLength(
             stepLength);
      }
      catch (std::exception& e)
      {
         LOG_F(ERROR, "SoundDevicesRpc::setArpeggiatorStepLength {}", e.what());
      }
   }
}

void SoundDevicesRpc::setArpeggiatorGateFill(const ::capnzero::SpanCL<16>& uuid,
                                             ::capnzero::Int8 voiceIdx,
                                             ::capnzero::Float32 gateFill)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      try
      {
         iter->second->soundHandler->arpeggiators().at(voiceIdx).setGateFill(
             gateFill);
      }
      catch (std::exception& e)
      {
         LOG_F(ERROR, "SoundDevicesRpc::setArpeggiatorGateFill {}", e.what());
      }
   }
}

void SoundDevicesRpc::setArpeggiatorAlgorithm(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx,
    ::capnzero::MidiEmRt::ArpeggiatorAlgorithm algorithm)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      try
      {
         iter->second->soundHandler->arpeggiators().at(voiceIdx).setAlgorithm(
             static_cast<base::arp::Algorithm>(algorithm));
      }
      catch (std::exception& e)
      {
         LOG_F(ERROR, "SoundDevicesRpc::setArpeggiatorAlgorithm {}", e.what());
      }
   }
}

void SoundDevicesRpc::setArpeggiatorRange(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx,
    ::capnzero::MidiEmRt::ArpeggiatorRangeType rangeType,
    ::capnzero::Int32 range)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      try
      {
         iter->second->soundHandler->arpeggiators().at(voiceIdx).setRange(
             static_cast<base::arp::RangeType>(rangeType), range);
      }
      catch (std::exception& e)
      {
         LOG_F(ERROR, "SoundDevicesRpc::setArpeggiatorRange {}", e.what());
      }
   }
}

void SoundDevicesRpc::setArpeggiatorHoldNotes(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx,
    ::capnzero::Int8 on)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      try
      {
         iter->second->soundHandler->arpeggiators().at(voiceIdx).setHoldNotes(
             on);
      }
      catch (std::exception& e)
      {
         LOG_F(ERROR, "SoundDevicesRpc::setArpeggiatorHoldNotes {}", e.what());
      }
   }
}

void SoundDevicesRpc::setArpeggiatorFeedMode(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx,
    ::capnzero::MidiEmRt::ArpeggiatorFeedMode feedMode)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      try
      {
         iter->second->soundHandler->arpeggiators().at(voiceIdx).setFeedMode(
             static_cast<base::arp::FeedMode>(feedMode));
      }
      catch (std::exception& e)
      {
         LOG_F(ERROR, "SoundDevicesRpc::setArpeggiatorFeedMode {}", e.what());
      }
   }
}

void SoundDevicesRpc::arpeggiatorSeqInsertPause(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      try
      {
         iter->second->soundHandler->arpeggiators()
             .at(voiceIdx)
             .seqInsertPause();
      }
      catch (std::exception& e)
      {
         LOG_F(ERROR, "SoundDevicesRpc::arpeggiatorSeqInsertPause {}",
               e.what());
      }
   }
}

void SoundDevicesRpc::arpeggiatorSeqRemoveLastNote(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int8 voiceIdx)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rMusicDeviceContainer.find(uuid_);
   if (iter != m_rMusicDeviceContainer.end())
   {
      assert(iter->second->soundHandler);
      try
      {
         iter->second->soundHandler->arpeggiators()
             .at(voiceIdx)
             .seqRemoveLastNote();
      }
      catch (std::exception& e)
      {
         LOG_F(ERROR, "SoundDevicesRpc::arpeggiatorSeqRemoveLastNote {}",
               e.what());
      }
   }
}
