#ifndef BASE_INSTRUMENTS_PARAMETER_CACHE_H
#define BASE_INSTRUMENTS_PARAMETER_CACHE_H

#include <type_traits>
#include <vector>

#include "CallbackSignal.h"
#include "DirtyFlags.h"
#include "ParameterData.h"
#include "ValueModifier.h"

namespace base::instruments
{
struct ParameterCache
{
   explicit ParameterCache(size_t size);
   [[nodiscard]] std::size_t size() const;
   using ParameterData = base::musicDevice::sound::ParameterData;
   using ValueModifier = base::musicDevice::sound::ParameterDataCustomType<
       base::musicDevice::sound::ValueModifier>;
   using DirtyFlags = base::musicDevice::sound::DirtyFlagsVec;
   [[nodiscard]] const ParameterData& at(std::size_t pos) const;
   void setParameter(std::size_t parameterIdx,
                     musicDevice::sound::ParameterAttr parameterAttr,
                     float value);
   [[nodiscard]] float getParameter(
       std::size_t parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;
   [[nodiscard]] float getModifiedParameterValue(
       std::size_t parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;

   void clearModifier(std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr);
   void applyModifier(std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr,
                      float destination, float intensity);
   void setParameterBackup(std::size_t parameterIdx,
                           musicDevice::sound::ParameterAttr parameterAttr,
                           float value);

   void updateParameterUI();
   void syncBackupToRt();

   void dontOverwriteOnNextNoteOn(
       std::size_t parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr);
   [[nodiscard]] bool shouldBeOverwritten(
       std::size_t parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;
   void clearOverwriteList();

   CB_SIGNAL_SINGLE_SUBSCRIBER(DataChangedUI, int,
                               musicDevice::sound::ParameterAttr, float);
   friend auto meta::registerMembers<ParameterCache>();

private:
   std::vector<ParameterData> data_;
   std::vector<ValueModifier> valueModifier_;
   std::vector<ParameterData> nonRtBackupData_;
   DirtyFlags dirtyFlags_;
   DirtyFlags dontOverwriteOnNextNoteOn_;
};

static_assert(std::is_move_constructible_v<ParameterCache>,
              "ParameterCache Object must be movable");
static_assert(std::is_move_constructible_v<ParameterCache::DirtyFlags>,
              "ParameterCache::DirtyFlags Object must be movable");

}   // namespace base::instruments

#include "InstrumentsParameterCache.inl"
#include "InstrumentsParameterCacheMeta.h"

#endif