#include "ArpSequenceFactory.h"

using namespace base::arp;

ArpSequenceFactory::ArpSequenceFactory(NoteContainer& rIncomingNoteBuffer) noexcept :
    m_rIncomingNoteBuffer(rIncomingNoteBuffer)
{
    m_rIncomingNoteBuffer.onChanged([this](){
        m_dirty = true;
    });
}

void ArpSequenceFactory::createIfDirty() noexcept
{
    m_dirty = false;
}

void ArpSequenceFactory::setRange(RangeType rangeType, int value) noexcept
{
    if(m_rangeType != rangeType)
    {
        m_rangeType = rangeType;
        m_dirty = true;
        emitRangeTypeChanged(m_rangeType);
    }
    if(m_range != value)
    {
        m_range = value;
        m_dirty = true;
        emitRangeChanged(m_range);
    }
}

void ArpSequenceFactory::setAlgorithm(Algorithm algorithm) noexcept
{
    if(m_algorithm != algorithm)
    {
        m_algorithm = algorithm;
        m_dirty = true;
        emitAlgorithmChanged(m_algorithm);
    }
}
