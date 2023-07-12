#ifndef PARAMETER_SCENE_RPC_H
#define PARAMETER_SCENE_RPC_H

#include "MidiEmRtParameterSceneRpcIf.h"

// clang-format off
namespace base::musicDevice::sound { class ParameterSceneContainer; }
namespace base::musicDevice { class ModifiersApplyer; }
// clang-format on
namespace uiadapter::capnzero
{
class ParameterSceneRpc : public ::capnzero::MidiEmRt::ParameterSceneRpcIf
{
public:
   ParameterSceneRpc(base::musicDevice::sound::ParameterSceneContainer&
                         rParameterSceneContainer,
                     base::musicDevice::ModifiersApplyer& rModifiersApplyer);
   void setSceneName(::capnzero::Int16 sceneIdx,
                     const ::capnzero::TextView& name) override;
   void setSceneIntensity(::capnzero::Int16 sceneIdx,
                          ::capnzero::Float32 intensity) override;
   void setModifierEndValue(::capnzero::Int16 sceneIdx,
                            const ::capnzero::SpanCL<16>& uuid,
                            ::capnzero::Int16 voiceIdx,
                            ::capnzero::Int16 parameterIdx,
                            ::capnzero::MidiEmRt::SDParameterAttr parameterAttr,
                            ::capnzero::Float32 endValue) override;
   void incrementModifierEndValue(
       ::capnzero::Int16 sceneIdx, const ::capnzero::SpanCL<16>& uuid,
       ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmRt::SDParameterAttr parameterAttr,
       ::capnzero::Float32 increment) override;
   void removeModifier(
       ::capnzero::Int16 sceneIdx, const ::capnzero::SpanCL<16>& uuid,
       ::capnzero::Int16 voiceIdx, ::capnzero::Int16 parameterIdx,
       ::capnzero::MidiEmRt::SDParameterAttr parameterAttr) override;

private:
   base::musicDevice::sound::ParameterSceneContainer&
       m_rParameterSceneContainer;
   base::musicDevice::ModifiersApplyer& m_rModifiersApplyer;
};

}   // namespace uiadapter::capnzero

#endif   // PARAMETER_SCENE_RPC_H