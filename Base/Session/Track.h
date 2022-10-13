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

class Track : public util::Identifiable
{
public:
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;
    explicit Track(std::string_view name, const allocator_type& alloc = {}) noexcept;
    Track duplicate(const allocator_type& alloc) const noexcept;
    Track(Track&& rhs, const allocator_type& alloc) noexcept;
    void update();
    inline void setName(std::string_view name);
    void toggleMute() noexcept;
    inline void createClip(int row);
    inline void createClip(int row, const Clip& clip);
    inline void deleteClip(int row);
    inline Clip* clip(int row) noexcept;
    inline const Clip* clip(int row) const noexcept;
private:
    Track(const Track& rhs, const allocator_type& alloc);
    std::pmr::string m_name;
    instruments::Instrument* m_instrument{nullptr};
    std::optional<int> m_activeClip;
    static constexpr size_t NumClips = 64;
    std::pmr::vector<util::pmr::unique_ptr<Clip>> m_clips;
};

}   // namespace session

#include "Track.inl"

#endif