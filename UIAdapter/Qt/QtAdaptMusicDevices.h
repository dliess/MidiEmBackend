#ifndef UIADAPTER_QT_MUSIC_DEVICES_H
#define UIADAPTER_QT_MUSIC_DEVICES_H

#include <QObject>

namespace base::musicDevice
{
struct Holder;
}

namespace uiadapter::qt
{
class MusicDevices : public QObject
{
   Q_OBJECT
public:
   explicit MusicDevices(base::musicDevice::Holder& rMusicDeviceHolder,
                         QObject* pParent = Q_NULLPTR) noexcept;

   Q_INVOKABLE void triggerSendMusicDeviceList() noexcept; // TODO const
   Q_INVOKABLE void registerForParameterChange(const QString& deviceIdStr,
                                               int voiceIdx) noexcept;
   Q_INVOKABLE void unregisterForParameterChange(const QString& deviceIdStr,
                                                 int voiceIdx) noexcept;

signals:
   void musicDeviceListChanged(const QString& deviceNameList);
   void parameterChanged(const QString& deviceId, int voiceId, int paramIdx,
                         float commanded, float actual);

private:
   base::musicDevice::Holder& m_rMusicDeviceHolder;
   [[nodiscard]] QString createDeviceListStr() const noexcept;
};
} // namespace uiadapter::qt
#endif