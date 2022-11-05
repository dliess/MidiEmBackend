#ifndef BASE_SESSION_TRACK_H
#define BASE_SESSION_TRACK_H

#include "Instrument.h"
#include "Clip.h"
#include "Identifiable.h"
#include "CallbackSignal.h"

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
    void resetActiveClip();
    void stop();
    void update();
    inline void setName(std::string_view name);
    void toggleMute() noexcept;
    inline void createClip(int row);
    inline void createClip(int row, const Clip& clip);
    inline void deleteClip(int row);
    inline void startClip(int row);
    inline void stopClip();
    inline Clip* clip(int row) noexcept;
    inline const Clip* clip(int row) const noexcept;
    inline std::string_view name() const;

    CB_SIGNAL_SINGLE_SUBSCRIBER(NameChanged, std::string_view);
    CB_SIGNAL_SINGLE_SUBSCRIBER(ClipCreated, int);
    CB_SIGNAL_SINGLE_SUBSCRIBER(ClipDeleted, int);
    CB_SIGNAL_SINGLE_SUBSCRIBER(ClipStartedChanged, int, bool);

    CB_SIGNAL_SINGLE_SUBSCRIBER(ClipNameChanged, int, std::string_view);
    CB_SIGNAL_SINGLE_SUBSCRIBER(ClipNoteAdded, int, sequencer::NoteId, sequencer::Beat, sequencer::Beat, int, float);
    CB_SIGNAL_SINGLE_SUBSCRIBER(ClipNoteVelocityChanged, int, sequencer::NoteId, float);
    CB_SIGNAL_SINGLE_SUBSCRIBER(ClipNoteRemoved, int, sequencer::NoteId);
    CB_SIGNAL_SINGLE_SUBSCRIBER(ClipAllNotesRemoved, int);

private:
    Track(const Track& rhs, const allocator_type& alloc);
    std::pmr::string m_name;
    static constexpr size_t NumClips = 64;
    std::pmr::vector<util::pmr::unique_ptr<Clip>> m_clips;
    instruments::Instrument* m_instrument{nullptr};
    static constexpr int StopperIdx = -1;
    std::optional<int> m_activeClipIdx;
    std::optional<int> m_toStartClipIdx;
    void registerCbs(int row);
};

}   // namespace session

#include "Track.inl"

#endif