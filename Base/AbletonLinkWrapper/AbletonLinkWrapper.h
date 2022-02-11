#ifndef BASE_ABLETON_LINK_WRAPPER_H
#define BASE_ABLETON_LINK_WRAPPER_H

#include <memory>
#include <tuple>
#include <chrono>

#include "CallbackSignal.h"

namespace ableton
{
class Link;
}
namespace base::musicDevice
{
class TransportControl;
};

namespace base
{
class AbletonLinkWrapper
{
public:
   AbletonLinkWrapper();
   ~AbletonLinkWrapper();   // Dummy for unique_ptr forward decl
   AbletonLinkWrapper(const AbletonLinkWrapper&) = delete;
   AbletonLinkWrapper& operator=(const AbletonLinkWrapper&) = delete;
   AbletonLinkWrapper(AbletonLinkWrapper&&) noexcept        = default;
   AbletonLinkWrapper& operator=(AbletonLinkWrapper&&) noexcept = default;
   void enable(bool enable);
   [[nodiscard]] bool isEnabled() const;

   void reactOnTransport(bool react) noexcept;
   [[nodiscard]] bool reactsOnTransport() const noexcept;

   void setTempo(double bpm);

   std::tuple<double, double, std::chrono::microseconds> snapshot();
   void retriggerCallbacks();

   void checkNumPeers();

   CB_SIGNAL(EnabledChanged, bool);
   CB_SIGNAL(ReactsOnTransportChanged, bool);
   CB_SIGNAL(StartStopChanged, bool);
   CB_SIGNAL(NumPeersChanged, size_t);

   class Offset 
   {
   public:
      void setOffsetSetupTempo(double bpm) noexcept { m_offsetSetupBpm = bpm; };
      void calcOffsets(std::chrono::microseconds timeDiff, double mainBpm);
      double offsetBeats() const noexcept { return m_offsetBeats; }
      std::chrono::microseconds offsetUs() const noexcept { return m_offsetUs; }
      void retriggerCallbacks();
      CB_SIGNAL(OffsetSetupBpmChanged, double);
      CB_SIGNAL(OffsetBeatsChanged, double);
      CB_SIGNAL(OffsetUsChanged, std::chrono::microseconds);
   private:
      double m_offsetSetupBpm{0.0};
      double m_offsetBeats{0.0};
      std::chrono::microseconds m_offsetUs;
   };
   Offset offset;
private:
   std::unique_ptr<ableton::Link> m_pAbletonLink;
   bool m_reactsOnTransport{false};
   size_t m_numPeers{0};
};

}   // namespace base

#endif   // BASE_ABLETON_LINK_WRAPPER_H