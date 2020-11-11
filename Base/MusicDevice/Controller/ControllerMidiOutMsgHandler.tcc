#include <loguru.hpp>

#include "ControllerColor.h"
#include "ControllerEvents.h"
#include "ControllerMidiOutMsgHandler.h"
#include "ControllerSection.h"

namespace base::musicDevice
{
template<typename MidiOutIfPtr, typename PluginIf>
controller::MidiOutMsgHandler<MidiOutIfPtr, PluginIf>::MidiOutMsgHandler(
   MidiOutIfPtr rMidiOutIf, PluginIf& rPluginHandler,
   const description::controller::Section& rControllerSection) noexcept :
   m_pMidiOutIf(std::move(rMidiOutIf)),
   m_rPluginHandler(rPluginHandler), m_rControllerSection(rControllerSection)
{
}

template<typename MidiOutIfPtr, typename PluginIf>
void controller::MidiOutMsgHandler<MidiOutIfPtr, PluginIf>::enlightLed(
   int widgetId, const WidgetCoord& widgetCoord,
   const ColorARGB& color) noexcept
{
   if (widgetId >= m_rControllerSection.widgets.size())
   {
      LOG_F(ERROR,
            "Requested enlightLed() for widget id({}), but it exceeds vector "
            "range",
            widgetId);
      return;
   }
   auto midiMessages = m_rPluginHandler.createEnlightLedMidiMsg(
      m_rControllerSection.widgets[widgetId], widgetCoord, color);

   if (!midiMessages)
   {
      LOG_F(ERROR, "Plugin did not return any midi message");
      return;
   }
   for (const auto& msg : *midiMessages)
   {
      m_pMidiOutIf->send(msg);
   }
}

template<typename MidiOutIfPtr, typename PluginIf>
void controller::MidiOutMsgHandler<MidiOutIfPtr, PluginIf>::setValueFeedback(
   int widgetId, const WidgetCoord& widgetCoord, float value) noexcept
{
   // TODO
}

} // namespace base::musicDevice