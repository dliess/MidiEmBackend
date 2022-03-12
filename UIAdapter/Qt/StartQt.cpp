#include "StartQt.h"
#include "Base.h"

// Qt stuff
#include <WebSocketClientWrapper.h>
#include <WebSocketTransport.h>

#include <QCoreApplication>
#include <QWebChannel>
#include <QWebSocketServer>

#include "MidiEmRt_QObjectClient.h"
#include "MidiEmLoader_QObjectClient.h"

int uiadapter::qt::startQt(base::Base& base, int& argc, char**& argv)
{
   QCoreApplication app(argc, argv);
   app.setApplicationName("NMBackend-Qt");
   QWebSocketServer server(QStringLiteral("Push2BackendServer"),
                           QWebSocketServer::NonSecureMode);
   if (!server.listen(QHostAddress::Any, 55560))
   {
      qFatal("Failed to open web socket server.");
      return 1;
   }
   WebSocketClientWrapper clientWrapper(&server);
   QWebChannel channel;
   QObject::connect(&clientWrapper, &WebSocketClientWrapper::clientConnected,
                    &channel, &QWebChannel::connectTo);

   zmq::context_t m_zmqContext;
   capnzero::MidiEmRt::QClient rtClient(m_zmqContext, "tcp://localhost:55555", "tcp://localhost:55556");
   capnzero::MidiEmLoader::QClient loaderClient(m_zmqContext, "tcp://localhost:55557", "tcp://localhost:55558");

   channel.registerObject(QStringLiteral("MidiEmBackendRtServer"), &rtClient);
   channel.registerObject(QStringLiteral("MidiEmBackendLoaderServer"), &loaderClient);
   return app.exec();
}