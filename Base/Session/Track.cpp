#include "Track.h"
#include "BeatTick.h"

using namespace base;

session::Track::Track(std::string_view name,
                      const allocator_type& alloc) noexcept :
    m_name(name, alloc), m_clips(NumClips, alloc)
{
}

session::Track::Track(const Track& rhs, const allocator_type& alloc) :
    m_name(rhs.m_name, alloc)
{
   m_clips.resize(rhs.m_clips.size());
   for (int i = 0; i < rhs.m_clips.size(); ++i)
   {
      if (rhs.m_clips[i])
      {
         Clip clip  = *rhs.m_clips[i];
         m_clips[i] = util::pmr::make_unique(clip, alloc);
      }
   }
}

session::Track::Track(Track&& rhs, const allocator_type& alloc) noexcept :
    m_name(std::move(rhs.m_name), alloc), m_clips(std::move(rhs.m_clips), alloc)
{
}

session::Track session::Track::duplicate(
    const allocator_type& alloc) const noexcept
{
   return Track(*this, alloc);
}

void session::Track::update()
{
   if (m_toStartClipIdx)
   {
      static constexpr double Threshold = 0.1;
      const double inBeatPos = tempo::BeatTick::instance().getInBeatPos();
      if (inBeatPos < Threshold)
      {
         if (m_startedClipIdx)
         {
            m_clips[m_startedClipIdx.value()]->stop(m_instrument);
         }
         if (StopperIdx != m_toStartClipIdx.value())
         {
            m_clips[m_toStartClipIdx.value()]->start();
            m_startedClipIdx = m_toStartClipIdx;
         }
         else
         {
            m_startedClipIdx.reset();
         }
         m_toStartClipIdx.reset();
      }
   }
   if (m_startedClipIdx)
   {
      m_clips[m_startedClipIdx.value()]->update(m_instrument);
   }
}

void session::Track::toggleMute() noexcept
{
   // TODO
}