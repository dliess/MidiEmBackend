#include "InstrumentsPlayRpc.h"

#include "Instruments.h"

using namespace uiadapter::capnzero;
using namespace base::instruments;

InstrumentsPlayRpc::InstrumentsPlayRpc(
    base::instruments::Instruments& rInstruments) :
    m_rInstruments(rInstruments)
{
}

void InstrumentsPlayRpc::melodicNoteOn(const ::capnzero::SpanCL<16>& uuid,
                                       ::capnzero::Int16 note,
                                       ::capnzero::Float32 velocity)
{
   m_rInstruments.withMelodicInstrumentRt(
       uuid, [&](const MelodicInstrument& mi) { mi.noteOn(note, velocity); });
}

void InstrumentsPlayRpc::melodicNoteOff(const ::capnzero::SpanCL<16>& uuid,
                                        ::capnzero::Int16 note,
                                        ::capnzero::Float32 velocity)
{
   m_rInstruments.withMelodicInstrumentRt(
       uuid, [&](const MelodicInstrument& mi) { mi.noteOff(note, velocity); });
}

void InstrumentsPlayRpc::kitNoteOn(const ::capnzero::SpanCL<16>& uuid,
                                   ::capnzero::Int16 soundIndex,
                                   ::capnzero::Int16 note,
                                   ::capnzero::Float32 velocity)
{
   m_rInstruments.withKitInstrumentRt(uuid, [&](const KitInstrument& ki) {
      ki.noteOn(soundIndex, note, velocity);
   });
}

void InstrumentsPlayRpc::kitNoteOff(const ::capnzero::SpanCL<16>& uuid,
                                    ::capnzero::Int16 soundIndex,
                                    ::capnzero::Int16 note,
                                    ::capnzero::Float32 velocity)
{
   m_rInstruments.withKitInstrumentRt(uuid, [&](const KitInstrument& ki) {
      ki.noteOff(soundIndex, note, velocity);
   });
}

void InstrumentsPlayRpc::incrementKitParameterValue(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterAttr parameterAttr,
    ::capnzero::Float32 increment)
{
   m_rInstruments.withKitInstrumentRt(uuid, [&](const KitInstrument& ki) {
      ki.incrementParameterValue(
          voiceIdx, componentIdx, parameterIdx,
          static_cast<base::musicDevice::sound::ParameterAttr>(parameterAttr),
          increment, base::musicDevice::sound::IncrementMode::Limit);
   });
}
void InstrumentsPlayRpc::setKitParameterValue(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterAttr parameterAttr,
    ::capnzero::Float32 value)
{
   m_rInstruments.withKitInstrumentRt(uuid, [&](const KitInstrument& ki) {
      ki.setParameterValue(
          voiceIdx, componentIdx, parameterIdx,
          static_cast<base::musicDevice::sound::ParameterAttr>(parameterAttr),
          value);
   });
}
void InstrumentsPlayRpc::blancKitParameterValues(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx)
{
   // TODO
}
void InstrumentsPlayRpc::incrementMelodicParameterValue(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 componentIdx,
    ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterAttr parameterAttr,
    ::capnzero::Float32 increment)
{
   m_rInstruments.withMelodicInstrumentRt(
       uuid, [&](const MelodicInstrument& mi) {
          mi.incrementParameterValue(
              componentIdx, parameterIdx,
              static_cast<base::musicDevice::sound::ParameterAttr>(
                  parameterAttr),
              increment, base::musicDevice::sound::IncrementMode::Limit);
       });
}
void InstrumentsPlayRpc::setMelodicParameterValue(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 componentIdx,
    ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterAttr parameterAttr,
    ::capnzero::Float32 value)
{
   m_rInstruments.withMelodicInstrumentRt(
       uuid, [&](const MelodicInstrument& mi) {
          mi.setParameterValue(
              componentIdx, parameterIdx,
              static_cast<base::musicDevice::sound::ParameterAttr>(
                  parameterAttr),
              value);
       });
}
void InstrumentsPlayRpc::blancMelodicParameterValues(
    const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 componentIdx)
{
   // TODO
}
   
void InstrumentsPlayRpc::setKitComponentAmp(const ::capnzero::SpanCL<16>& uuid,
                           ::capnzero::Int16 voiceIdx,
                           ::capnzero::Int16 componentIdx,
                           ::capnzero::Float32 amp)
{

}

   
void InstrumentsPlayRpc::setKitVoiceAmp(const ::capnzero::SpanCL<16>& uuid,
                       ::capnzero::Int16 voiceIdx, ::capnzero::Float32 amp)
{

}
   
void InstrumentsPlayRpc::setMelodicComponentAmp(const ::capnzero::SpanCL<16>& uuid,
                               ::capnzero::Int16 componentIdx,
                               ::capnzero::Float32 amp) 
{

}
