#include "QtAdaptMusicDevices.h"

#include <QDebug>
#include <string>

#include "Meta.h"
#include "MusicDevice.h"
#include "MusicDeviceHolder.h"

uiadapter::qt::MusicDevices::MusicDevices(
   base::musicDevice::Holder& rMusicDeviceHolder, QObject* pParent) noexcept :
   QObject(pParent),
   m_rMusicDeviceHolder(rMusicDeviceHolder)
{
   m_rMusicDeviceHolder.musicDevices.registerForAdd(
      [this](std::shared_ptr<base::musicDevice::MusicDevice> pMusicDevice) {
         emit musicDeviceListChanged(createDeviceListStr());
      });
   m_rMusicDeviceHolder.musicDevices.registerForAboutToRemove(
      [this](std::shared_ptr<base::musicDevice::MusicDevice> pMusicDevice) {
         emit musicDeviceListChanged(createDeviceListStr());
      });
}

void uiadapter::qt::MusicDevices::triggerSendMusicDeviceList() noexcept
{
   emit musicDeviceListChanged(createDeviceListStr());
}

QString uiadapter::qt::MusicDevices::createDeviceListStr() const noexcept
{
   std::vector<base::musicDevice::MusicDeviceId> v;
   v.reserve(m_rMusicDeviceHolder.musicDevices.size());
   for (const auto e : m_rMusicDeviceHolder.musicDevices)
   {
      v.emplace_back(e.first);
   }
   return QString(meta::serialize(v).dump().c_str());
}

void uiadapter::qt::MusicDevices::registerForParameterChange(
   const QString& deviceIdStr, int voiceIdx) noexcept
{
   qDebug() << "registerForParameterChange(" << deviceIdStr << ", " << voiceIdx;
   const base::musicDevice::MusicDeviceId deviceId(deviceIdStr.toStdString());
   auto it = m_rMusicDeviceHolder.musicDevices.find(deviceId);
   if (it == m_rMusicDeviceHolder.musicDevices.end())
   {
      LOG_F(ERROR, "DeviceId '{}' not found", deviceId.toStr());
      return;
   }
   assert(it->second);
   assert(it->second->soundHandler);
   it->second->soundHandler->registerForParameterChange(
      voiceIdx, [this, deviceIdStr](int voiceId, int paramIdx, float commanded, float actual) {
         emit parameterChanged(deviceIdStr, voiceId, paramIdx, commanded, actual);
      }
   );
}

void uiadapter::qt::MusicDevices::unregisterForParameterChange(
   const QString& deviceIdStr, int voiceIdx) noexcept
{
   qDebug() << "unRegisterForParameterChange(" << deviceIdStr << ", "
            << voiceIdx;
   const base::musicDevice::MusicDeviceId deviceId(deviceIdStr.toStdString());
   auto it = m_rMusicDeviceHolder.musicDevices.find(deviceId);
   if (it == m_rMusicDeviceHolder.musicDevices.end())
   {
      LOG_F(ERROR, "DeviceId '{}' not found", deviceId.toStr());
      return;
   }
   assert(it->second);
   assert(it->second->soundHandler);
   it->second->soundHandler->unregisterForParameterChange(voiceIdx);
}
