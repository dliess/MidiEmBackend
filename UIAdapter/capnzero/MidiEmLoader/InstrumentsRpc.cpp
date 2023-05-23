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

void InstrumentsRpc::removeMelodicInstrument(const ::capnzero::SpanCL<16>& instrumentUuid)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   m_rInstruments.removeMelodicInstrument(instrumentUuid_);
}
void InstrumentsRpc::renameMelodicInstrument(const ::capnzero::SpanCL<16>& instrumentUuid,
                                             const ::capnzero::TextView& name)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   m_rInstruments.renameMelodicInstrument(instrumentUuid_, std::string(name));
}

void InstrumentsRpc::createNewSlotInMelodicInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid,
    const ::capnzero::SpanCL<16>& sdUuid, ::capnzero::Int16 sdVoiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   util::Identifiable::UUID sdUuid_;
   std::ranges::copy(sdUuid, sdUuid_.begin());
   m_rInstruments.createNewVoiceInMelodicInstrument(instrumentUuid_,
                                                   sdUuid_, sdVoiceIdx);
}

void InstrumentsRpc::addVoiceToMelodicInstrumentSlot(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    const ::capnzero::SpanCL<16>& sdUuid, ::capnzero::Int16 sdVoiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   util::Identifiable::UUID sdUuid_;
   std::ranges::copy(sdUuid, sdUuid_.begin());
   m_rInstruments.addComponentToMelodicInstrumentVoice(instrumentUuid_, voiceIdx,
                                                       sdUuid_, sdVoiceIdx);
}

void InstrumentsRpc::removeVoiceFromMelodicInstrumentSlot(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   m_rInstruments.removeComponentFromMelodicInstrumentVoice(instrumentUuid_, voiceIdx,
                                                       componentIdx);
}

void InstrumentsRpc::removeSlotFromMelodicInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   m_rInstruments.removeVoiceFromMelodicInstrument(instrumentUuid_, voiceIdx);
}

void InstrumentsRpc::setNoteOffsetInMelodicInstrumentVoice(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx, ::capnzero::Int16 noteOffset)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   m_rInstruments.setNoteOffsetInMelodicInstrumentComponent(
       instrumentUuid_, voiceIdx, componentIdx, noteOffset);
}

void InstrumentsRpc::setCompositeNameInMelodicInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    const ::capnzero::TextView& name)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   m_rInstruments.setVoiceNameInMelodicInstrument(instrumentUuid_, voiceIdx,
                                                      std::string(name));
}

void InstrumentsRpc::createKitInstrument(const ::capnzero::TextView& name)
{
   m_rInstruments.createKitInstrument(std::string(name));
}

void InstrumentsRpc::removeKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   m_rInstruments.removeKitInstrument(instrumentUuid_);
}

void InstrumentsRpc::renameKitInstrument(const ::capnzero::SpanCL<16>& instrumentUuid,
                                             const ::capnzero::TextView& name)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   m_rInstruments.renameKitInstrument(instrumentUuid_, std::string(name));
}

void InstrumentsRpc::createNewSlotInKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid,
    const ::capnzero::SpanCL<16>& sdUuid, ::capnzero::Int16 sdVoiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   util::Identifiable::UUID sdUuid_;
   std::ranges::copy(sdUuid, sdUuid_.begin());
   m_rInstruments.createNewVoiceInKitInstrument(instrumentUuid_,
                                               sdUuid_, sdVoiceIdx);
}

void InstrumentsRpc::addVoiceToKitInstrumentSlot(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    const ::capnzero::SpanCL<16>& sdUuid, ::capnzero::Int16 sdVoiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   util::Identifiable::UUID sdUuid_;
   std::ranges::copy(sdUuid, sdUuid_.begin());
   m_rInstruments.addComponentToKitInstrumentVoice(instrumentUuid_, voiceIdx,
                                              sdUuid_, sdVoiceIdx);
}
void InstrumentsRpc::moveKitInstrumentSlotVoice(
    const ::capnzero::SpanCL<16>& srcInstrumentUuid,
    ::capnzero::Int16 srcVoiceIdx, ::capnzero::Int16 srcCompositeIdx,
    const ::capnzero::SpanCL<16>& dstInstrumentUuid,
    ::capnzero::Int16 dstVoiceIdx)
{
   util::Identifiable::UUID srcInstrumentUuid_;
   std::ranges::copy(srcInstrumentUuid, srcInstrumentUuid_.begin());
   util::Identifiable::UUID dstInstrumentUuid_;
   std::ranges::copy(dstInstrumentUuid, dstInstrumentUuid_.begin());
   m_rInstruments.moveKitInstrumentComponent(srcInstrumentUuid_, srcVoiceIdx,
                                             srcCompositeIdx,
                                             dstInstrumentUuid_, dstVoiceIdx);
}
void InstrumentsRpc::removeVoiceFromKitInstrumentSlot(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   m_rInstruments.removeComponentFromKitInstrumentVoice(instrumentUuid_, voiceIdx,
                                                   componentIdx);
}
void InstrumentsRpc::removeSlotFromKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   m_rInstruments.removeVoiceFromKitInstrument(instrumentUuid_, voiceIdx);
}
void InstrumentsRpc::setNoteOffsetInKitInstrumentVoice(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx, ::capnzero::Int16 noteOffset)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   m_rInstruments.setNoteOffsetInKitInstrumentComponent(instrumentUuid_, voiceIdx,
                                                    componentIdx, noteOffset);
}

void InstrumentsRpc::setCompositeNameInKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    const ::capnzero::TextView& name)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   m_rInstruments.setVoiceNameInKitInstrument(instrumentUuid_, voiceIdx,
                                                  std::string(name));
}