#ifndef CONTROLLER_MIDI_OUT_MSG_HANDLER_H
#define CONTROLLER_MIDI_OUT_MSG_HANDLER_H

namespace base::musicDevice
{
namespace description::controller { struct Section; }
namespace controller
{
struct WidgetCoord;
struct ColorARGB;

template<typename MidiOutIfPtr, typename PluginIf>
class MidiOutMsgHandler
{
public:
   MidiOutMsgHandler(
      MidiOutIfPtr rMidiOutIf,
      PluginIf& rPluginHandler,
      const description::controller::Section& rControllerSection) noexcept;
   void enlightLed(int widgetId, const WidgetCoord& widgetCoord,
                   const ColorARGB& color) noexcept;
   void setValueFeedback(int widgetId, const WidgetCoord& widgetCoord,
                         float value) noexcept;

private:
   MidiOutIfPtr m_pMidiOutIf;
   PluginIf&  m_rPluginHandler;
   const description::controller::Section& m_rControllerSection;
};

} // namespace controller
} // namespace base::musicDevice

#include "ControllerMidiOutMsgHandler.tcc"

#endif