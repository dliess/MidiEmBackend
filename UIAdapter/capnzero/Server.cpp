#include "Server.h"
#include "Instruments.h"
#include "InstrumentsRpc.h"
#include "MusicDeviceContainer.h"
#include "MusicDevicesRpc.h"
#include "MusicDeviceDescription.h"

#include "JsonCast.h" // meta::serialize

using namespace uiadapter::capnzero;

Server::Server(zmq::context_t &rZmqContext,
               base::instruments::Instruments &rInstruments,
               base::musicDevice::MusicDeviceContainer &rMusicDeviceContainer)
    : ::capnzero::MidiEm::MidiEmServer(
          rZmqContext, "tcp://*:5555", "tcp://*:5556",
          std::make_unique<InstrumentsRpc>(rInstruments),
          std::make_unique<MusicDevicesRpc>(rMusicDeviceContainer)) {
  rInstruments.registerForDataChange([this, &rInstruments]() {
    Super::signals().Instruments__kitInstrumentsChanged(
        meta::serialize(rInstruments.data.kitInstruments).dump().c_str());
    Super::signals().Instruments__melodicInstrumentsChanged(
        meta::serialize(rInstruments.data.melodicInstruments).dump().c_str());
  });

  Super::signals().registerInstrumentsKitInstrumentsChangedSubscrCb(
      [&rInstruments](Signals &rSignals) {
        rSignals.Instruments__kitInstrumentsChanged(
            meta::serialize(rInstruments.data.kitInstruments).dump().c_str());
      });
  Super::signals().registerInstrumentsMelodicInstrumentsChangedSubscrCb(
      [&rInstruments](Signals &rSignals) {
        rSignals.Instruments__melodicInstrumentsChanged(
            meta::serialize(rInstruments.data.melodicInstruments)
                .dump()
                .c_str());
      });

  Super::signals().registerMusicDevicesMusicDevicesChangedSubscrCb(
      [&rMusicDeviceContainer](Signals &rSignals) {
        rSignals.MusicDevices__musicDevicesChanged(
            meta::serialize(rMusicDeviceContainer).dump().c_str());
      });

  rMusicDeviceContainer.registerForAdd(
      [this, &rMusicDeviceContainer](std::shared_ptr<base::musicDevice::MusicDevice> ptr) {
        signals().MusicDevices__musicDevicesChanged(meta::serialize(rMusicDeviceContainer).dump().c_str());

        const auto& deviceName = ptr.get()->deviceId().deviceName;
        const auto& description = *ptr.get()->description();
        signals().MusicDevices__musicDeviceDescriptionAdded(deviceName, meta::serialize(description).dump().c_str());    

      });
  rMusicDeviceContainer.registerForAboutToRemove(
      [this, &rMusicDeviceContainer](std::shared_ptr<base::musicDevice::MusicDevice> ptr) {
        signals().MusicDevices__musicDevicesChanged(
            meta::serialize(rMusicDeviceContainer).dump().c_str());
      });

    Super::signals().registerMusicDevicesMusicDeviceDescriptionAddedSubscrCb([&rMusicDeviceContainer, this](Signals &rSignals) {
        for(auto& it : rMusicDeviceContainer){
            const auto& deviceName = it.second.get()->deviceId().deviceName;
            const auto& description = *it.second.get()->description();
            signals().MusicDevices__musicDeviceDescriptionAdded(deviceName, meta::serialize(description).dump().c_str());    
        }
    });
}
