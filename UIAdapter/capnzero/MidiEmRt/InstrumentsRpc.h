#ifndef INSTRUMENTS_RPC_H
#define INSTRUMENTS_RPC_H

#include "MidiEmRtInstrumentsRpcIf.h"

namespace base::instruments
{
struct Instruments;
}

namespace uiadapter::capnzero
{
class InstrumentsRpc : public ::capnzero::MidiEmRt::InstrumentsRpcIf
{
public:
   InstrumentsRpc(base::instruments::Instruments& rInstruments);

   void createMelodicInstrument(const ::capnzero::TextView& name) override;
   void removeMelodicInstrument(const ::capnzero::SpanCL<16>& uuid) override;
   void createNewSlotInMelodicInstrument(
       const ::capnzero::SpanCL<16>& instrumentUuid,
       const ::capnzero::SpanCL<16>& soundDeviceUuid,
       ::capnzero::Int16 voiceIdx) override;
   void addVoiceToMelodicInstrumentSlot(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
       const ::capnzero::SpanCL<16>& soundDeviceUuid,
       ::capnzero::Int16 voiceIdx) override;
   void removeVoiceFromMelodicInstrumentSlot(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
       ::capnzero::Int16 compositeIdx) override;
   void removeSlotFromMelodicInstrument(
       const ::capnzero::SpanCL<16>& instrumentUuid,
       ::capnzero::Int16 slotIdx) override;
   void setNoteOffsetInMelodicInstrumentVoice(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
       ::capnzero::Int16 compositeIdx, ::capnzero::Int16 noteOffset) override;
   void setCompositeNameInMelodicInstrument(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
       const ::capnzero::TextView& name) override;

   void createKitInstrument(const ::capnzero::TextView& name) override;
   void removeKitInstrument(
       const ::capnzero::SpanCL<16>& instrumentUUID) override;

   void createNewSlotInKitInstrument(
       const ::capnzero::SpanCL<16>& instrumentUuid,
       const ::capnzero::SpanCL<16>& soundDeviceUuid,
       ::capnzero::Int16 voiceIdx) override;
   void addVoiceToKitInstrumentSlot(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
       const ::capnzero::SpanCL<16>& soundDeviceUuid,
       ::capnzero::Int16 voiceIdx) override;
   void moveKitInstrumentSlotVoice(const ::capnzero::SpanCL<16>& srcInstrumentUuid,
                                   ::capnzero::Int16 srcSlotIdx,
                                   ::capnzero::Int16 srcCompositeIdx,
                                   const ::capnzero::SpanCL<16>& dstInstrumentUuid,
                                   ::capnzero::Int16 dstSlotIdx) override;
   void removeVoiceFromKitInstrumentSlot(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
       ::capnzero::Int16 compositeIdx) override;
   void removeSlotFromKitInstrument(
       const ::capnzero::SpanCL<16>& instrumentUuid,
       ::capnzero::Int16 slotIdx) override;
   void setNoteOffsetInKitInstrumentVoice(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
       ::capnzero::Int16 compositeIdx, ::capnzero::Int16 noteOffset) override;
   void setCompositeNameInKitInstrument(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 slotIdx,
       const ::capnzero::TextView& name) override;


   void melodicNoteOn(const ::capnzero::SpanCL<16>& uuid,
                      ::capnzero::Int16 note,
                      ::capnzero::Float32 velocity) override;
   void melodicNoteOff(const ::capnzero::SpanCL<16>& uuid,
                       ::capnzero::Int16 note,
                       ::capnzero::Float32 velocity) override;
   void kitNoteOn(const ::capnzero::SpanCL<16>& uuid,
                  ::capnzero::Int16 soundIndex, ::capnzero::Int16 note,
                  ::capnzero::Float32 velocity) override;
   void kitNoteOff(const ::capnzero::SpanCL<16>& uuid,
                   ::capnzero::Int16 soundIndex, ::capnzero::Int16 note,
                   ::capnzero::Float32 velocity) override;

private:
   base::instruments::Instruments& m_rInstruments;
};

}   // namespace uiadapter::capnzero

#endif   // INSTRUMENTS_RPC_H