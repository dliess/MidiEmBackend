#include "ParameterSceneRpc.h"

#include "ParameterSceneContainer.h"

using namespace uiadapter::capnzero;

ParameterSceneRpc::ParameterSceneRpc(
    base::musicDevice::sound::ParameterSceneContainer&
        rParameterSceneContainer) :
    m_rParameterSceneContainer(rParameterSceneContainer)
{
}

void ParameterSceneRpc::setSceneName(const ::capnzero::SpanCL<16>& sceneUuid,
                                     const ::capnzero::TextView& name)
{
   util::Identifiable::UUID uuid_;
   std::copy(sceneUuid.begin(), sceneUuid.end(), uuid_.begin());
   m_rParameterSceneContainer.setSceneName(uuid_, std::string(name));
}

void ParameterSceneRpc::setSceneIntensity(
    const ::capnzero::SpanCL<16>& sceneUuid, ::capnzero::Float32 intensity)
{
   util::Identifiable::UUID uuid_;
   std::copy(sceneUuid.begin(), sceneUuid.end(), uuid_.begin());
   m_rParameterSceneContainer.setSceneIntensity(uuid_, intensity);
}

void ParameterSceneRpc::setModifierEndValue(
    const ::capnzero::SpanCL<16>& sceneUuid, const ::capnzero::SpanCL<16>& uuid,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterDestination parameterPart,
    ::capnzero::Float32 endValue)
{
   util::Identifiable::UUID sceneUuid_;
   util::Identifiable::UUID deviceUuid_;
   std::copy(sceneUuid.begin(), sceneUuid.end(), sceneUuid_.begin());
   std::copy(uuid.begin(), uuid.end(), deviceUuid_.begin());
   m_rParameterSceneContainer.setModifierEndValue(
       sceneUuid_,
       base::musicDevice::sound::ParameterCoordinate{
           deviceUuid_, voiceIdx, parameterIdx,
           static_cast<base::musicDevice::sound::ParameterPart>(parameterPart)},
       endValue);
}

void ParameterSceneRpc::removeModifier(
    const ::capnzero::SpanCL<16>& sceneUuid, const ::capnzero::SpanCL<16>& uuid,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterDestination parameterPart)
{
   util::Identifiable::UUID sceneUuid_;
   util::Identifiable::UUID deviceUuid_;
   std::copy(sceneUuid.begin(), sceneUuid.end(), sceneUuid_.begin());
   std::copy(uuid.begin(), uuid.end(), deviceUuid_.begin());
   m_rParameterSceneContainer.removeModifier(
       sceneUuid_,
       base::musicDevice::sound::ParameterCoordinate{
           deviceUuid_, voiceIdx, parameterIdx,
           static_cast<base::musicDevice::sound::ParameterPart>(
               parameterPart)});
}
