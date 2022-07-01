#ifndef CONTROLLER_MIDI_IN_MSG_HANDLER_H
#define CONTROLLER_MIDI_IN_MSG_HANDLER_H

#include <vector>
#include <unordered_map>
#include <functional>
#include "MidiMessageIds.h"
#include "MidiMessageIdsExtension.h"
#include "ControllerEvents.h"

namespace base::musicDevice
{
namespace description::controller { struct Section; }
namespace controller
{
template<typename MidiInIfPtr>
class MidiInMsgHandler
{
public:
   using Cb = std::function<void(const Event& event)>;
   MidiInMsgHandler(MidiInIfPtr pMidiInIf,
                    const description::controller::Section& rControllerSection,
                    Cb cb) noexcept;

   void enableNativeNoteMode(bool enable) noexcept;

private:
   MidiInIfPtr m_pMidiInIf;
   const description::controller::Section& m_rControllerSection;
   std::unordered_map<midi::MidiMessageId, EventId> m_map;
   mutable std::array<EventId::Coord, midi::NUM_CHANNELS>
      m_mpeMap;
   Cb m_drainCb;
   bool m_nativeNoteMode{true};

   static std::string cache2Str(
      const std::unordered_map<midi::MidiMessageId, EventId>& map);
   void initCache() noexcept;
   void handleEventSource(const std::vector<std::vector<midi::MidiMessageId>>& source,
                          int widgetId, int eventId) noexcept;
   void handleRouting(EventId id, const midi::MidiMessage& midiMsg) const noexcept;
};

} // namespace controller
} // namespace base::musicDevice

#include "ControllerMidiInMsgHandler.tcc"

#endif