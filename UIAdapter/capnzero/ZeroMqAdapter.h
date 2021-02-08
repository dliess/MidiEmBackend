#ifndef UIADAPTER_ZEROMQ_ZEROMQ_ADAPTER_H
#define UIADAPTER_ZEROMQ_ZEROMQ_ADAPTER_H

#include <zmq.hpp>

namespace uiadapter::zeromq {

class ZeroMqAdapter {
public:
  ZeroMqAdapter();
  ~ZeroMqAdapter();
  void peekForRequests();

private:
  zmq::context_t m_zmqContext;
  zmq::socket_t m_sockRep;
  zmq::socket_t m_sockPub;
};

} // namespace uiadapter::zeromq

#endif // UIADAPTER_ZEROMQ_ZEROMQ_ADAPTER_H