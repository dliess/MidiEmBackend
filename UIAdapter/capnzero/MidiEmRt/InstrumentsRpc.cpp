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
void InstrumentsRpc::renameMelodicInstrument(const ::capnzero::SpanCL<16>& instrumentUUID,
                                             const ::capnzero::TextView& name)
{
   util::Identifiable::UUID instrumentUUID_;
   std::copy(instrumentUUID.begin(), instrumentUUID.end(),
             instrumentUUID_.begin());
   m_rInstruments.renameMelodicInstrument(instrumentUUID_, std::string(name));
}

void InstrumentsRpc::createNewSlotInMelodicInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid,
    const ::capnzero::SpanCL<16>& soundDeviceUuid, ::capnzero::Int16 voiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::copy(instrumentUuid.begin(), instrumentUuid.end(),
             instrumentUuid_.begin());
   util::Identifiable::UUID soundDeviceUuid_;
   std::copy(soundDeviceUuid.begin(), soundDeviceUuid.end(),
             soundDeviceUuid_.begin());
   m_rInstruments.createNewSlotInMelodicInstrument(instrumentUuid_,
                                                   soundDeviceUuid_, voiceIdx);
}

void InstrumentsRpc::addVoiceToMelodicInstrumentSlot(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
    const ::capnzero::SpanCL<16>& soundDeviceUuid, ::capnzero::Int16 voiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::copy(instrumentUuid.begin(), instrumentUuid.end(),
             instrumentUuid_.begin());
   util::Identifiable::UUID soundDeviceUuid_;
   std::copy(soundDeviceUuid.begin(), soundDeviceUuid.end(),
             soundDeviceUuid_.begin());
   m_rInstruments.addVoiceToMelodicInstrumentSlot(instrumentUuid_, slotIdx,
                                                  soundDeviceUuid_, voiceIdx);
}

void InstrumentsRpc::removeVoiceFromMelodicInstrumentSlot(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
    ::capnzero::Int16 compositeIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::copy(instrumentUuid.begin(), instrumentUuid.end(),
             instrumentUuid_.begin());
   m_rInstruments.removeVoiceFromMelodicInstrumentSlot(instrumentUuid_, slotIdx,
                                                       compositeIdx);
}

void InstrumentsRpc::removeSlotFromMelodicInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::copy(instrumentUuid.begin(), instrumentUuid.end(),
             instrumentUuid_.begin());
   m_rInstruments.removeSlotFromMelodicInstrument(instrumentUuid_, slotIdx);
}

void InstrumentsRpc::setNoteOffsetInMelodicInstrumentVoice(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
    ::capnzero::Int16 compositeIdx, ::capnzero::Int16 noteOffset)
{
   util::Identifiable::UUID instrumentUuid_;
   std::copy(instrumentUuid.begin(), instrumentUuid.end(),
             instrumentUuid_.begin());
   m_rInstruments.setNoteOffsetInMelodicInstrumentVoice(
       instrumentUuid_, slotIdx, compositeIdx, noteOffset);
}

void InstrumentsRpc::setCompositeNameInMelodicInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
    const ::capnzero::TextView& name)
{
   util::Identifiable::UUID instrumentUuid_;
   std::copy(instrumentUuid.begin(), instrumentUuid.end(),
             instrumentUuid_.begin());
   m_rInstruments.setCompositeNameInMelodicInstrument(instrumentUuid_, slotIdx,
                                                      std::string(name));
}

void InstrumentsRpc::createKitInstrument(const ::capnzero::TextView& name)
{
   m_rInstruments.createKitInstrument(std::string(name));
}

void InstrumentsRpc::removeKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUUID)
{
   util::Identifiable::UUID instrumentUUID_;
   std::copy(instrumentUUID.begin(), instrumentUUID.end(),
             instrumentUUID_.begin());
   m_rInstruments.removeKitInstrument(instrumentUUID_);
}

void InstrumentsRpc::renameKitInstrument(const ::capnzero::SpanCL<16>& instrumentUUID,
                                             const ::capnzero::TextView& name)
{
   util::Identifiable::UUID instrumentUUID_;
   std::copy(instrumentUUID.begin(), instrumentUUID.end(),
             instrumentUUID_.begin());
   m_rInstruments.renameKitInstrument(instrumentUUID_, std::string(name));
}

