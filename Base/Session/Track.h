#ifndef BASE_SESSION_TRACK_H
#define BASE_SESSION_TRACK_H

#include "Instrument.h"
#include "Clip.h"
#include "Identifiable.h"
#include <vector>
#include <optional>
#include <memory>
#include "Memory.h"
#include <string_view>
#include <memory_resource>
#include <cstddef> // std::byte
namespace base::instruments {
    class Instrument;
}

namespace base::session
{

struct Track : public util::Identifiable
{
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;
    explicit Track(std::string_view name, const allocator_type& alloc = {}) noexcept;
    Track duplicate(const allocator_type& alloc) const noexcept;
    Track(Track&& rhs, const allocator_type& alloc) noexcept;
    void update();
    void toggleMute() noexcept;
    inline void createClip(int row);
    inline void createClip(int row, const Clip& clip);
    inline void deleteClip(int row);
    std::pmr::string name;
    instruments::Instrument* instrument{nullptr};
    std::optional<int> activeClip;
    static constexpr size_t NumClips = 64;
    std::pmr::vector<util::pmr::unique_ptr<Clip>> clips;
private:   
    Track(const Track& rhs, const allocator_type& alloc);
};

}   // namespace session

#include "Track.inl"

#endif