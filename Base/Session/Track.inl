#ifndef BASE_SESSION_TRACK_INL
#define BASE_SESSION_TRACK_INL

namespace base
{

inline
void session::Track::createClip(int row)
{
    if(!clips[row])
    {
        clips[row] = util::pmr::make_unique<Clip>(clips.get_allocator());
    }
}

inline
void session::Track::createClip(int row, const session::Clip& clip)
{
   if (!clips[row])
   {
      clips[row] = util::pmr::make_unique(clip, clips.get_allocator());
   }
}

inline 
void session::Track::deleteClip(int row)
{
   if (clips[row])
   {
      clips[row].reset();
   }
}

}   // namespace base::session

#endif