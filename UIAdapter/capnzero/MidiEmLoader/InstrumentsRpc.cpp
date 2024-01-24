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

void InstrumentsRpc::removeMelodicInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   try
   {
      m_rInstruments.removeMelodicInstrument(instrumentUuid_);
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}
void InstrumentsRpc::renameMelodicInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid,
    const ::capnzero::TextView& name)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   try
   {
      m_rInstruments.renameMelodicInstrument(instrumentUuid_,
                                             std::string(name));
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}

void InstrumentsRpc::createNewVoiceInMelodicInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid,
    const ::capnzero::SpanCL<16>& sdUuid, ::capnzero::Int16 sdVoiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   util::Identifiable::UUID sdUuid_;
   std::ranges::copy(sdUuid, sdUuid_.begin());
   try
   {
      m_rInstruments.createNewVoiceInMelodicInstrument(instrumentUuid_, sdUuid_,
                                                       sdVoiceIdx);
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}

void InstrumentsRpc::addComponentToMelodicInstrumentVoice(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    const ::capnzero::SpanCL<16>& sdUuid, ::capnzero::Int16 sdVoiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   util::Identifiable::UUID sdUuid_;
   std::ranges::copy(sdUuid, sdUuid_.begin());
   try
   {
      m_rInstruments.addComponentToMelodicInstrumentVoice(
          instrumentUuid_, voiceIdx, sdUuid_, sdVoiceIdx);
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}

void InstrumentsRpc::removeComponentFromMelodicInstrumentVoice(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   try
   {
      m_rInstruments.removeComponentFromMelodicInstrumentVoice(
          instrumentUuid_, voiceIdx, componentIdx);
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}

void InstrumentsRpc::removeVoiceFromMelodicInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   try
   {
      m_rInstruments.removeVoiceFromMelodicInstrument(instrumentUuid_,
                                                      voiceIdx);
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}

void InstrumentsRpc::setNoteOffsetInKitInstrumentVoice(
       const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 noteOffset)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   try
   {
       m_rInstruments.setNoteOffsetInKitInstrumentVoice(instrumentUuid_, voiceIdx, noteOffset);
   }
   catch (std::exception& e)
   {
       spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}

void InstrumentsRpc::setNoteOffsetInMelodicInstrumentComponent(
    const ::capnzero::SpanCL<16>& instrumentUuid, 
    ::capnzero::Int16 componentIdx, ::capnzero::Int16 noteOffset)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   try
   {
      m_rInstruments.setNoteOffsetInMelodicInstrumentComponent(
          instrumentUuid_, componentIdx, noteOffset);
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}

void InstrumentsRpc::setVoiceNameInMelodicInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    const ::capnzero::TextView& name)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   try
   {
      m_rInstruments.setVoiceNameInMelodicInstrument(instrumentUuid_, voiceIdx,
                                                     std::string(name));
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}

void InstrumentsRpc::createKitInstrument(const ::capnzero::TextView& name)
{
   try
   {
      m_rInstruments.createKitInstrument(std::string(name));
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}

void InstrumentsRpc::removeKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   try
   {
      m_rInstruments.removeKitInstrument(instrumentUuid_);
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}

void InstrumentsRpc::renameKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid,
    const ::capnzero::TextView& name)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   try
   {
      m_rInstruments.renameKitInstrument(instrumentUuid_, std::string(name));
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}

void InstrumentsRpc::createNewVoiceInKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid,
    const ::capnzero::SpanCL<16>& sdUuid, ::capnzero::Int16 sdVoiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   util::Identifiable::UUID sdUuid_;
   std::ranges::copy(sdUuid, sdUuid_.begin());
   try
   {
      m_rInstruments.createNewVoiceInKitInstrument(instrumentUuid_, sdUuid_,
                                                   sdVoiceIdx);
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}

void InstrumentsRpc::addComponentToKitInstrumentVoice(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    const ::capnzero::SpanCL<16>& sdUuid, ::capnzero::Int16 sdVoiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   util::Identifiable::UUID sdUuid_;
   std::ranges::copy(sdUuid, sdUuid_.begin());
   try
   {
      m_rInstruments.addComponentToKitInstrumentVoice(instrumentUuid_, voiceIdx,
                                                      sdUuid_, sdVoiceIdx);
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}
void InstrumentsRpc::moveKitInstrumentComponent(
    const ::capnzero::SpanCL<16>& srcInstrumentUuid,
    ::capnzero::Int16 srcVoiceIdx, ::capnzero::Int16 srcComponentIdx,
    const ::capnzero::SpanCL<16>& dstInstrumentUuid,
    ::capnzero::Int16 dstVoiceIdx)
{
   util::Identifiable::UUID srcInstrumentUuid_;
   std::ranges::copy(srcInstrumentUuid, srcInstrumentUuid_.begin());
   util::Identifiable::UUID dstInstrumentUuid_;
   std::ranges::copy(dstInstrumentUuid, dstInstrumentUuid_.begin());
   try
   {
      m_rInstruments.moveKitInstrumentComponent(
          srcInstrumentUuid_, srcVoiceIdx, srcComponentIdx, dstInstrumentUuid_,
          dstVoiceIdx);
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}
void InstrumentsRpc::removeComponentFromKitInstrumentVoice(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   try
   {
      m_rInstruments.removeComponentFromKitInstrumentVoice(
          instrumentUuid_, voiceIdx, componentIdx);
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}
void InstrumentsRpc::removeVoiceFromKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   try
   {
      m_rInstruments.removeVoiceFromKitInstrument(instrumentUuid_, voiceIdx);
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}
void InstrumentsRpc::setNoteOffsetInKitInstrumentComponent(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    ::capnzero::Int16 componentIdx, ::capnzero::Int16 noteOffset)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   try
   {
      m_rInstruments.setNoteOffsetInKitInstrumentComponent(
          instrumentUuid_, voiceIdx, componentIdx, noteOffset);
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}

void InstrumentsRpc::setVoiceNameInKitInstrument(
    const ::capnzero::SpanCL<16>& instrumentUuid, ::capnzero::Int16 voiceIdx,
    const ::capnzero::TextView& name)
{
   util::Identifiable::UUID instrumentUuid_;
   std::ranges::copy(instrumentUuid, instrumentUuid_.begin());
   try
   {
      m_rInstruments.setVoiceNameInKitInstrument(instrumentUuid_, voiceIdx,
                                                 std::string(name));
   }
   catch (std::exception& e)
   {
      spdlog::error("InstrumentsRpc exception occured: {}", e.what());
   }
}
