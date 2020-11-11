#include "QtAdaptInstruments.h"

#include "Instruments.h"
#include "JsonCast.h"
#include <QDebug>

inline util::Identifiable::UUID str2Id(const QString& str) noexcept
{
   const auto j = nlohmann::json::parse(str.toStdString());
   return meta::deserialize<util::Identifiable::UUID>(j);
}

uiadapter::qt::Instruments::Instruments(
   base::instruments::Instruments& rInstruments, QObject* pParent) noexcept :
   QObject(pParent),
   m_rInstruments(rInstruments)
{
   rInstruments.registerForDataChange([this]() {
      emit dataChanged(
         QString(meta::serialize(m_rInstruments.data).dump().c_str()));
   });
}

void uiadapter::qt::Instruments::triggerSendData() noexcept
{
   qDebug() << "Data Changed: " << meta::serialize(m_rInstruments.data).dump().c_str(); 
   emit dataChanged(
      QString(meta::serialize(m_rInstruments.data).dump().c_str()));
}

void uiadapter::qt::Instruments::createKitInstrument(
   const QString& name) noexcept
{
   m_rInstruments.createKitInstrument(name.toStdString());
}

void uiadapter::qt::Instruments::removeKitInstrument(
   const QString& instrumentUUID) noexcept
{
   m_rInstruments.removeKitInstrument(str2Id(instrumentUUID));
}

void uiadapter::qt::Instruments::createMelodicInstrument(
   const QString& name) noexcept
{
   m_rInstruments.createMelodicInstrument(name.toStdString());
}

void uiadapter::qt::Instruments::removeMelodicInstrument(
   const QString& instrumentUUID) noexcept
{
   m_rInstruments.removeMelodicInstrument(str2Id(instrumentUUID));
}

void uiadapter::qt::Instruments::createKitInstrumentSound(
   const QString& instrumentUUID, const QString& name) noexcept
{
   m_rInstruments.createKitInstrumentSound(str2Id(instrumentUUID),
                                           name.toStdString());
}

void uiadapter::qt::Instruments::removeKitInstrumentSound(
   const QString& kitSoundUUID) noexcept
{
   m_rInstruments.removeKitInstrumentSound(str2Id(kitSoundUUID));
}

void uiadapter::qt::Instruments::addVoiceToKitInstrumentSound(
   const QString& kitSoundUUID, const QString& soundDeviceId,
   int voiceIdx) noexcept
{
   m_rInstruments.addVoiceToKitInstrumentSound(
      str2Id(kitSoundUUID),
      base::musicDevice::MusicDeviceId(soundDeviceId.toStdString()), voiceIdx);
}

void uiadapter::qt::Instruments::removeVoiceFromInstrumentSound(
   const QString& voiceUUID) noexcept
{
   m_rInstruments.removeVoiceFromInstrumentSound(str2Id(voiceUUID));
}

void uiadapter::qt::Instruments::addVoiceToMelodicInstrument(
   const QString& instrumentUUID, const QString& soundDeviceId,
   int voiceIdx) noexcept
{
   m_rInstruments.addVoiceToMelodicInstrument(
      str2Id(instrumentUUID),
      base::musicDevice::MusicDeviceId(soundDeviceId.toStdString()), voiceIdx);
}
void uiadapter::qt::Instruments::removeVoiceFromMelodicInstrument(
   const QString& voiceUUID) noexcept
{
   m_rInstruments.removeVoiceFromMelodicInstrument(str2Id(voiceUUID));
}
