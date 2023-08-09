#ifndef BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_HASH_H
#define BASE_MUSIC_DEVICE_CONTROLLER_EVENT_DESTINATION_HASH_H

#include <Variant.h>

namespace std
{

template <> struct hash<base::eventRouter::EventDestination::Endpoint>
{
   std::size_t operator()(const base::eventRouter::EventDestination::Endpoint& endpoint) const
   {
      return dl::visit(
         dl::overload{
            [](const base::eventRouter::EventDestination::DrumKit drumKit) -> std::size_t {
               return util::IdentifiableHash{}(drumKit.uuid) ^
                      std::hash<int>{}(drumKit.voiceIdx) << 16 ^
                      std::hash<int>{}(drumKit.componentIdx) << 24;
            },
            [](const base::eventRouter::EventDestination::Melodic melodic) -> std::size_t {
               return util::IdentifiableHash{}(melodic.uuid) ^
                      std::hash<int>{}(melodic.componentIdx) << 16;
            },
            [](const base::eventRouter::EventDestination::MusicDevice musicDevice) -> std::size_t {
               return std::hash<std::string>{}(musicDevice.mdid.toStr()) ^
                      std::hash<int>{}(musicDevice.voiceIdx) << 16;
            }
         }, endpoint
      );
   }
};

template <> struct hash<base::eventRouter::EventDestination::ControlType>
{
   std::size_t operator()(const base::eventRouter::EventDestination::ControlType& controllerType) const
   {
      return dl::visit(
         dl::overload{
            [](const base::eventRouter::EventDestination::Note& note) -> std::size_t {
               return std::hash<int>{}(note.pitch);
            },
            [](const base::eventRouter::EventDestination::Parameter& parameter) -> std::size_t {
               return std::hash<int>{}(parameter.id) ^
                      std::hash<int>{}(static_cast<int>(parameter.parameterAttr)) << 16;
            }
         }, controllerType
      );
   }
};

template <> struct hash<base::eventRouter::EventDestination>
{
   std::size_t operator()(const base::eventRouter::EventDestination& evtDest) const
   {
      return std::hash<base::eventRouter::EventDestination::Endpoint>{}(
                 evtDest.endpoint) ^
             std::hash<base::eventRouter::EventDestination::ControlType>{}(
                 evtDest.controlType) << 16;
   }
};

} // namespace std

#endif
