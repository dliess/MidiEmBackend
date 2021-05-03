#include "StartQt.h"
#include "Base.h"

// Qt stuff
#include <WebSocketClientWrapper.h>
#include <WebSocketTransport.h>

#include <QCoreApplication>
#include <QWebChannel>
#include <QWebSocketServer>

#include "MidiEm_QObjectClient.h"

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

   zmq::context_t m_zmqContext;
   capnzero::MidiEm::QClient client(m_zmqContext, "tcp://localhost:5555", "tcp://localhost:5556");

   channel.registerObject(QStringLiteral("MidiEmBackendServer"), &client);
   return app.exec();
}