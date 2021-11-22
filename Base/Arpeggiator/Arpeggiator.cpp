#include "Arpeggiator.h"

#include "ArpSequence.h"
#include "ArpSequencePlayer.h"
#include "ArpSequenceFactory.h"
#include "NoteContainer.h"

namespace base::arp
{
struct ArpeggiatorPrivate
{
   ArpeggiatorPrivate();
   bool m_bypass{true};
   NoteContainer m_incomingNoteBuffer;
   ArpSequenceFactory m_arpSequenceFactory;
   ArpSequence m_arpSequence;
   ArpSequencePlayer m_arpSequencePlayer;

   CB_SIGNAL_PRIVATE(Arpeggiator, BypassChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, NoteOn);
   CB_SIGNAL_PRIVATE(Arpeggiator, NoteOff);
   CB_SIGNAL_PRIVATE(Arpeggiator, RangeTypeChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, RangeChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, GateFillChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, StepLengthChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, AlgorithmChanged);
};

}   // namespace base::arp

using namespace base::arp;

CB_SIGNAL_IMPL(Arpeggiator, BypassChanged);
CB_SIGNAL_IMPL(Arpeggiator, NoteOn);
CB_SIGNAL_IMPL(Arpeggiator, NoteOff);
CB_SIGNAL_IMPL(Arpeggiator, RangeTypeChanged);
CB_SIGNAL_IMPL(Arpeggiator, RangeChanged);
CB_SIGNAL_IMPL(Arpeggiator, GateFillChanged);
CB_SIGNAL_IMPL(Arpeggiator, StepLengthChanged);
CB_SIGNAL_IMPL(Arpeggiator, AlgorithmChanged);

ArpeggiatorPrivate::ArpeggiatorPrivate() :
    m_arpSequenceFactory(m_incomingNoteBuffer, m_arpSequence),
    m_arpSequencePlayer(m_arpSequence)
{
   m_arpSequenceFactory.onAlgorithmChanged(
       [this](const Algorithm& algorithm) { emitAlgorithmChanged(algorithm); });
   m_arpSequenceFactory.onRangeChanged(
       [this](int range) { emitRangeChanged(range); });
   m_arpSequenceFactory.onRangeTypeChanged(
       [this](const RangeType& rangeType) { emitRangeTypeChanged(rangeType); });

   m_arpSequencePlayer.onNoteOn(
       [this](int note, float velocity) { emitNoteOn(note, velocity); });
   m_arpSequencePlayer.onNoteOff(
       [this](int note, float velocity) { emitNoteOff(note, velocity); });
   m_arpSequencePlayer.onGateFillChanged(
       [this](float gateFill) { emitGateFillChanged(gateFill); });
   m_arpSequencePlayer.onStepLengthChanged(
       [this](int stepLength) { emitStepLengthChanged(stepLength); });
   m_arpSequencePlayer.onTurnOver([this](){
      m_arpSequenceFactory.createIfDirty();
   });
}

Arpeggiator::Arpeggiator() : m_pImpl(std::make_unique<ArpeggiatorPrivate>()) {}

Arpeggiator::~Arpeggiator() = default;
Arpeggiator::Arpeggiator(Arpeggiator&&) = default;
Arpeggiator& Arpeggiator::operator=(Arpeggiator&&) = default;

void Arpeggiator::update() noexcept
{
   if(!m_pImpl->m_bypass)
   {
      m_pImpl->m_arpSequencePlayer.update();
   }
}

void Arpeggiator::bypass(bool onOff) noexcept
{
   if (m_pImpl->m_bypass != onOff)
   {
      m_pImpl->m_bypass = onOff;
      if(m_pImpl->m_bypass)
      {
         m_pImpl->m_arpSequencePlayer.stop();
      }
      else
      {
         m_pImpl->m_arpSequencePlayer.start();
      }
      m_pImpl->emitBypassChanged(m_pImpl->m_bypass);
   }
}

void Arpeggiator::noteOn(int note, float velocity) noexcept
{
   if (m_pImpl->m_bypass)
   {
      m_pImpl->emitNoteOn(note, velocity);
   }
   else
   {
      m_pImpl->m_incomingNoteBuffer.addNote(note, velocity);
   }
}

void Arpeggiator::noteOff(int note, float velocity) noexcept
{
   if (m_pImpl->m_bypass)
   {
      m_pImpl->emitNoteOff(note, velocity);
   }
   else
   {
      m_pImpl->m_incomingNoteBuffer.removeNote(note);
   }
}

void Arpeggiator::setRange(RangeType rangeType, int value) noexcept
{
   m_pImpl->m_arpSequenceFactory.setRange(rangeType, value);
}

void Arpeggiator::setGateFill(float gateFill) noexcept
{
   m_pImpl->m_arpSequencePlayer.setGateFill(gateFill);
}

void Arpeggiator::setStepLength(int stepLength) noexcept
{
   m_pImpl->m_arpSequencePlayer.setStepLength(stepLength);
}

void Arpeggiator::setAlgorithm(Algorithm algorithm) noexcept
{
   m_pImpl->m_arpSequenceFactory.setAlgorithm(algorithm);
}
