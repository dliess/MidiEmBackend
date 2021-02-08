#include "ZeroMqAdapter.h"
#include "IfDescription.h"

using namespace uiadapter::zeromq;

ZeroMqAdapter::ZeroMqAdapter()
    : m_zmqContext(0), m_sockRep(m_zmqContext, zmq::socket_type::rep),
      m_sockPub(m_zmqContext, zmq::socket_type::pub) {}

ZeroMqAdapter::~ZeroMqAdapter() { m_zmqContext.shutdown(); }


void ZeroMqAdapter::peekForRequests() {
  /*
  int buffer;
  auto buf = zmq::mutable_buffer(&buffer, sizeof(buffer));
  */
  zmq::message_t buf;
  auto res = m_sockRep.recv(buf, zmq::recv_flags::dontwait);
  if (res) {
  }
}
