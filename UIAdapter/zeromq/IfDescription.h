#ifndef UIADAPTER_ZEROMQ_IFDESCRIPTION_H
#define UIADAPTER_ZEROMQ_IFDESCRIPTION_H

namespace uiadapter::zeromq {

enum class ReqCat { Instruments, MusicDevices };

template <ReqCat T> struct Request {};

template <> struct Request<ReqCat::Instruments> {
  static constexpr auto categoryId = ReqCat::Instruments;
  enum class MethodId {
    triggerSendData,
    createKitInstrument,
    removeKitInstrument,
    createMelodicInstrument,
    removeMelodicInstrument,
    createKitInstrumentSound,
    removeKitInstrumentSound,
    addVoiceToKitInstrumentSound,
    removeVoiceFromInstrumentSound,
    addVoiceToMelodicInstrument,
    removeVoiceFromMelodicInstrument
  };
};

template <ReqCat T, typename Request<T>::MethodId methodId>
struct RequestParameters {};

template <>
struct RequestParameters<
    ReqCat::Instruments,
    Request<ReqCat::Instruments>::MethodId::triggerSendData> {
  using InputParam = void;
  using ReturnParam = void;
};

template <>
struct RequestParameters<
    ReqCat::Instruments,
    Request<ReqCat::Instruments>::MethodId::createKitInstrument> {
  using InputParam = struct{std::string name;};
  using ReturnParam = void;
};

// TODO: other specializations

} // namespace uiadapter::zeromq

#endif
