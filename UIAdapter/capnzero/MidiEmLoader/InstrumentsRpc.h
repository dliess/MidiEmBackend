#ifndef INSTRUMENTS_RPC_H
#define INSTRUMENTS_RPC_H

#include "MidiEmLoaderInstrumentsRpcIf.h"

namespace base::instruments
{
struct Instruments;
}

namespace uiadapter::capnzero
{
class InstrumentsRpc : public ::capnzero::MidiEmLoader::InstrumentsRpcIf
{
public:
   explicit InstrumentsRpc(base::instruments::Instruments& rInstruments);

   void createMelodicInstrument(const ::capnzero::TextView& name) override;
   void removeMelodicInstrument(const ::capnzero::SpanCL<16>& uuid) override;
   void renameMelodicInstrument(const ::capnzero::SpanCL<16>& instrumentUUID,
                                const ::capnzero::TextView& name) override;
   void createNewVoiceInMelodicInstrument(
       const ::capnzero::SpanCL<16>& instrumentUuid,
       const ::capnzero::SpanCL<16>& sdUuid,
       ::capnzero::Int16 sdVoiceIdx) override;
   void addComponentToMelodicInstrumentVoice(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
       const ::capnzero::SpanCL<16>& sdUuid,
       ::capnzero::Int16 sdVoiceIdx) override;
   void removeComponentFromMelodicInstrumentVoice(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 componentIdx) override;
   void removeVoiceFromMelodicInstrument(
       const ::capnzero::SpanCL<16>& instrumentUuid,
       ::capnzero::Int16 voiceIdx) override;
   void setVoiceNameInMelodicInstrument(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
       const ::capnzero::TextView& name) override;

   void createKitInstrument(const ::capnzero::TextView& name) override;
   void removeKitInstrument(
       const ::capnzero::SpanCL<16>& instrumentUUID) override;
   void renameKitInstrument(const ::capnzero::SpanCL<16>& instrumentUUID,
                            const ::capnzero::TextView& name) override;
   void createNewVoiceInKitInstrument(
       const ::capnzero::SpanCL<16>& instrumentUuid,
       const ::capnzero::SpanCL<16>& sdUuid,
       ::capnzero::Int16 sdVoiceIdx) override;
   void addComponentToKitInstrumentVoice(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
       const ::capnzero::SpanCL<16>& sdUuid,
       ::capnzero::Int16 sdVoiceIdx) override;
   void moveKitInstrumentComponent(
       const ::capnzero::SpanCL<16>& srcInstrumentUuid,
       ::capnzero::Int16 srcVoiceIdx, ::capnzero::Int16 srcComponentIdx,
       const ::capnzero::SpanCL<16>& dstInstrumentUuid,
       ::capnzero::Int16 dstVoiceIdx) override;
   void removeComponentFromKitInstrumentVoice(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 componentIdx) override;
   void removeVoiceFromKitInstrument(
       const ::capnzero::SpanCL<16>& instrumentUuid,
       ::capnzero::Int16 voiceIdx) override;
   void setNoteOffsetInKitInstrumentComponent(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 componentIdx, ::capnzero::Int16 noteOffset) override;
   void setNoteOffsetInKitInstrumentVoice(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 noteOffset) override;
   void setNoteOffsetInMelodicInstrumentComponent(
       const ::capnzero::SpanCL<16>& instrumentUuid, 
       ::capnzero::Int16 componentIdx, ::capnzero::Int16 noteOffset) override;
   void setVoiceNameInKitInstrument(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
       const ::capnzero::TextView& name) override;

private:
   base::instruments::Instruments& m_rInstruments;
};

}   // namespace uiadapter::capnzero

#endif   // INSTRUMENTS_RPC_H
