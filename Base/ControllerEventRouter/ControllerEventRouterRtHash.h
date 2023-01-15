#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_RT_HASH_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_ROUTER_RT_HASH_H

namespace std
{
template <> struct hash<base::eventRouter::ParameterCacheKey>
{
   std::size_t operator()(const base::eventRouter::ParameterCacheKey& key) const
   {
      return std::hash<base::musicDevice::controller::EventIdExt>{}(
                 key.eventId) ^
             std::hash<base::eventRouter::EventDestination>{}(
                 key.eventDestination);
   }
};

}   // namespace std

namespace base::eventRouter
{
inline bool operator==(const ParameterCacheKey& lhs,
                       const ParameterCacheKey& rhs)
{
   return lhs.eventDestination == rhs.eventDestination &&
          lhs.eventId == rhs.eventId;
}

}   // namespace base::eventRouter

#endif