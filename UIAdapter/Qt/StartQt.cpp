#include "StartQt.h"
#include "Base.h"

// Qt stuff
#include <WebSocketClientWrapper.h>
#include <WebSocketTransport.h>

#include <QCoreApplication>
#include <QWebChannel>
#include <QWebSocketServer>

#include "QtAdaptInstruments.h"
#include "QtAdaptMusicDevices.h"

int uiadapter::qt::startQt(base::Base& base, int& argc, char**& argv)
{
   QCoreApplication app(argc, argv);
   QWebSocketServer server(QStringLiteral("Push2BackendServer"),
                           QWebSocketServer::NonSecureMode);
   if (!server.listen(QHostAddress::Any, 12345))
   {
      qFatal("Failed to open web socket server.");
      return 1;
   }
   WebSocketClientWrapper clientWrapper(&server);
   QWebChannel channel;
   QObject::connect(&clientWrapper, &WebSocketClientWrapper::clientConnected,
                    &channel, &QWebChannel::connectTo);

   uiadapter::qt::Instruments instrumentsAdapter(base.instruments);
   uiadapter::qt::MusicDevices musicDevicesAdapter(base.musicDeviceHolder);

   channel.registerObject(QStringLiteral("Instruments"), &instrumentsAdapter);
   channel.registerObject(QStringLiteral("MusicDevices"), &musicDevicesAdapter);
   return app.exec();
}