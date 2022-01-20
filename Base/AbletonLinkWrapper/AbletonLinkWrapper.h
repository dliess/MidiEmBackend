#ifndef BASE_ABLETON_LINK_WRAPPER_H
#define BASE_ABLETON_LINK_WRAPPER_H

#include <memory>
#include "CallbackSignal.h"

namespace ableton { class Link; }
namespace base::musicDevice { class TransportControl; };

namespace base
{

class AbletonLinkWrapper
{
public:
    AbletonLinkWrapper(base::musicDevice::TransportControl& rTransportControl);
    ~AbletonLinkWrapper(); // Dummy for unique_ptr forward decl
    void enable(bool enable);
    [[nodiscard]] bool isEnabled() const;

    void reactOnTransport(bool react) noexcept;
    [[nodiscard]] bool reactsOnTransport() const noexcept;

    void update();
    void retriggerCallbacks();
 
    CB_SIGNAL(EnabledChanged, bool);
    CB_SIGNAL(ReactsOnTransportChanged, bool);
private:
    std::unique_ptr<ableton::Link> m_pAbletonLink;
    base::musicDevice::TransportControl& m_rTransportControl;
    bool m_reactsOnTransport{false};
};

} // namespace base

#endif // BASE_ABLETON_LINK_WRAPPER_H