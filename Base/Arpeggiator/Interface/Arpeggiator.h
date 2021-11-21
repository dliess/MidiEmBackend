#ifndef BASE_ARPEGGIATOR_H
#define BASE_ARPEGGIATOR_H

#include "EnumReflect.h"
#include <memory>

namespace base::arp
{

DECLARE_ENUM(Algorithm, uint, Up, Down, UpDown, Random, RecvOrder, CustomSequence);

struct ArpeggiatorPrivate;

class Arpeggiator
{
public:
    Arpeggiator();
    ~Arpeggiator();
    Arpeggiator(const Arpeggiator&) = delete;
    Arpeggiator(Arpeggiator&&) = default;
    Arpeggiator& operator=(const Arpeggiator&) = delete;
    Arpeggiator& operator=(Arpeggiator&&) = default;

    void update() noexcept;
    void noteOn(float velocity) noexcept;
    void noteOff(float velocity) noexcept;
    void setOctave(int octave) noexcept;
    void setRange(int range) noexcept;
    void setGateFill(float gateFill) noexcept;
    void setAlgorithm(Algorithm algorithm) noexcept;
private:
    std::unique_ptr<ArpeggiatorPrivate> m_pImpl;
};

} // base::arp

#endif