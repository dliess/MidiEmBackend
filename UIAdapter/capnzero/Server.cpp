#include "Server.h"
#include "Instruments.h"
#include "InstrumentsRpc.h"

#include "JsonCast.h" // meta::serialize

using namespace uiadapter::capnzero;

Server::Server(zmq::context_t &rZmqContext,
               base::instruments::Instruments &rInstruments)
    : ::capnzero::MidiEm::MidiEmServer(
          rZmqContext, "tcp://*:5555", "tcp://*:5556",
          std::make_unique<InstrumentsRpc>(rInstruments)) {
  rInstruments.registerForDataChange([this, rInstruments]() {
    LOG_F(INFO, "Sending {}",
          meta::serialize(rInstruments.data).dump().c_str());
    Super::signals().Instruments__dataChanged(
        meta::serialize(rInstruments.data).dump().c_str());
  });

  Super::signals().registerInstrumentsDataChangedSubscrCb(
      [rInstruments](Signals &rSignals) {
        LOG_F(INFO, "Sending {}",
              meta::serialize(rInstruments.data).dump().c_str());
        rSignals.Instruments__dataChanged(
            meta::serialize(rInstruments.data).dump().c_str());
      });
}
