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
  rInstruments.registerForDataChange([this, &rInstruments]() {
    Super::signals().Instruments__kitInstrumentsChanged(
        meta::serialize(rInstruments.data.kitInstruments).dump().c_str());
    Super::signals().Instruments__melodicInstrumentsChanged(
        meta::serialize(rInstruments.data.melodicInstruments).dump().c_str());
  });

  Super::signals().registerInstrumentsKitInstrumentsChangedSubscrCb(
      [rInstruments](Signals &rSignals) {
        rSignals.Instruments__kitInstrumentsChanged(
            meta::serialize(rInstruments.data.kitInstruments).dump().c_str());
      });
  Super::signals().registerInstrumentsMelodicInstrumentsChangedSubscrCb(
      [rInstruments](Signals &rSignals) {
        rSignals.Instruments__melodicInstrumentsChanged(
            meta::serialize(rInstruments.data.melodicInstruments)
                .dump()
                .c_str());
      });
}
