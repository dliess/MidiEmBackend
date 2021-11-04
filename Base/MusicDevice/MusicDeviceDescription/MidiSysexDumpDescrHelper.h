#ifndef MIDI_SYSEX_DUMP_DESCR_HELPER_H
#define MIDI_SYSEX_DUMP_DESCR_HELPER_H

#include "SoundSection.h"

namespace base::musicDevice::description::sound::midisysex
{
class FiledDescrHelper
{
public:
   FiledDescrHelper(const std::vector<uint8_t>& rSysexMsg, 
       std::vector<midisysex::FieldDescr>& rFieldDescriptors) noexcept :
       m_rSysexMsg(rSysexMsg),
       m_rFieldDescriptors(rFieldDescriptors)
   {
   }
   template <class Calback> void forEachParameter(Callback&& cb)
   {
      std::optional<int> voiceIdx;
      for (const auto& fieldDescr : m_rFieldDescriptors)
      {

         mpark::visit(
             util::overload{
                 [&voiceIdx](const VoiceIdx& vIdxDescr){ voiceIdx = m_rSysexMsg[vIdxDescr.offset]; },
                 [this, &voiceIdx](ParameterLowRes& param) {
                     assert(voiceIdx);
                     cb(*voiceIdx, param);
                 },
                 [](auto&&) {}},
             fieldDescr);
      }
   }

private:
   const std::vector<uint8_t>& m_rSysexMsg;
   std::vector<midisysex::FieldDescr>& m_rFieldDescriptors;
};

}   // namespace base::musicDevice::description::sound::midisysex
#endif