void InstrumentsRpc::createNewSlotInKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid,
    const ::capnzero::SpanCL<16>& soundDeviceUuid, ::capnzero::Int16 voiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::copy(instrumentUuid.begin(), instrumentUuid.end(),
             instrumentUuid_.begin());
   util::Identifiable::UUID soundDeviceUuid_;
   std::copy(soundDeviceUuid.begin(), soundDeviceUuid.end(),
             soundDeviceUuid_.begin());
   m_rInstruments.createNewSlotInKitInstrument(instrumentUuid_,
                                               soundDeviceUuid_, voiceIdx);
}

void InstrumentsRpc::addVoiceToKitInstrumentSlot(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
    const ::capnzero::SpanCL<16>& soundDeviceUuid, ::capnzero::Int16 voiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::copy(instrumentUuid.begin(), instrumentUuid.end(),
             instrumentUuid_.begin());
   util::Identifiable::UUID soundDeviceUuid_;
   std::copy(soundDeviceUuid.begin(), soundDeviceUuid.end(),
             soundDeviceUuid_.begin());
   m_rInstruments.addVoiceToKitInstrumentSlot(instrumentUuid_, slotIdx,
                                              soundDeviceUuid_, voiceIdx);
}
void InstrumentsRpc::moveKitInstrumentSlotVoice(
    const ::capnzero::SpanCL<16>& srcInstrumentUuid,
    ::capnzero::Int16 srcSlotIdx, ::capnzero::Int16 srcCompositeIdx,
    const ::capnzero::SpanCL<16>& dstInstrumentUuid,
    ::capnzero::Int16 dstSlotIdx)
{
   util::Identifiable::UUID srcInstrumentUuid_;
   std::copy(srcInstrumentUuid.begin(), srcInstrumentUuid.end(),
             srcInstrumentUuid_.begin());
   util::Identifiable::UUID dstInstrumentUuid_;
   std::copy(dstInstrumentUuid.begin(), dstInstrumentUuid.end(),
             dstInstrumentUuid_.begin());
   m_rInstruments.moveKitInstrumentSlotVoice(srcInstrumentUuid_, srcSlotIdx,
                                             srcCompositeIdx,
                                             dstInstrumentUuid_, dstSlotIdx);
}
void InstrumentsRpc::removeVoiceFromKitInstrumentSlot(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
    ::capnzero::Int16 compositeIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::copy(instrumentUuid.begin(), instrumentUuid.end(),
             instrumentUuid_.begin());
   m_rInstruments.removeVoiceFromKitInstrumentSlot(instrumentUuid_, slotIdx,
                                                   compositeIdx);
}
void InstrumentsRpc::removeSlotFromKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::copy(instrumentUuid.begin(), instrumentUuid.end(),
             instrumentUuid_.begin());
   m_rInstruments.removeSlotFromKitInstrument(instrumentUuid_, slotIdx);
}
void InstrumentsRpc::setNoteOffsetInKitInstrumentVoice(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
    ::capnzero::Int16 compositeIdx, ::capnzero::Int16 noteOffset)
{
   util::Identifiable::UUID instrumentUuid_;
   std::copy(instrumentUuid.begin(), instrumentUuid.end(),
             instrumentUuid_.begin());
   m_rInstruments.setNoteOffsetInKitInstrumentVoice(instrumentUuid_, slotIdx,
                                                    compositeIdx, noteOffset);
}

void InstrumentsRpc::setCompositeNameInKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
    const ::capnzero::TextView& name)
{
   util::Identifiable::UUID instrumentUuid_;
   std::copy(instrumentUuid.begin(), instrumentUuid.end(),
             instrumentUuid_.begin());
   m_rInstruments.setCompositeNameInKitInstrument(instrumentUuid_, slotIdx,
                                                  std::string(name));
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
      iter->noteOn(note, velocity);
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
      iter->noteOff(note, velocity);
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
      iter->noteOn(soundIndex, note, velocity);
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
      iter->noteOff(soundIndex, note, velocity);
      return;
   }
}
