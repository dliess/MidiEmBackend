#include "Arpeggiator.h"

#include "ArpSequenceFactory.h"
#include "NoteContainer.h"

namespace base::arp
{
struct ArpeggiatorPrivate
{
   ArpeggiatorPrivate();
   bool m_bypass{true};
   float m_gateFill{0.5};
   int m_stepLength{240};
   NoteContainer m_incomingNoteBuffer;
   ArpSequenceFactory m_arpSequenceFactory;

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
    m_arpSequenceFactory(m_incomingNoteBuffer)
{
    m_arpSequenceFactory.onAlgorithmChanged([this](const Algorithm& algorithm){
        emitAlgorithmChanged(algorithm);
    });
    m_arpSequenceFactory.onRangeChanged([this](int range){
        emitRangeChanged(range);
    });
    m_arpSequenceFactory.onRangeTypeChanged([this](const RangeType& rangeType){
        emitRangeTypeChanged(rangeType);
    });
}

Arpeggiator::Arpeggiator() : 
    m_pImpl(std::make_unique<ArpeggiatorPrivate>())
{
}

Arpeggiator::~Arpeggiator() = default;

void Arpeggiator::update() noexcept
{
   m_pImpl->m_arpSequenceFactory.createIfDirty();
}

void Arpeggiator::bypass(bool onOff) noexcept
{
   if (m_pImpl->m_bypass != onOff)
   {
      m_pImpl->m_bypass = onOff;
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
   if (m_pImpl->m_gateFill != gateFill)
   {
      m_pImpl->m_gateFill = gateFill;
      m_pImpl->emitGateFillChanged(m_pImpl->m_gateFill);
   }
}

void Arpeggiator::setStepLength(int stepLength) noexcept
{
   if (m_pImpl->m_stepLength != stepLength)
   {
      m_pImpl->m_stepLength = stepLength;
      m_pImpl->emitStepLengthChanged(m_pImpl->m_stepLength);
   }
}

void Arpeggiator::setAlgorithm(Algorithm algorithm) noexcept
{
   m_pImpl->m_arpSequenceFactory.setAlgorithm(algorithm);
}
