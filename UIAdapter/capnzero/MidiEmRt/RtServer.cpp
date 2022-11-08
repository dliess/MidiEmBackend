#include "RtServer.h"

#include "AbletonLinkRpc.h"
#include "AbletonLinkWrapper.h"
#include "ControllerDevicesRpc.h"
#include "ControllerEventRouter.h"
#include "ControllerEventRouterRpc.h"
#include "Instruments.h"
#include "InstrumentsRpc.h"
#include "JsonCast.h"   // meta::serialize
#include "MainRpc.h"
#include "MidiRouter.h"
#include "MidiRoutingRpc.h"
#include "MusicDeviceDescription.h"
#include "MusicDeviceHolder.h"
#include "ParameterSceneContainer.h"
#include "ParameterSceneRpc.h"
#include "SessionRpc.h"
#include "SoundDevicesRpc.h"
#include "TempoRpc.h"
#include "TransportControl.h"
#include "TransportControlRpc.h"

using namespace uiadapter::capnzero;
using ::capnzero::MidiEmRt::MidiEmRtServer;

RtServer::RtServer(
    zmq::context_t &rZmqContext, const std::string &rpcBindAddr,
    const std::string &signalBindAddr,
    base::instruments::Instruments &rInstruments,
    base::musicDevice::Holder &rMDHolder,
    base::TransportControl &rTransportControl,
    base::AbletonLinkWrapper &rAbletonLinkWrapper,
    base::midifriends::Router &rMidiRouter,
    base::musicDevice::controller::EventRouter &rCtrlEventRouter,
    base::musicDevice::sound::ParameterSceneContainer &rParameterSceneContainer,
    base::session::Tracks &rTracks) :
    MidiEmRtServer(
        rZmqContext, rpcBindAddr, signalBindAddr,
        std::make_unique<InstrumentsRpc>(rInstruments),
        std::make_unique<MainRpc>(
            signals(), rInstruments, rMDHolder.musicDevices, rTransportControl,
            rAbletonLinkWrapper, rMidiRouter, rCtrlEventRouter,
            rParameterSceneContainer, rTracks),
        std::make_unique<SoundDevicesRpc>(rMDHolder.musicDevices),
        std::make_unique<ParameterSceneRpc>(rParameterSceneContainer),
        std::make_unique<ControllerDevicesRpc>(),
        std::make_unique<TempoRpc>(Super::signals(), rMDHolder),
        std::make_unique<TransportControlRpc>(rTransportControl),
        std::make_unique<AbletonLinkRpc>(rAbletonLinkWrapper),
        std::make_unique<MidiRoutingRpc>(rMidiRouter),
        std::make_unique<ControllerEventRouterRpc>(rCtrlEventRouter,
                                                   rMDHolder.musicDevices),
        std::make_unique<SessionRpc>(rTracks))
{
   /*
  Super::signals().registerAbletonLinkEnabledChangedSubscrCb(
      [&rAbletonLinkWrapper](Signals &signals) {
          signals.AbletonLink__enabledChanged(rAbletonLinkWrapper.isEnabled());
      });
   */

   rInstruments.registerForDataChange([this, &rInstruments]() {
      Super::signals().Instruments__kitInstrumentsChanged(
          meta::serialize(rInstruments.data.kitInstruments).dump().c_str());
      Super::signals().Instruments__melodicInstrumentsChanged(
          meta::serialize(rInstruments.data.melodicInstruments).dump().c_str());
   });

   rMDHolder.musicDevices.onAboutToAdd(
       [this](const base::musicDevice::MusicDevice &md) {
          const auto &deviceName  = md.deviceId().deviceName;
          const auto &description = *md.description();
          const auto &mediumId    = md.mediumId();
          const auto midiVoiceOffset =
              md.soundHandler ? md.soundHandler->getMidiVoiceOffset() : 0;
          signals().MusicDevices__musicDeviceDescriptionAdded(
              deviceName, meta::serialize(description).dump().c_str());
          signals().MusicDevices__deviceAdded(
              md.id(), md.deviceId().deviceName, md.deviceId().portName,
              mediumId.toStr(), midiVoiceOffset);
       });

   rMDHolder.musicDevices.onAboutToRemove(
       [this](std::shared_ptr<base::musicDevice::MusicDevice> ptr) {
          signals().MusicDevices__deviceRemoved(ptr.get()->id());
       });

   rMDHolder.musicDevices.onSoundDevParamChanged(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              float commanded, float actual) {
          signals().SoundDevices__parameterChanged(uuid, voiceId, paramIdx,
                                                   commanded, actual);
       });

   rMDHolder.musicDevices.onControllerDevEventOccuredUI(
       [this](util::Identifiable::UUID uuid,
              const base::musicDevice::controller::Event &event) {
          const float val = mpark::visit(
              util::overload{
                  [](const mpark::monostate &value) -> float { return 0; },
                  [](auto &&value) -> float { return value.value; }},
              event.value);
          mpark::visit(
              util::overload{
                  [this, &event, &uuid, val](const mpark::monostate &) {
                     signals().ControllerDevices__controllerEventOccured(
                         uuid, event.id.widgetId, 0, 0, event.id.eventId,
                         event.id.channelId, val);
                  },
                  [this, &event, &uuid,
                   val](const base::musicDevice::controller::WidgetCoord
                            &widgetCoord) {
                     signals().ControllerDevices__controllerEventOccured(
                         uuid, event.id.widgetId, widgetCoord.col,
                         widgetCoord.row, event.id.eventId, event.id.channelId,
                         val);
                  },
                  [this, &event, &uuid,
                   val](const base::musicDevice::controller::Note &note) {
                     signals().ControllerDevices__controllerNoteEventOccured(
                         uuid, event.id.widgetId, note.number, event.id.eventId,
                         event.id.channelId, val);
                  }},
              event.id.widgetCoord);
       });

   rMDHolder.musicDevices.onLFOWaveformChanged(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              base::musicDevice::sound::lfo::Waveform waveform) {
          signals().SoundDevices__lFOWaveformChanged(
              uuid, voiceId, paramIdx,
              static_cast<::capnzero::MidiEmRt::LFOWaveform>(waveform));
       });

   rMDHolder.musicDevices.onLFOAmplitudeChanged(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              float amplitude) {
          signals().SoundDevices__lFOAmplitudeChanged(uuid, voiceId, paramIdx,
                                                      amplitude);
       });

   rMDHolder.musicDevices.onLFOFrequencyChanged(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              float frequency) {
          signals().SoundDevices__lFOFrequencyChanged(uuid, voiceId, paramIdx,
                                                      frequency);
       });

   rMDHolder.musicDevices.onLFOMultiplierExpChanged(
       [this](util::Identifiable::UUID uuid, int voiceId, int paramIdx,
              uint32_t multiplierExp) {
          signals().SoundDevices__lFOMultiplierExpChanged(
              uuid, voiceId, paramIdx, multiplierExp);
       });

   rMDHolder.musicDevices.onEnginePresetChanged(
       [this](const std::string &musicDeviceName, int engineIdx,
              const std::string &presetName) {
          signals().SoundDevices__presetChanged(musicDeviceName, engineIdx,
                                                presetName);
       });
   // Actual Preset -----------------
   rMDHolder.musicDevices.onActualPresetChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx,
              const std::string &presetName) {
          signals().SoundDevices__actualPresetChanged(uuid, voiceIdx,
                                                      presetName);
       });

   // ARP -------------
   rMDHolder.musicDevices.onArpBypassChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx, bool on) {
          signals().SoundDevices__arpeggiatorBypassChanged(uuid, voiceIdx, on);
       });
   rMDHolder.musicDevices.onArpRangeTypeChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx,
              base::arp::RangeType rangeType) {
          signals().SoundDevices__arpeggiatorRangeTypeChanged(
              uuid, voiceIdx,
              static_cast<::capnzero::MidiEmRt::ArpeggiatorRangeType>(
                  rangeType));
       });
   rMDHolder.musicDevices.onArpRangeChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx, int range) {
          signals().SoundDevices__arpeggiatorRangeChanged(uuid, voiceIdx,
                                                          range);
       });
   rMDHolder.musicDevices.onArpGateFillChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx, float gateFill) {
          signals().SoundDevices__arpeggiatorGateFillChanged(uuid, voiceIdx,
                                                             gateFill);
       });
   rMDHolder.musicDevices.onArpStepLengthChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx, float stepLength) {
          signals().SoundDevices__arpeggiatorStepLengthChanged(uuid, voiceIdx,
                                                               stepLength);
       });
   rMDHolder.musicDevices.onArpAlgorithmChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx,
              base::arp::Algorithm algorithm) {
          signals().SoundDevices__arpeggiatorAlgorithmChanged(
              uuid, voiceIdx,
              static_cast<::capnzero::MidiEmRt::ArpeggiatorAlgorithm>(
                  algorithm));
       });
   rMDHolder.musicDevices.onArpHoldNotesChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx, bool holdNotes) {
          signals().SoundDevices__arpeggiatorHoldNotesChanged(uuid, voiceIdx,
                                                              holdNotes);
       });
   rMDHolder.musicDevices.onArpFeedModeChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx,
              base::arp::FeedMode feedMode) {
          signals().SoundDevices__arpeggiatorFeedModeChanged(
              uuid, voiceIdx,
              static_cast<::capnzero::MidiEmRt::ArpeggiatorFeedMode>(feedMode));
       });
   rMDHolder.musicDevices.onArpSeqSizeChanged(
       [this](util::Identifiable::UUID uuid, int voiceIdx, int seqSize) {
          signals().SoundDevices__arpeggiatorSeqSizeChanged(uuid, voiceIdx,
                                                            seqSize);
       });
   // -----------------

   rTransportControl.onTransportMaskChanged(
       [this](const util::Identifiable::UUID &uuid, bool masked) {
          signals().TransportControl__enabledChanged(uuid, !masked);
       });
   rTransportControl.onStartedChanged([this](bool started) {
      signals().TransportControl__startedChanged(started ? 1 : 0);
   });
   rTransportControl.onStartOnBeat([this](bool startOnBeat) {
      signals().TransportControl__quantizedStartChanged(startOnBeat);
   });
   rAbletonLinkWrapper.onEnabledChanged([this](bool enabled) {
      signals().AbletonLink__enabledChanged(enabled);
   });
   rAbletonLinkWrapper.onReactsOnTransportChanged([this](bool reacts) {
      signals().AbletonLink__reactOnTransportChanged(reacts);
   });
   rAbletonLinkWrapper.onNumPeersChanged([this](size_t numPeers) {
      signals().AbletonLink__numberOfParticipantsChanged(numPeers);
   });
   rAbletonLinkWrapper.offset.onOffsetSetupBpmChanged([this](double bpm) {
      signals().AbletonLink__offsetSetupTempoChanged(bpm);
   });
   rAbletonLinkWrapper.offset.onOffsetBeatsChanged([this](double offsetBeats) {
      signals().AbletonLink__offsetBeatsChanged(offsetBeats);
   });
   rAbletonLinkWrapper.offset.onOffsetUsChanged(
       [this](const std::chrono::microseconds &us) {
          signals().AbletonLink__offsetTimeUsChanged(us.count());
       });

   rMidiRouter.registerRoutedChangedCB(
       [this](const base::musicDevice::MidiHolder::Id &source,
              const base::musicDevice::MidiHolder::Id &dest, bool routed) {
          if (routed)
          {
             signals().MidiRouting__routedAdded(source.toStr(), dest.toStr());
          }
          else
          {
             signals().MidiRouting__routedRemoved(source.toStr(), dest.toStr());
          }
       });
   rMidiRouter.registerSpecialRoutedChangedCB(
       [this](const base::musicDevice::MidiHolder::Id &source,
              const base::musicDevice::MidiHolder::Id &dest, bool created) {
          if (created)
          {
             signals().MidiRouting__specializedRoutingInited(source.toStr(),
                                                             dest.toStr());
          }
          else
          {
             signals().MidiRouting__specializedRoutingCleared(source.toStr(),
                                                              dest.toStr());
          }
       });
   rMidiRouter.registerSpecialRouteChangedCB(
       [this](const base::musicDevice::MidiHolder::Id &source,
              const base::musicDevice::MidiHolder::Id &dest,
              uint8_t sourceChannel, uint8_t destChannel, bool enabled) {
          signals().MidiRouting__specializedRoutingSet(
              source.toStr(), dest.toStr(), sourceChannel, destChannel,
              enabled);
       });

   rCtrlEventRouter.onGotConnected([this](const base::musicDevice::controller::
                                              EventIdExt &from,
                                          const base::musicDevice::controller::
                                              EventDestination &to) {
      mpark::visit(
          util::overload{
              [this](
                  const mpark::monostate
                      &) { /* TODO */
                           spdlog::error(
                               "Unhandled path in controller-event-connection");
              },
              [&](const base::musicDevice::controller::WidgetCoord
                      &widgetCoord) {
                 mpark::visit(
                     util::overload{
                         [&](const base::musicDevice::controller::
                                 EventDestination::Note &note) {
                            signals()
                                .ControllerEventRouter__connectedWidget2Notes(
                                    from.uuid, from.eventId.widgetId,
                                    widgetCoord.col, widgetCoord.row,
                                    from.eventId.eventId,
                                    from.eventId.channelId, to.uuid,
                                    to.voiceIdx);
                         },
                         [&](const base::musicDevice::controller::
                                 EventDestination::Parameter &parameter) {
                            signals()
                                .ControllerEventRouter__connectedWidget2Parameter(
                                    from.uuid, from.eventId.widgetId,
                                    widgetCoord.col, widgetCoord.row,
                                    from.eventId.eventId,
                                    from.eventId.channelId, to.uuid,
                                    to.voiceIdx, parameter.id,
                                    static_cast<int>(
                                        ::capnzero::MidiEmRt::
                                            SDParameterDestination::PARAMETER));
                         },
                         [](auto &&) {
                            spdlog::error("Unhandled path in "
                                          "controller-event-connection");
                         }},
                     to.endpoint);
              },
              [&](const base::musicDevice::controller::Note &note) {
                 mpark::visit(
                     util::overload{
                         [&](const base::musicDevice::controller::
                                 EventDestination::Note &) {
                            signals()
                                .ControllerEventRouter__connectedNotes2Notes(
                                    from.uuid, from.eventId.widgetId,
                                    note.number, from.eventId.eventId,
                                    from.eventId.channelId, to.uuid,
                                    to.voiceIdx);
                         },
                         [&](const base::musicDevice::controller::
                                 EventDestination::Parameter &parameter) {
                            signals()
                                .ControllerEventRouter__connectedNotes2Parameter(
                                    from.uuid, from.eventId.widgetId,
                                    note.number, from.eventId.eventId,
                                    from.eventId.channelId, to.uuid,
                                    to.voiceIdx, parameter.id,
                                    static_cast<int>(
                                        ::capnzero::MidiEmRt::
                                            SDParameterDestination::PARAMETER));
                         },
                         [](auto &&) {
                            spdlog::error("Unhandled path in "
                                          "controller-event-connection");
                         }},
                     to.endpoint);
              },
          },
          from.eventId.widgetCoord);
   });
   rCtrlEventRouter.onGotErased([this](const base::musicDevice::controller::
                                           EventIdExt &from) {
      mpark::visit(
          util::overload{
              [](const mpark::monostate
                     &) { /* TODO */
                          spdlog::error(
                              "Unhandled path in controller-event-connection");
              },
              [&](const base::musicDevice::controller::WidgetCoord
                      &widgetCoord) {
                 signals().ControllerEventRouter__erasedConnectionForWidget(
                     from.uuid, from.eventId.widgetId, widgetCoord.col,
                     widgetCoord.row, from.eventId.eventId,
                     from.eventId.channelId);
              },
              [&](const base::musicDevice::controller::Note &note) {
                 signals().ControllerEventRouter__erasedConnectionForNotes(
                     from.uuid, from.eventId.widgetId, note.number,
                     from.eventId.eventId, from.eventId.channelId);
              },
          },
          from.eventId.widgetCoord);
   });

   rParameterSceneContainer.onSceneNameChanged(
       [this](int sceneIdx, const std::string &name) {
          signals().ParameterScene__sceneNameChanged(sceneIdx, name);
       });
   rParameterSceneContainer.onSceneIntensityChanged(
       [this](int sceneIdx, float intensity) {
          signals().ParameterScene__sceneIntensityChanged(sceneIdx, intensity);
       });
   rParameterSceneContainer.onModifierEndValueChanged(
       [this](int sceneIdx,
              const base::musicDevice::sound::ParameterCoordinate &paramCoord,
              float value) {
          signals().ParameterScene__modifierEndValueChanged(
              sceneIdx, paramCoord.uuid, paramCoord.voiceIdx,
              paramCoord.parameterIdx,
              static_cast<::capnzero::MidiEmRt::SDParameterDestination>(
                  paramCoord.parameterPart),
              value);
       });
   rParameterSceneContainer.onModifierRemoved(
       [this](int sceneIdx,
              const base::musicDevice::sound::ParameterCoordinate &paramCoord) {
          signals().ParameterScene__modifierRemoved(
              sceneIdx, paramCoord.uuid, paramCoord.voiceIdx,
              paramCoord.parameterIdx,
              static_cast<::capnzero::MidiEmRt::SDParameterDestination>(
                  paramCoord.parameterPart));
       });

   rTracks.onTrackAdded([this](util::Identifiable::UUIDView uuid,
                               std::string_view name, int position) {
      signals().Session__trackAdded(
          *reinterpret_cast<const util::Identifiable::UUID *>(uuid.data()),
          std::string(name));
   });
   rTracks.onTrackRemoved([this](util::Identifiable::UUIDView uuid) {
      signals().Session__trackRemoved(
          *reinterpret_cast<const util::Identifiable::UUID *>(uuid.data()));
   });
   rTracks.onTrackDuplicated(
       [this](util::Identifiable::UUIDView uuidOriginal,
              util::Identifiable::UUIDView uuidDuplicate) {
          signals().Session__trackDuplicated(
              *reinterpret_cast<const util::Identifiable::UUID *>(
                  uuidOriginal.data()),
              *reinterpret_cast<const util::Identifiable::UUID *>(
                  uuidDuplicate.data()));
       });
   rTracks.onTrackMoved([this](util::Identifiable::UUIDView uuid, int dest) {
      signals().Session__trackMoved(
          *reinterpret_cast<const util::Identifiable::UUID *>(uuid.data()),
          dest);
   });
   rTracks.onTrackMuted([this](util::Identifiable::UUIDView uuid, bool muted) {
      signals().Session__trackMuted(
          *reinterpret_cast<const util::Identifiable::UUID *>(uuid.data()),
          muted);
   });

   rTracks.onTrackNameChanged(
       [this](util::Identifiable::UUIDView uuid, std::string_view name) {
          signals().Session__trackNameChanged(
              *reinterpret_cast<const util::Identifiable::UUID *>(uuid.data()),
              std::string(name));
       });
   rTracks.onTrackClipCreated([this, &rTracks](
                                  util::Identifiable::UUIDView uuid, int row) {
      std::string clipName;
      rTracks.withClip(
          uuid, row, [&clipName](const auto &clip) { clipName = clip.name(); });
      signals().Session__clipAdded(
          *reinterpret_cast<const util::Identifiable::UUID *>(uuid.data()), row,
          clipName);
   });
   rTracks.onTrackClipDeleted(
       [this](util::Identifiable::UUIDView uuid, int row) {
          signals().Session__clipRemoved(
              *reinterpret_cast<const util::Identifiable::UUID *>(uuid.data()),
              row);
       });
   rTracks.onTrackClipStartedChanged([this](util::Identifiable::UUIDView uuid,
                                            int row, bool started) {
      if (started)
      {
         signals().Session__clipStarted(
             *reinterpret_cast<const util::Identifiable::UUID *>(uuid.data()),
             row);
      }
      else
      {
         signals().Session__clipStopped(
             *reinterpret_cast<const util::Identifiable::UUID *>(uuid.data()),
             row);
      }
   });

   rTracks.onTrackClipNameChanged([this](util::Identifiable::UUIDView uuid,
                                         int row, std::string_view name) {
      signals().Session__clipRenamed(
          *reinterpret_cast<const util::Identifiable::UUID *>(uuid.data()), row,
          std::string(name));
   });
   rTracks.onTrackClipNoteAdded(
       [this](util::Identifiable::UUIDView uuid, int row,
              base::sequencer::NoteId noteId, base::sequencer::Beat startBeat,
              base::sequencer::Beat length, int note, float velocity) {
          signals().Session__clipNoteAdded(
              *reinterpret_cast<const util::Identifiable::UUID *>(uuid.data()),
              row, noteId, startBeat, length, note, velocity);
       });
   rTracks.onTrackClipNoteVelocityChanged(
       [this](util::Identifiable::UUIDView uuid, int row,
              base::sequencer::NoteId noteId, float velocity) {
          signals().Session__clipNoteVelocityChanged(
              *reinterpret_cast<const util::Identifiable::UUID *>(uuid.data()),
              row, noteId, velocity);
       });
   rTracks.onTrackClipNoteRemoved([this](util::Identifiable::UUIDView uuid,
                                         int row,
                                         base::sequencer::NoteId noteId) {
      signals().Session__clipNoteRemoved(
          *reinterpret_cast<const util::Identifiable::UUID *>(uuid.data()), row,
          noteId);
   });
   rTracks.onTrackClipAllNotesRemoved(
       [this](util::Identifiable::UUIDView uuid, int row) {
          signals().Session__clipAllNotesRemoved(
              *reinterpret_cast<const util::Identifiable::UUID *>(uuid.data()),
              row);
       });
}