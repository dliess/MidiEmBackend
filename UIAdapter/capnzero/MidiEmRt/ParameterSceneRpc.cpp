#include "ParameterSceneRpc.h"

#include "ParameterSceneContainer.h"

using namespace uiadapter::capnzero;

ParameterSceneRpc::ParameterSceneRpc(
    base::musicDevice::sound::ParameterSceneContainer&
        rParameterSceneContainer) :
    m_rParameterSceneContainer(rParameterSceneContainer)
{
}

void ParameterSceneRpc::setSceneName(::capnzero::Int16 sceneIdx,
                                     const ::capnzero::TextView& name)
{
   m_rParameterSceneContainer.setSceneName(sceneIdx, std::string(name));
}

void ParameterSceneRpc::setSceneIntensity(::capnzero::Int16 sceneIdx,
                                          ::capnzero::Float32 intensity)
{
   if (intensity < 0 || 1.0 < intensity)
   {
      spdlog::error("intensity out of range");
      return;
   }
   m_rParameterSceneContainer.setSceneIntensity(sceneIdx, intensity);
}

void ParameterSceneRpc::setModifierEndValue(
    ::capnzero::Int16 sceneIdx, const ::capnzero::SpanCL<16>& uuid,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterDestination parameterPart,
    ::capnzero::Float32 endValue)
{
   util::Identifiable::UUID deviceUuid_;
   std::copy(uuid.begin(), uuid.end(), deviceUuid_.begin());
   m_rParameterSceneContainer.setModifierEndValue(
       sceneIdx,
       base::musicDevice::sound::ParameterCoordinate{
           deviceUuid_, voiceIdx, parameterIdx,
           static_cast<base::musicDevice::sound::ParameterPart>(parameterPart)},
       endValue);
}

void ParameterSceneRpc::incrementModifierEndValue(
    ::capnzero::Int16 sceneIdx, const ::capnzero::SpanCL<16>& uuid,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterDestination parameterPart,
    ::capnzero::Float32 increment)
{
   util::Identifiable::UUID deviceUuid_;
   std::copy(uuid.begin(), uuid.end(), deviceUuid_.begin());
   m_rParameterSceneContainer.incrementModifierEndValue(
       sceneIdx,
       base::musicDevice::sound::ParameterCoordinate{
           deviceUuid_, voiceIdx, parameterIdx,
           static_cast<base::musicDevice::sound::ParameterPart>(parameterPart)},
       increment);
}

void ParameterSceneRpc::removeModifier(
    ::capnzero::Int16 sceneIdx, const ::capnzero::SpanCL<16>& uuid,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterDestination parameterPart)
{
   util::Identifiable::UUID deviceUuid_;
   std::copy(uuid.begin(), uuid.end(), deviceUuid_.begin());
   m_rParameterSceneContainer.removeModifier(
       sceneIdx, base::musicDevice::sound::ParameterCoordinate{
                     deviceUuid_, voiceIdx, parameterIdx,
                     static_cast<base::musicDevice::sound::ParameterPart>(
                         parameterPart)});
}
