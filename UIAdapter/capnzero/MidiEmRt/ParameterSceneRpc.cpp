#include "ParameterSceneRpc.h"

#include "ModifiersApplyer.h"

using namespace uiadapter::capnzero;

ParameterSceneRpc::ParameterSceneRpc(
    base::parameterScenes::ModifiersApplyer& rModifiersApplyer) :
    m_rModifiersApplyer(rModifiersApplyer)
{
}

void ParameterSceneRpc::setSceneName(::capnzero::Int16 sceneIdx,
                                     const ::capnzero::TextView& name)
{
   m_rModifiersApplyer.setSceneName(sceneIdx, std::string(name));
}

void ParameterSceneRpc::setSceneIntensity(::capnzero::Int16 sceneIdx,
                                          ::capnzero::Float32 intensity)
{
   if (intensity < 0 || 1.0 < intensity)
   {
      spdlog::error("intensity out of range");
      return;
   }
   m_rModifiersApplyer.setSceneIntensity(sceneIdx, intensity);
}

void ParameterSceneRpc::setModifierEndValue(
    ::capnzero::Int16 sceneIdx, const ::capnzero::SpanCL<16>& uuid,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterAttr parameterAttr,
    ::capnzero::Float32 endValue)
{
   util::Identifiable::UUID deviceUuid_;
   std::copy(uuid.begin(), uuid.end(), deviceUuid_.begin());
   m_rModifiersApplyer.setModifierEndValue(
       sceneIdx,
       base::musicDevice::sound::ParameterCoordinate{
           deviceUuid_, voiceIdx, parameterIdx,
           static_cast<base::musicDevice::sound::ParameterAttr>(parameterAttr)},
       endValue);
}

void ParameterSceneRpc::incrementModifierEndValue(
    ::capnzero::Int16 sceneIdx, const ::capnzero::SpanCL<16>& uuid,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterAttr parameterAttr,
    ::capnzero::Float32 increment)
{
   util::Identifiable::UUID deviceUuid_;
   std::copy(uuid.begin(), uuid.end(), deviceUuid_.begin());
   m_rModifiersApplyer.incrementModifierEndValue(
       sceneIdx,
       base::musicDevice::sound::ParameterCoordinate{
           deviceUuid_, voiceIdx, parameterIdx,
           static_cast<base::musicDevice::sound::ParameterAttr>(parameterAttr)},
       increment);
}

void ParameterSceneRpc::removeModifier(
    ::capnzero::Int16 sceneIdx, const ::capnzero::SpanCL<16>& uuid,
    ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
    ::capnzero::MidiEmRt::SDParameterAttr parameterAttr)
{
   util::Identifiable::UUID deviceUuid_;
   std::copy(uuid.begin(), uuid.end(), deviceUuid_.begin());
   m_rModifiersApplyer.removeModifier(
       sceneIdx, base::musicDevice::sound::ParameterCoordinate{
                     deviceUuid_, voiceIdx, parameterIdx,
                     static_cast<base::musicDevice::sound::ParameterAttr>(
                         parameterAttr)});
}
