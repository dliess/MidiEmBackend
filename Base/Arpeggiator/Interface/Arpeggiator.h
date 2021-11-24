#ifndef BASE_ARPEGGIATOR_H
#define BASE_ARPEGGIATOR_H

#include <memory>

#include "CallbackSignal.h"
#include "ArpTypes.h"

namespace base::arp
{

struct ArpeggiatorPrivate;
class Arpeggiator
{
public:
   Arpeggiator();
   ~Arpeggiator();
   Arpeggiator(const Arpeggiator&) = delete;
   Arpeggiator(Arpeggiator&&);
   Arpeggiator& operator=(const Arpeggiator&) = delete;
   Arpeggiator& operator=(Arpeggiator&&);

   void update() noexcept;
   void bypass(bool onOff) noexcept;
   void noteOn(int note, float velocity) noexcept;
   void noteOff(int note, float velocity) noexcept;
   void setRange(RangeType rangeType, int value) noexcept;
   void setGateFill(float gateFill) noexcept;
   void setStepLength(int beatTicks) noexcept;
   void setAlgorithm(Algorithm algorithm) noexcept;
   void setHoldNotes(bool on) noexcept;
   void setFeedMode(FeedMode feedMode) noexcept;
   void seqInsertPause() noexcept;
   void seqRemoveLastNote() noexcept;

   [[nodiscard]] bool getBypass() const noexcept;
   [[nodiscard]] RangeType getRangeType() const noexcept;
   [[nodiscard]] int getRange() const noexcept;
   [[nodiscard]] float getGateFill() const noexcept;
   [[nodiscard]] int getStepLength() const noexcept;
   [[nodiscard]] Algorithm getAlgorithm() const noexcept;
   [[nodiscard]] bool getHoldNotes() const noexcept;
   [[nodiscard]] FeedMode getFeedMode() const noexcept;
   [[nodiscard]] int getSeqSize() const noexcept;

   CB_SIGNAL_IF(BypassChanged, bool);
   CB_SIGNAL_IF(NoteOn, int, float);
   CB_SIGNAL_IF(NoteOff, int, float);
   CB_SIGNAL_IF(RangeTypeChanged, RangeType);
   CB_SIGNAL_IF(RangeChanged, int);
   CB_SIGNAL_IF(GateFillChanged, float);
   CB_SIGNAL_IF(StepLengthChanged, int);
   CB_SIGNAL_IF(AlgorithmChanged, Algorithm);
   CB_SIGNAL_IF(HoldNotesChanged, bool);
   CB_SIGNAL_IF(FeedModeChanged, FeedMode);
   CB_SIGNAL_IF(SeqSizeChanged, int);

private:
    std::unique_ptr<ArpeggiatorPrivate> m_pImpl;
};

}   // namespace base::arp

#endif