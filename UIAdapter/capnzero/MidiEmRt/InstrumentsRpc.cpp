#include "InstrumentsRpc.h"

#include "Instruments.h"

using namespace uiadapter::capnzero;
using namespace base::instruments;

InstrumentsRpc::InstrumentsRpc(base::instruments::Instruments& rInstruments) :
    m_rInstruments(rInstruments)
{
}

void InstrumentsRpc::createMelodicInstrument(const ::capnzero::TextView& name)
{
   m_rInstruments.createMelodicInstrument(std::string(name));
}

void InstrumentsRpc::removeMelodicInstrument(const ::capnzero::SpanCL<16>& uuid)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   m_rInstruments.removeMelodicInstrument(uuid_);
}

void InstrumentsRpc::createNewSlotInMelodicInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid,
    const ::capnzero::SpanCL<16>& soundDeviceUuid, ::capnzero::Int16 voiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::copy(instrumentUuid.begin(), instrumentUuid.end(), instrumentUuid_.begin());
   util::Identifiable::UUID soundDeviceUuid_;
   std::copy(soundDeviceUuid.begin(), soundDeviceUuid.end(), soundDeviceUuid_.begin());
   m_rInstruments.createNewSlotInMelodicInstrument(instrumentUuid_, soundDeviceUuid_, voiceIdx);
}

void InstrumentsRpc::addVoiceToMelodicInstrumentSlot(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
    const ::capnzero::SpanCL<16>& soundDeviceUuid, ::capnzero::Int16 voiceIdx)
{
}
void InstrumentsRpc::removeVoiceFromMelodicInstrumentSlot(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
    const ::capnzero::SpanCL<16>& soundDeviceUuid, ::capnzero::Int16 voiceIdx)
{
}
void InstrumentsRpc::removeSlotFromMelodicInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx)
{
}
void InstrumentsRpc::setNoteOffsetInMelodicInstrumentVoice(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 noteOffset)
{
}

void InstrumentsRpc::createKitInstrument(const ::capnzero::TextView& name) {}
void InstrumentsRpc::removeKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUUID)
{
}

void InstrumentsRpc::createNewSlotInKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid,
    const ::capnzero::SpanCL<16>& soundDeviceUuid, ::capnzero::Int16 voiceIdx)
{
}
void InstrumentsRpc::addVoiceToKitInstrumentSlot(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
    const ::capnzero::SpanCL<16>& soundDeviceUuid, ::capnzero::Int16 voiceIdx)
{
}
void InstrumentsRpc::removeVoiceFromKitInstrumentSlot(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
    const ::capnzero::SpanCL<16>& soundDeviceUuid, ::capnzero::Int16 voiceIdx)
{
}
void InstrumentsRpc::removeSlotFromKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx)
{
}
void InstrumentsRpc::setNoteOffsetInKitInstrumentVoice(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 noteOffset)
{
}

void InstrumentsRpc::melodicNoteOn(const ::capnzero::SpanCL<16>& uuid,
                                   ::capnzero::Int16 note,
                                   ::capnzero::Float32 velocity)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rInstruments.data.melodicInstruments.findByUUID(uuid_);
   if (iter != m_rInstruments.data.melodicInstruments.end())
   {
      iter->get()->noteOn(note, velocity);
      return;
   }
}

void InstrumentsRpc::melodicNoteOff(const ::capnzero::SpanCL<16>& uuid,
                                    ::capnzero::Int16 note,
                                    ::capnzero::Float32 velocity)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rInstruments.data.melodicInstruments.findByUUID(uuid_);
   if (iter != m_rInstruments.data.melodicInstruments.end())
   {
      iter->get()->noteOff(note, velocity);
      return;
   }
}

void InstrumentsRpc::kitNoteOn(const ::capnzero::SpanCL<16>& uuid,
                               ::capnzero::Int16 soundIndex,
                               ::capnzero::Int16 note,
                               ::capnzero::Float32 velocity)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rInstruments.data.kitInstruments.findByUUID(uuid_);
   if (iter != m_rInstruments.data.kitInstruments.end())
   {
      iter->get()->noteOn(soundIndex, note, velocity);
      return;
   }
}

void InstrumentsRpc::kitNoteOff(const ::capnzero::SpanCL<16>& uuid,
                                ::capnzero::Int16 soundIndex,
                                ::capnzero::Int16 note,
                                ::capnzero::Float32 velocity)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = m_rInstruments.data.kitInstruments.findByUUID(uuid_);
   if (iter != m_rInstruments.data.kitInstruments.end())
   {
      iter->get()->noteOff(soundIndex, note, velocity);
      return;
   }
}
