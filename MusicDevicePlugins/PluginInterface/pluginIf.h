#ifndef MUSIC_DEVICE_PLUGIN_IF_H
#define MUSIC_DEVICE_PLUGIN_IF_H

#include <optional>
#include <vector>

#include "MidiMessage.h"

namespace base::musicDevice::description::controller { struct Widget; }
namespace base::musicDevice::controller{
   struct WidgetCoord;
   struct ColorARGB;
}

extern "C"
{
   std::optional<std::vector<midi::MidiMessage>> createEnlightLedMidiMsg(
      const base::musicDevice::description::controller::Widget& widgetDescr,
      const base::musicDevice::controller::WidgetCoord& widgetCoord,
      const base::musicDevice::controller::ColorARGB& colorARGB);
}

#endif