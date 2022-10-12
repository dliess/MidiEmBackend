#ifndef BASE_SESSION_CLIP_INL
#define BASE_SESSION_CLIP_INL

namespace base::session
{

inline
void Clip::setName(std::string_view nameV)
{
    name = nameV;
}

inline 
void Clip::addNote(sequencer::Beat beat, int note, float velocity)
{
    //noteEvents.
}


}   // namespace base::session

#endif