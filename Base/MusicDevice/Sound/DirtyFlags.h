#ifndef BASE_SOUND_DIRTY_FLAGS_H
#define BASE_SOUND_DIRTY_FLAGS_H

#include <bitset>
#include <tuple>

#include "ParameterAttr.h"

namespace base::musicDevice::sound
{
struct DirtyFlags
{
   static constexpr size_t MAX_NUM_PARAMS = 256;
   static constexpr size_t NUM_ATTRS =
       size_t(ParameterAttr::LfoMultiplierExp) + 1;
   void set(size_t parameterIdx, ParameterAttr parameterAttr)
   {
      bitset_.set(toIndex(parameterIdx, parameterAttr), true);
   }
   [[nodiscard]] bool any() const { return bitset_.any(); }
   template <typename CB> void forEach(CB&& cb)
   {
      for (int i = 0; i < bitset_.size(); ++i)
      {
         if (bitset_[i])
         {
            const auto& [paramIdx, paramAttr] = fromIndex(i);
            std::forward<CB>(cb)(paramIdx, paramAttr);
         }
      }
   }
   void reset() { bitset_.reset(); }

private:
   std::bitset<MAX_NUM_PARAMS * NUM_ATTRS> bitset_;
   static size_t toIndex(size_t parameterIdx, ParameterAttr parameterAttr)
   {
      return (parameterIdx * NUM_ATTRS) + size_t(parameterAttr);
   }
   static auto fromIndex(size_t i) -> std::pair<size_t, ParameterAttr>
   {
      return std::make_pair(i / NUM_ATTRS, ParameterAttr(i % NUM_ATTRS));
   }
};

struct DirtyFlagsVec
{
   explicit DirtyFlagsVec(size_t size) : bitset_(size * NUM_ATTRS) {}
   static constexpr size_t NUM_ATTRS =
       size_t(ParameterAttr::LfoMultiplierExp) + 1;
   void set(size_t parameterIdx, ParameterAttr parameterAttr)
   {
      bitset_.at(toIndex(parameterIdx, parameterAttr)) = true;
   }
   [[nodiscard]] bool contains(size_t parameterIdx,
                               ParameterAttr parameterAttr) const
   {
      return bitset_.at(toIndex(parameterIdx, parameterAttr)) == true;
   }
   [[nodiscard]] bool any() const
   {
      return std::ranges::any_of(bitset_, [](bool element) { return element; });
   }
   template <typename CB> void forEach(CB&& cb)
   {
      for (int i = 0; i < bitset_.size(); ++i)
      {
         if (bitset_[i])
         {
            const auto& [paramIdx, paramAttr] = fromIndex(i);
            std::forward<CB>(cb)(paramIdx, paramAttr);
         }
      }
   }
   void reset() { std::fill(bitset_.begin(), bitset_.end(), false); }

private:
   std::vector<bool> bitset_;
   static size_t toIndex(size_t parameterIdx, ParameterAttr parameterAttr)
   {
      return (parameterIdx * NUM_ATTRS) + size_t(parameterAttr);
   }
   static auto fromIndex(size_t i) -> std::pair<size_t, ParameterAttr>
   {
      return std::make_pair(i / NUM_ATTRS, ParameterAttr(i % NUM_ATTRS));
   }
};

}   // namespace base::musicDevice::sound

#endif