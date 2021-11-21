#include "Arpeggiator.h"

using namespace base::arp;

Arpeggiator::Arpeggiator() = default;

void Arpeggiator::update() noexcept
{

}

void Arpeggiator::bypass(bool onOff) noexcept
{
    if(m_bypass != onOff)
    {
        m_bypass = onOff;
        emitBypassChanged(m_bypass);
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
