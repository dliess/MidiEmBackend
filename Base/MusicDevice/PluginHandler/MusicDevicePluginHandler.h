#ifndef MUSIC_DEVICE_PLUGIN_H
#define MUSIC_DEVICE_PLUGIN_H

#include <optional>
#include <vector>
#include <exception>
#include <string>
#include "pluginIf.h" // ::createEnlightLedMidiMsg

namespace base::musicDevice::description::controller { struct Widget; }
namespace base::musicDevice::controller{
   struct WidgetCoord;
   struct ColorARGB;
}

namespace base::musicDevice
{

class PluginHandler
{
public:
   struct PluginOpenError : public std::runtime_error
   { PluginOpenError(const std::string desc) : std::runtime_error(desc) {} };
   struct SymbolError : public std::runtime_error
   { SymbolError(const std::string desc) : std::runtime_error(desc) {} };

   PluginHandler(std::string resourceRootDir) noexcept;
   ~PluginHandler() noexcept;
   PluginHandler(const PluginHandler&) = delete;
   PluginHandler& operator=(const PluginHandler&) = delete;
   PluginHandler(PluginHandler&&) noexcept = default;

   void load(const std::string& deviceRelPath);

   std::optional<std::vector<midi::MidiMessage>> createEnlightLedMidiMsg(
      const description::controller::Widget& widgetDescr,
      const controller::WidgetCoord& widgetCoord,
      const controller::ColorARGB& colorARGB) noexcept;
private:
   std::string m_resourceRootDir;
   decltype(::createEnlightLedMidiMsg)* m_createEnlightLedMidiMsg{nullptr};
   void* m_handle{nullptr};
};

} // namespace base::musicDevice

#endif