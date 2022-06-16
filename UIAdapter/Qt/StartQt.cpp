#include "StartQt.h"

// Qt stuff
#include <WebSocketClientWrapper.h>
#include <WebSocketTransport.h>

#include <QCoreApplication>
#include <QWebChannel>
#include <QWebSocketServer>

#include "MidiEmLoader_QObjectClient.h"
#include "MidiEmRt_QObjectClient.h"

int uiadapter::qt::startQt(const std::string& rtServerRpcAddr,
                           const std::string& rtServerSignalAddr,
                           const std::string& loaderServerRpcAddr,
                           const std::string& loaderServerSignalAddr, int& argc,
                           char**& argv)
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
   capnzero::MidiEmRt::QClient rtClient(m_zmqContext, rtServerRpcAddr,
                                        rtServerSignalAddr);
   capnzero::MidiEmLoader::QClient loaderClient(
       m_zmqContext, loaderServerRpcAddr, loaderServerSignalAddr);

   channel.registerObject(QStringLiteral("Rt"), &rtClient);
   channel.registerObject(QStringLiteral("Loader"), &loaderClient);
   return app.exec();
}