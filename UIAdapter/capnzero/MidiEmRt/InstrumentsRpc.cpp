#include "InstrumentsRpc.h"

#include "Instruments.h"

using namespace uiadapter::capnzero;
using namespace base::instruments;

InstrumentsRpc::InstrumentsRpc(base::instruments::Instruments& rInstruments) :
    m_rInstruments(rInstruments)
{
}

void InstrumentsRpc::melodicNoteOn(const ::capnzero::SpanCL<16>& uuid,
                                   ::capnzero::Int16 note,
                                   ::capnzero::Float32 velocity)
{
   util::Identifiable::UUID uuid_;
   std::copy(uuid.begin(), uuid.end(), uuid_.begin());
   auto iter = std::find_if(m_rInstruments.data.melodicInstruments.begin(), 
                            m_rInstruments.data.melodicInstruments.end(), 
                            [&uuid_](const MelodicInstrument& mi){ return uuid_ == mi.id(); });
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
   auto iter = std::find_if(m_rInstruments.data.melodicInstruments.begin(), 
                            m_rInstruments.data.melodicInstruments.end(), 
                            [&uuid_](const MelodicInstrument& mi){ return uuid_ == mi.id(); });
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
   auto iter = std::find_if(m_rInstruments.data.kitInstruments.begin(), 
                            m_rInstruments.data.kitInstruments.end(), 
                            [&uuid_](const KitInstrument& mi){ return uuid_ == mi.id(); });
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
   auto iter = std::find_if(m_rInstruments.data.kitInstruments.begin(), 
                            m_rInstruments.data.kitInstruments.end(), 
                            [&uuid_](const KitInstrument& mi){ return uuid_ == mi.id(); });
   if (iter != m_rInstruments.data.kitInstruments.end())
   {
      iter->noteOff(soundIndex, note, velocity);
      return;
   }
}
