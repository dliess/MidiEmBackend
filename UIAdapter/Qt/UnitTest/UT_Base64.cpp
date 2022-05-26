#include <gtest/gtest.h>

#include <QByteArray>
#include <QDebug>

#include "Identifiable.h"

struct TestIdentifiable : util::Identifiable
{
};

TEST(Base64, test)
{
   TestIdentifiable uuid;
   QByteArray byteArray;
   const QByteArray uuid_ba(reinterpret_cast<const char *>(uuid.id().begin()),
                            int(uuid.id().size()));
   const auto uuid_ba_encoded = uuid_ba.toBase64();
   const auto uuid_ba_decoded =
       QByteArray::fromBase64(uuid_ba_encoded, QByteArray::Base64Encoding);
   util::Identifiable::UUID uuid_back;
   
   std::copy(uuid_ba_decoded.begin(), uuid_ba_decoded.end(),
             uuid_back.begin());

   qDebug() << "uuid: " << util::uuid2Str(uuid.id()).c_str();
   qDebug() << "uuid_ba: " << uuid_ba;
   qDebug() << "uuid_ba_encoded: " << uuid_ba_encoded;
   qDebug() << "uuid_ba_decoded: " << uuid_ba_decoded;
   qDebug() << "uuid_back: " << util::uuid2Str(uuid_back).c_str();

   ASSERT_EQ(uuid_ba, uuid_ba_decoded);
}