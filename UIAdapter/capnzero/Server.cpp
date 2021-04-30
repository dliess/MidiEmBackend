#include "Server.h"
#include "Instruments.h"
#include "InstrumentsRpc.h"

#include "JsonCast.h" // meta::serialize

using namespace uiadapter::capnzero;

Server::Server(base::instruments::Instruments &rInstruments)
    : ::capnzero::MidiEm::MidiEmServer(
          m_zmqContext, "tcp://*:5555", "tcp://*:5556",
          std::make_unique<InstrumentsRpc>(rInstruments)) {
  rInstruments.registerForDataChange([this, rInstruments]() {
    Super::signals().Instruments__dataChanged(
        meta::serialize(rInstruments.data).dump().c_str());
  });
}
