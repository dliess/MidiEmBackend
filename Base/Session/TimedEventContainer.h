#ifndef BASE_SESSION_TIMED_EVENT_CONTAINER_H
#define BASE_SESSION_TIMED_EVENT_CONTAINER_H

#include "Clip.h"

namespace base::session
{

template <typename Container>
struct TimedEventContainer : public Container
{
   template <typename Cb>
   void forNoteEvents(sequencer::Beat fromBeat, sequencer::Beat toBeat, Cb&& cb)
   {
      // [start, end[
      const auto itStart =
         Container::lower_bound(typename Container::value_type{fromBeat});
      const auto itEnd =
         Container::upper_bound(typename Container::value_type{toBeat});
      if (fromBeat > toBeat)   // wrapped
      {
         for (auto it = itStart; it != Container::end(); ++it) 
         {
            cb(*it);
         }
         for (auto it = Container::begin(); it != itEnd; ++it) 
         {
            cb(*it);
         }
      }
      else
      {
         for (auto it = itStart; it != itEnd; ++it) 
         {
            cb(*it);
         }
      }
   }
};

}   // namespace base::session

#endif