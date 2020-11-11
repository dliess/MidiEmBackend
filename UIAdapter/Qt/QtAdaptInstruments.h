#ifndef UIADAPTER_QT_INSTRUMENTS_H
#define UIADAPTER_QT_INSTRUMENTS_H

#include <QObject>

namespace base::instruments
{
struct Instruments;
}

namespace uiadapter::qt
{
class Instruments : public QObject
{
   Q_OBJECT
public:
   explicit Instruments(base::instruments::Instruments& rInstruments,
                        QObject* pParent = Q_NULLPTR) noexcept;
   Q_INVOKABLE void triggerSendData() noexcept; // TODO const
   Q_INVOKABLE void createKitInstrument(const QString& name) noexcept;
   Q_INVOKABLE void removeKitInstrument(const QString& instrumentUUID) noexcept;
   Q_INVOKABLE void createMelodicInstrument(const QString& name) noexcept;
   Q_INVOKABLE void removeMelodicInstrument(
      const QString& instrumentUUID) noexcept;

   Q_INVOKABLE void createKitInstrumentSound(const QString& instrumentUUID,
                                             const QString& name) noexcept;
   Q_INVOKABLE void removeKitInstrumentSound(
      const QString& kitSoundUUID) noexcept;
   Q_INVOKABLE void addVoiceToKitInstrumentSound(const QString& kitSoundUUID,
                                                 const QString& soundDeviceId,
                                                 int voiceIdx) noexcept;
   Q_INVOKABLE void removeVoiceFromInstrumentSound(
      const QString& voiceUUID) noexcept;

   Q_INVOKABLE void addVoiceToMelodicInstrument(const QString& instrumentUUID,
                                                const QString& soundDeviceId,
                                                int voiceIdx) noexcept;
   Q_INVOKABLE void removeVoiceFromMelodicInstrument(
      const QString& voiceUUID) noexcept;
signals:
   void dataChanged(const QString& jsonStrData);

private:
   base::instruments::Instruments& m_rInstruments;
};

} // namespace uiadapter::qt
#endif