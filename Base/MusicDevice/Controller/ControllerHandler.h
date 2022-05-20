#ifndef MUSIC_DEVICE_CONTROLLER_HANDLER_H
#define MUSIC_DEVICE_CONTROLLER_HANDLER_H

#include <string>
#include <memory>
#include "Midi1Input.h"
#include "Midi1Output.h"
#include "MusicDevicePluginHandler.h"
#include "DoubleBufferedMessageDrain.h"
#include "CallbackSignal.h"
#include "ControllerEvents.h"

namespace base::musicDevice
{
namespace description::controller { struct Section; }
namespace controller
{

template<typename T> class MidiInMsgHandler;
template<typename T, typename V> class MidiOutMsgHandler;

struct WidgetCoord;
struct ColorARGB;
class ControllerHandler
{
public:
   ControllerHandler(std::string deviceName,
                     PluginHandler& rPluginHandler,
                     const description::controller::Section& rControllerSection) noexcept;
   ~ControllerHandler();
   ControllerHandler(const ControllerHandler& other) = delete;
   ControllerHandler& operator=(const ControllerHandler& other) = delete;
   ControllerHandler(ControllerHandler&& other) noexcept;

   using MidiInput = midi::Midi1Input<midi::DoubleBufferedMessageDrain>;
   using MidiOutput = midi::Midi1Output;
   void initMidiInHandler(std::shared_ptr<MidiInput> pMidiIn) noexcept;
   void initMidiOutHandler(std::shared_ptr<MidiOutput> pMidiOut) noexcept;
   void initEvdevHandler();

   float getParameterValue(int presetId, int parameterId) const noexcept;
   void enlightLed(int widgetId, const WidgetCoord& widgetCoord,
                   const ColorARGB& color) noexcept;

   CB_SIGNAL(EventReceived, const Event&);

private:
   std::string m_deviceName;
   PluginHandler& m_rPluginHandler;
   const description::controller::Section& m_rControllerSection;
   using MidiInMsgHandlerT = MidiInMsgHandler<std::shared_ptr<MidiInput>>;
   using MidiOutMsgHandlerT = 
      MidiOutMsgHandler<std::shared_ptr<MidiOutput>, PluginHandler>;
   std::unique_ptr<MidiInMsgHandlerT>     m_midiInMsgHandler;
   std::unique_ptr<MidiOutMsgHandlerT>    m_midiOutHandler;
};

} // namespace controller
} // namespace base::musicDevice

#endif