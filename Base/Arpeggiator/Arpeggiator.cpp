#include "Arpeggiator.h"

namespace base::arp
{
struct ArpeggiatorPrivate
{
 int i;
};
} // namespace base::arp

using namespace base::arp;

Arpeggiator::Arpeggiator() :
    m_pImpl(std::make_unique<ArpeggiatorPrivate>())
{}

Arpeggiator::~Arpeggiator() = default;

void Arpeggiator::update() noexcept
{

}

void Arpeggiator::noteOn(float velocity) noexcept
{

}

void Arpeggiator::noteOff(float velocity) noexcept
{

}

void Arpeggiator::setOctave(int octave) noexcept
{

}

void Arpeggiator::setRange(int range) noexcept
{

}

void Arpeggiator::setGateFill(float gateFill) noexcept
{

}

void Arpeggiator::setAlgorithm(Algorithm algorithm) noexcept
{
    
}
