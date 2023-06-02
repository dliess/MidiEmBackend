#ifndef BASE_INSTRUMENTS_PARAMETER_CACHE_H
#define BASE_INSTRUMENTS_PARAMETER_CACHE_H

#include <vector>

#include "CallbackSignal.h"
#include "DirtyFlags.h"
#include "ParameterData.h"

namespace base::instruments
{
struct ParameterCache
{
   explicit ParameterCache(size_t size);
   [[nodiscard]] std::size_t size() const;
   using ParameterData = base::musicDevice::sound::ParameterData;
   using DirtyFlags    = base::musicDevice::sound::DirtyFlagsVec;
   [[nodiscard]] const ParameterData& at(std::size_t pos) const;
   void setParameter(std::size_t parameterIdx,
                     musicDevice::sound::ParameterAttr parameterAttr,
                     float value);
   void setParameterBackup(std::size_t parameterIdx,
                           musicDevice::sound::ParameterAttr parameterAttr,
                           float value);

   void updateParameterUI();
   void syncRtToNonRt();
   void syncNonRtToRt();
   void emiAllNonNullParameters();
   CB_SIGNAL_SINGLE_SUBSCRIBER(DataChangedUI, int,
                               musicDevice::sound::ParameterAttr, float);
   friend auto meta::registerMembers<ParameterCache>();

private:
   std::vector<ParameterData> data_;
   std::vector<ParameterData> nonRtBackupData_;
   DirtyFlags dirtyFlags_;
};

}   // namespace base::instruments

#include "InstrumentsParameterCache.inl"
#include "InstrumentsParameterCacheMeta.h"

#endif