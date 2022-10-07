#include "Track.h"

using namespace base;

session::Track::Track(std::string_view name, const allocator_type& alloc) noexcept :
    name(name, alloc),
    clips(alloc)
{}

session::Track::Track(const Track& rhs, const allocator_type& alloc) :
    name(rhs.name, alloc),
    clips(rhs.clips, alloc)
{
}

session::Track::Track(Track&& rhs, const allocator_type& alloc) noexcept :
    name(std::move(rhs.name), alloc),
    clips(std::move(rhs.clips), alloc)
{
}

session::Track session::Track::duplicate(const allocator_type& alloc) const noexcept
{
    return Track(*this, alloc);
}
