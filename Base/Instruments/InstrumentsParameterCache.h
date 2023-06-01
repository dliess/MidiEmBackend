#ifndef BASE_INSTRUMENTS_PARAMETER_CACHE_H
#define BASE_INSTRUMENTS_PARAMETER_CACHE_H

#include <vector>
#include "DirtyFlags.h"
#include "ParameterData.h"
#include "CallbackSignal.h"

namespace base::instruments 
{
struct ParameterCaches;

struct ParameterCache
{
    explicit ParameterCache(size_t size) : data_(size), dirtyFlags_(size) {}
    [[nodiscard]] std::size_t size() const { return data_.size(); }
    using ParameterData = base::musicDevice::sound::ParameterData;
    using DirtyFlags    = base::musicDevice::sound::DirtyFlagsVec;
    [[nodiscard]] const ParameterData& at(std::size_t pos) const;
    void setParameter(std::size_t index,
                    musicDevice::sound::ParameterAttr parameterAttr,
                    float value);
    void updateParameterUI();
    void emiAllNonNullParameters();
    friend struct ParameterCaches;
    CB_SIGNAL_SINGLE_SUBSCRIBER(DataChangedUI, int,
                                musicDevice::sound::ParameterAttr, float);
private:
    std::vector<ParameterData> data_;
    DirtyFlags dirtyFlags_;
};
struct ParameterCacheNonRt 
{
    explicit ParameterCacheNonRt(size_t size) : data_(size) {}
    using ParameterData = base::musicDevice::sound::ParameterData;
    friend struct ParameterCaches;
private:
    std::vector<ParameterData> data_;
};

struct ParameterCaches
{
    explicit ParameterCaches(size_t size) : rt(size), nonRt(size) {}
    void syncRtToNonRt() {std::ranges::copy(rt.data_, nonRt.data_.begin());}
    ParameterCache rt;
    ParameterCacheNonRt nonRt;
};

} // namespace base::instruments

#include "InstrumentsParameterCache.inl"

#endif