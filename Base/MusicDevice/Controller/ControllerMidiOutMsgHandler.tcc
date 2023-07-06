#include <spdlog/spdlog.h>

#include "ControllerColor.h"
#include "ControllerEvents.h"
#include "ControllerMidiOutMsgHandler.h"
#include "ControllerSection.h"

namespace base::musicDevice
{
template <typename MidiOutIfPtr, typename PluginIf>
controller::MidiOutMsgHandler<MidiOutIfPtr, PluginIf>::MidiOutMsgHandler(
    MidiOutIfPtr rMidiOutIf, PluginIf& rPluginHandler,
    const description::controller::Section& rControllerSection) noexcept :
    m_pMidiOutIf(std::move(rMidiOutIf)),
    m_rPluginHandler(rPluginHandler),
    m_rControllerSection(rControllerSection)
{
   if (m_rControllerSection.initMsg)
   {
      for (const auto& sysexMsgDescr :
           m_rControllerSection.initMsg->sysexDescriptors)
      {
         std::vector<uint8_t> sysexMsgToSend;
         sysexMsgToSend.insert(sysexMsgToSend.end(),
                               sysexMsgDescr.values.begin(),
                               sysexMsgDescr.values.end());
         m_pMidiOutIf->sysEx(sysexMsgToSend);
      }
      spdlog::info("initMsg sent");
   }
}

template <typename MidiOutIfPtr, typename PluginIf>
void controller::MidiOutMsgHandler<MidiOutIfPtr, PluginIf>::enlightLed(
    int widgetId, const WidgetCoord& widgetCoord,
    const ColorARGB& color) noexcept
{
   if (widgetId >= m_rControllerSection.widgets.size())
   {
      spdlog::error(
          "Requested enlightLed() for widget id({}), but it exceeds vector "
          "range",
          widgetId);
      return;
   }
   auto midiMessages = m_rPluginHandler.createEnlightLedMidiMsg(
       m_rControllerSection.widgets[widgetId], widgetCoord, color);

   if (!midiMessages)
   {
      spdlog::error("Plugin did not return any midi message");
      return;
   }
   for (const auto& msg : *midiMessages) { m_pMidiOutIf->send(msg); }
}

template <typename MidiOutIfPtr, typename PluginIf>
void controller::MidiOutMsgHandler<MidiOutIfPtr, PluginIf>::setValueFeedback(
    int widgetId, const WidgetCoord& widgetCoord, float value) noexcept
{
   // TODO
}

}   // namespace base::musicDevice