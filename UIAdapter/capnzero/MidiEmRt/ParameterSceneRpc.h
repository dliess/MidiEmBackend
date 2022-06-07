#ifndef PARAMETER_SCENE_RPC_H
#define PARAMETER_SCENE_RPC_H

#include "MidiEmRtParameterSceneRpcIf.h"

namespace base::musicDevice::sound
{
	class ParameterSceneContainer;
}

namespace uiadapter::capnzero
{
class ParameterSceneRpc : public ::capnzero::MidiEmRt::ParameterSceneRpcIf
{
public:
   ParameterSceneRpc(base::musicDevice::sound::ParameterSceneContainer&
                         rParameterSceneContainer);
   void setSceneName(const ::capnzero::SpanCL<16>& sceneUuid,
                     const ::capnzero::TextView& name) override;
   void setSceneIntensity(const ::capnzero::SpanCL<16>& sceneUuid,
                          ::capnzero::Float32 intensity) override;
   void setModifierEndValue(
       const ::capnzero::SpanCL<16>& sceneUuid,
       const ::capnzero::SpanCL<16>& uuid, ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmRt::SDParameterDestination parameterPart,
       ::capnzero::Float32 endValue) override;
   void removeModifier(
       const ::capnzero::SpanCL<16>& sceneUuid, const ::capnzero::SpanCL<16>& uuid,
       ::capnzero::Int16 voiceIdx,
       ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmRt::SDParameterDestination parameterPart) override;

private:
   base::musicDevice::sound::ParameterSceneContainer&
       m_rParameterSceneContainer;
};

}   // namespace uiadapter::capnzero

#endif   // PARAMETER_SCENE_RPC_H