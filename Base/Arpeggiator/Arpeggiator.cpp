#include "Arpeggiator.h"

namespace base::arp
{
struct ArpeggiatorPrivate
{
   bool m_bypass{true};
   RangeType m_rangeType{RangeType::Octave};
   int m_range{1};
   float m_gateFill{0.5};
   Algorithm m_algorithm{Algorithm::Up};
   //NoteContainer m_incomingNoteBuffer;

   CB_SIGNAL_PRIVATE(Arpeggiator, BypassChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, NoteOn);
   CB_SIGNAL_PRIVATE(Arpeggiator, NoteOff);
   CB_SIGNAL_PRIVATE(Arpeggiator, RangeTypeChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, RangeChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, GateFillChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, AlgorithmChanged);
};

} // namespace base::arp

using namespace base::arp;

CB_SIGNAL_IMPL(Arpeggiator, BypassChanged);
CB_SIGNAL_IMPL(Arpeggiator, NoteOn);
CB_SIGNAL_IMPL(Arpeggiator, NoteOff);
CB_SIGNAL_IMPL(Arpeggiator, RangeTypeChanged);
CB_SIGNAL_IMPL(Arpeggiator, RangeChanged);
CB_SIGNAL_IMPL(Arpeggiator, GateFillChanged);
CB_SIGNAL_IMPL(Arpeggiator, AlgorithmChanged);

Arpeggiator::Arpeggiator() :
    m_pImpl(std::make_unique<ArpeggiatorPrivate>())
{}

Arpeggiator::~Arpeggiator() = default;

void Arpeggiator::update() noexcept
{

}

void Arpeggiator::bypass(bool onOff) noexcept
{
    if(m_pImpl->m_bypass != onOff)
    {
        m_pImpl->m_bypass = onOff;
        m_pImpl->emitBypassChanged(m_pImpl->m_bypass);
    }
}

void Arpeggiator::noteOn(float velocity) noexcept
{

}

void Arpeggiator::noteOff(float velocity) noexcept
{

}

void Arpeggiator::setRange(RangeType rangeType, int value) noexcept
{

}

void Arpeggiator::setGateFill(float gateFill) noexcept
{

}

void Arpeggiator::setAlgorithm(Algorithm algorithm) noexcept
{
    
}
