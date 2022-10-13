#include "Track.h"

using namespace base;

session::Track::Track(std::string_view name,
                      const allocator_type& alloc) noexcept :
    name(name, alloc), clips(NumClips, alloc)
{
}

session::Track::Track(const Track& rhs, const allocator_type& alloc) :
    name(rhs.name, alloc)
{
    clips.resize(rhs.clips.size());
    for(int i = 0; i < rhs.clips.size(); ++i)
    {
        if(rhs.clips[i])
        {
            Clip clip = *rhs.clips[i];
            clips[i] = util::pmr::make_unique(clip, alloc);
        }
    }
}

session::Track::Track(Track&& rhs, const allocator_type& alloc) noexcept :
    name(std::move(rhs.name), alloc), clips(std::move(rhs.clips), alloc)
{
}

session::Track session::Track::duplicate(
    const allocator_type& alloc) const noexcept
{
   return Track(*this, alloc);
}

void session::Track::update()
{
   if (activeClip)
   {
      auto& clip = clips[activeClip.value()];
      if (clip)
      {
          clip->update(instrument);
      }
   }
}

void session::Track::toggleMute() noexcept
{
   // TODO
}