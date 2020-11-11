#include "MusicDevicePluginHandler.h"

#include <dlfcn.h>

namespace base::musicDevice
{

PluginHandler::PluginHandler(
   std::string resourceRootDir) noexcept :
   m_resourceRootDir(std::move(resourceRootDir))
{
}

PluginHandler::~PluginHandler() noexcept
{
   if (m_handle)
   {
      dlclose(m_handle);
   }
}

void PluginHandler::load(const std::string& deviceRelPath)
{
   const std::string pluginPath =
      fmt::format("{}/MusicDevicePlugins/Devices/{}/libplugin.so",
                  m_resourceRootDir, deviceRelPath);
   m_handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
   if (!m_handle)
   {
      throw PluginOpenError(dlerror());
   }
   dlerror();
   m_createEnlightLedMidiMsg = (decltype(m_createEnlightLedMidiMsg))dlsym(
      m_handle, "createEnlightLedMidiMsg");
   const char* dlsym_error = dlerror();
   if (dlsym_error)
   {
      throw SymbolError(dlsym_error);
   }
}

std::optional<std::vector<midi::MidiMessage>>
PluginHandler::createEnlightLedMidiMsg(
   const description::controller::Widget& widgetDescr,
   const controller::WidgetCoord& widgetCoord,
   const controller::ColorARGB& colorARGB) noexcept
{
   if (m_createEnlightLedMidiMsg)
   {
      return m_createEnlightLedMidiMsg(widgetDescr, widgetCoord, colorARGB);
   }
   return std::nullopt;
}

} // namespace base::musicDevice