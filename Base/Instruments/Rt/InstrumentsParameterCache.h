#ifndef BASE_INSTRUMENTS_PARAMETER_CACHE_RT_H
#define BASE_INSTRUMENTS_PARAMETER_CACHE_RT_H

#include <type_traits>
#include <vector>

#include "CallbackSignal.h"
#include "DirtyFlags.h"
#include "ParameterData.h"
#include "ValueModifier.h"
#include "ErrorHandling.h"

namespace base::instruments::rt
{
struct ParameterCache
{
   explicit ParameterCache(size_t size);
   [[nodiscard]] std::size_t size() const;
   using ParameterData = base::musicDevice::sound::ParameterData;
   using ValueModifier = base::musicDevice::sound::ParameterDataCustomType<
       base::musicDevice::sound::ValueModifier>;
   using DirtyFlags = base::musicDevice::sound::DirtyFlagsVec;
   [[nodiscard]] Ret<const ParameterData*> at(std::size_t pos) const;
   Void setParameter(std::size_t parameterIdx,
                     musicDevice::sound::ParameterAttr parameterAttr,
                     float value);
   [[nodiscard]] Ret<float> getParameter(
       std::size_t parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;
   [[nodiscard]] Ret<float> getModifiedParameterValue(
       std::size_t parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;

   Void clearModifier(std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr);
   Void applyModifier(std::size_t parameterIdx,
                      musicDevice::sound::ParameterAttr parameterAttr,
                      float destination, float intensity);

   void updateParameterUI();

   void dontOverwriteOnNextNoteOn(
       std::size_t parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr);
   [[nodiscard]] bool shouldBeOverwritten(
       std::size_t parameterIdx,
       musicDevice::sound::ParameterAttr parameterAttr) const;
   void clearOverwriteList() const;

   CB_SIGNAL_SINGLE_SUBSCRIBER(DataChangedUI, int,
                               musicDevice::sound::ParameterAttr, float);
   friend auto meta::registerMembers<ParameterCache>();

private:
   std::vector<ParameterData> data_;
   std::vector<ValueModifier> valueModifier_;
   mutable DirtyFlags dirtyFlags_;
   mutable DirtyFlags dontOverwriteOnNextNoteOn_;
};

static_assert(std::is_move_constructible_v<ParameterCache>,
              "ParameterCache Object must be movable");
static_assert(std::is_move_constructible_v<ParameterCache::DirtyFlags>,
              "ParameterCache::DirtyFlags Object must be movable");

}   // namespace base::instruments::rt

#include "InstrumentsParameterCache.inl"

#endif
