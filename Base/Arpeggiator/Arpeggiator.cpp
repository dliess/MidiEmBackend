#include "Arpeggiator.h"

#include "ArpSequence.h"
#include "ArpSequenceFactory.h"
#include "ArpSequencePlayer.h"
#include "BaseSequence.h"
#include "NoteContainer.h"
#include "stack_mempool.h"

namespace base::arp
{
struct ArpeggiatorPrivate
{
   ArpeggiatorPrivate();
   util::StackMempool<8192> m_memoryPool;
   bool m_bypass{true};
   FeedMode m_feedMode{FeedMode::Control};
   NoteContainer m_incomingNoteBuffer;
   BaseSequence m_incomingSequenceBuffer;
   ArpSequence m_arpSequence;
   ArpSequenceFactory m_arpSequenceFactory;
   ArpSequencePlayer m_arpSequencePlayer;

   CB_SIGNAL_PRIVATE(Arpeggiator, BypassChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, NoteOn);
   CB_SIGNAL_PRIVATE(Arpeggiator, NoteOff);
   CB_SIGNAL_PRIVATE(Arpeggiator, RangeTypeChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, RangeChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, GateFillChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, StepLengthChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, AlgorithmChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, HoldNotesChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, FeedModeChanged);
   CB_SIGNAL_PRIVATE(Arpeggiator, SeqSizeChanged);
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
CB_SIGNAL_IMPL(Arpeggiator, HoldNotesChanged);
CB_SIGNAL_IMPL(Arpeggiator, FeedModeChanged);
CB_SIGNAL_IMPL(Arpeggiator, SeqSizeChanged);

ArpeggiatorPrivate::ArpeggiatorPrivate() :
    m_memoryPool("ArpeggiatorPrivate"),
    m_incomingNoteBuffer(m_memoryPool.pool()),
    m_incomingSequenceBuffer(m_memoryPool.pool()),
    m_arpSequence(m_memoryPool.pool()),
    m_arpSequenceFactory(m_memoryPool.pool(), m_incomingNoteBuffer,
                         m_incomingSequenceBuffer, m_arpSequence),
    m_arpSequencePlayer(m_arpSequence)
{
   m_incomingNoteBuffer.onChanged([this](size_t prevSize, size_t actSize) {
      if (!m_bypass)
      {
         if (prevSize == 0 && actSize > 0)
         {
            m_arpSequencePlayer.start();
         }
         else if (prevSize > 0 && actSize == 0)
         {
            m_arpSequencePlayer.stop();
         }
      }
   });
   m_incomingNoteBuffer.onHoldNotesChanged(
       [this](bool on) { emitHoldNotesChanged(on); });

   m_incomingSequenceBuffer.onSizeChanged([this](int size){
      emitSeqSizeChanged(size);
   });

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
   m_arpSequencePlayer.onTurnOver(
       [this]() { m_arpSequenceFactory.createIfDirty(); });
}

Arpeggiator::Arpeggiator() : m_pImpl(std::make_unique<ArpeggiatorPrivate>()) {}

Arpeggiator::~Arpeggiator()             = default;
Arpeggiator::Arpeggiator(Arpeggiator&&) = default;
Arpeggiator& Arpeggiator::operator=(Arpeggiator&&) = default;

void Arpeggiator::update() noexcept
{
   if (!m_pImpl->m_bypass)
   {
      m_pImpl->m_incomingNoteBuffer.update();
      m_pImpl->m_arpSequencePlayer.update();
   }
}

void Arpeggiator::bypass(bool onOff) noexcept
{
   if (m_pImpl->m_bypass != onOff)
   {
      m_pImpl->m_bypass = onOff;
      if (m_pImpl->m_bypass)
      {
         m_pImpl->m_arpSequencePlayer.stop();
      }
      else if (m_pImpl->m_incomingNoteBuffer.size())
      {
         m_pImpl->m_arpSequencePlayer.start();
      }
      m_pImpl->emitBypassChanged(m_pImpl->m_bypass);
   }
}

void Arpeggiator::noteOn(int note, float velocity) noexcept
{
   switch (m_pImpl->m_feedMode)
   {
      case FeedMode::Control:
      {
         if (m_pImpl->m_bypass)
         {
            m_pImpl->emitNoteOn(note, velocity);
         }
         m_pImpl->m_incomingNoteBuffer.addNote(note, velocity);
         break;
      }
      case FeedMode::Sequence:
      {
         if (m_pImpl->m_bypass)
         {
            m_pImpl->emitNoteOn(note, velocity);
         }
         else
         {
            if(m_pImpl->m_incomingNoteBuffer.size() == 0)
            {
               m_pImpl->emitNoteOn(note, velocity);
            }
            m_pImpl->m_incomingSequenceBuffer.addNote(note, velocity);
         }
         break;
      }
   }
}

void Arpeggiator::noteOff(int note, float velocity) noexcept
{
   switch (m_pImpl->m_feedMode)
   {
      case FeedMode::Control:
      {
         if (m_pImpl->m_bypass)
         {
            m_pImpl->emitNoteOff(note, velocity);
         }
         m_pImpl->m_incomingNoteBuffer.removeNote(note);
         break;
      }
      case FeedMode::Sequence:
      {
         if(m_pImpl->m_incomingNoteBuffer.size() == 0)
         {
            m_pImpl->emitNoteOff(note, velocity);
         }
         break;
      }
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

void Arpeggiator::setHoldNotes(bool on) noexcept
{
   m_pImpl->m_incomingNoteBuffer.setHoldNotes(on);
}

void Arpeggiator::setFeedMode(FeedMode feedMode) noexcept
{
   if (m_pImpl->m_feedMode != feedMode)
   {
      m_pImpl->m_feedMode = feedMode;
      switch (m_pImpl->m_feedMode)
      {
         case FeedMode::Control:
         {
            m_pImpl->m_arpSequenceFactory.updateSequence();
            break;
         }
         case FeedMode::Sequence:
         {
            m_pImpl->m_incomingNoteBuffer.noteOffAll();
            break;
         }
      }
      m_pImpl->m_incomingSequenceBuffer.clear();
      m_pImpl->emitFeedModeChanged(m_pImpl->m_feedMode);
   }
}

void Arpeggiator::seqInsertPause() noexcept
{
   m_pImpl->m_incomingSequenceBuffer.addPause();
}

void Arpeggiator::seqRemoveLastNote() noexcept
{
   m_pImpl->m_incomingSequenceBuffer.removeLastNote();
}

bool Arpeggiator::getBypass() const noexcept { return m_pImpl->m_bypass; }
RangeType Arpeggiator::getRangeType() const noexcept
{
   return m_pImpl->m_arpSequenceFactory.getRangeType();
}
int Arpeggiator::getRange() const noexcept
{
   return m_pImpl->m_arpSequenceFactory.getRange();
}
float Arpeggiator::getGateFill() const noexcept
{
   return m_pImpl->m_arpSequencePlayer.getGateFill();
}
int Arpeggiator::getStepLength() const noexcept
{
   return m_pImpl->m_arpSequencePlayer.getStepLength();
}
Algorithm Arpeggiator::getAlgorithm() const noexcept
{
   return m_pImpl->m_arpSequenceFactory.getAlgorithm();
}
bool Arpeggiator::getHoldNotes() const noexcept
{
   return m_pImpl->m_incomingNoteBuffer.getHoldNotes();
}
FeedMode Arpeggiator::getFeedMode() const noexcept
{
   return m_pImpl->m_feedMode;
}
int Arpeggiator::getSeqSize() const noexcept
{
   return m_pImpl->m_incomingSequenceBuffer.size();
}
