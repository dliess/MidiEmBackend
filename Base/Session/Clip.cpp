#include "Clip.h"

using namespace base;

session::Clip::Clip(const allocator_type& alloc) noexcept:
    name(alloc),
    noteEvents(alloc),
    parameterEvents(alloc)
{}

session::Clip::Clip(Clip&& other, const allocator_type& alloc) noexcept:
    name(std::move(other.name), alloc),
    noteEvents(std::move(other.noteEvents), alloc),
    parameterEvents(std::move(other.parameterEvents), alloc)
{}

session::Clip::Clip(const Clip& other, const allocator_type& alloc):
    name(other.name, alloc),
    noteEvents(other.noteEvents, alloc),
    parameterEvents(other.parameterEvents, alloc)
{
}
