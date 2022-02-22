#include "ControllerHandler.h"

#include "ControllerColor.h"
#include "ControllerEvents.h" // WidgetCoord
#include "ControllerMidiInMsgHandler.h"
#include "ControllerMidiOutMsgHandler.h"
#include "ControllerSection.h"

using namespace base::musicDevice::controller;

ControllerHandler::ControllerHandler(
   std::string deviceName, PluginHandler& rPluginHandler,
   const description::controller::Section& rControllerSection) noexcept :
   m_deviceName(std::move(deviceName)),
   m_rPluginHandler(rPluginHandler), m_rControllerSection(rControllerSection)
{
}

ControllerHandler::~ControllerHandler() = default;

ControllerHandler::ControllerHandler(ControllerHandler&& other) noexcept =
   default;

void ControllerHandler::initMidiInHandler(
   std::shared_ptr<MidiInput> pMidiIn) noexcept
{
   assert(!m_midiInMsgHandler);
   m_midiInMsgHandler = std::make_unique<MidiInMsgHandlerT>(
      pMidiIn, m_rControllerSection, [](const Event& event) {
         /*
         spdlog::info( "Received evt id {} | value: {}",
               meta::serialize(event.id).dump(),
               meta::serialize(event.value).dump());
         */
      });
}

void ControllerHandler::initMidiOutHandler(
   std::shared_ptr<MidiOutput> pMidiOut) noexcept
{
   m_midiOutHandler = std::make_unique<MidiOutMsgHandlerT>(
      pMidiOut, m_rPluginHandler, m_rControllerSection);
}

void ControllerHandler::initEvdevHandler()
{
   // TODO
}

float ControllerHandler::getParameterValue(int presetId,
                                           int parameterId) const noexcept
{
   // TODO
   return 0.0;
}

void ControllerHandler::enlightLed(int widgetId, const WidgetCoord& widgetCoord,
                                   const ColorARGB& color) noexcept
{
   // TODO
}

/*
void ControllerHandler::registerForControllerParameterChange(
   ParameterChangeCb cb) noexcept
{
   m_controllerParameterChangeCbs.emplace_back(std::move(cb));
}

void ControllerHandler::registerForNoteChange(NoteChangeCb cb) noexcept
{
   m_noteChangeCbs.emplace_back(std::move(cb));
}

void ControllerHandler::registerForPitchBendChange(PitchBendChangeCb cb)
noexcept
{
   m_pitchBendChangeCbs.emplace_back(std::move(cb));
}
*/