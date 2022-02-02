#ifndef BASE_ABLETON_LINK_WRAPPER_H
#define BASE_ABLETON_LINK_WRAPPER_H

#include <memory>
#include <tuple>
#include "CallbackSignal.h"

namespace ableton { class Link; }
namespace base::musicDevice { class TransportControl; };

namespace base
{

class AbletonLinkWrapper
{
public:
    AbletonLinkWrapper() = default;
    ~AbletonLinkWrapper(); // Dummy for unique_ptr forward decl
    AbletonLinkWrapper(const AbletonLinkWrapper&) = delete;
    AbletonLinkWrapper& operator=(const AbletonLinkWrapper&) = delete;
    AbletonLinkWrapper(AbletonLinkWrapper&&) noexcept = default;
    AbletonLinkWrapper& operator=(AbletonLinkWrapper&&) noexcept = default;
    void enable(bool enable);
    [[nodiscard]] bool isEnabled() const;

    void reactOnTransport(bool react) noexcept;
    [[nodiscard]] bool reactsOnTransport() const noexcept;

    void setTempo(double bpm);

    std::pair<double, double> snapshot();
    void retriggerCallbacks();
 
    CB_SIGNAL(EnabledChanged, bool);
    CB_SIGNAL(ReactsOnTransportChanged, bool);
    CB_SIGNAL(StartStopChanged, bool);
    CB_SIGNAL(NumPeersChanged, bool);
private:
    std::unique_ptr<ableton::Link> m_pAbletonLink;
    bool m_reactsOnTransport{false};
};

} // namespace base

#endif // BASE_ABLETON_LINK_WRAPPER_H