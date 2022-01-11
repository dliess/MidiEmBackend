#ifndef BASE_ABLETON_LINK_WRAPPER_H
#define BASE_ABLETON_LINK_WRAPPER_H

#include <memory>


namespace ableton { class Link; }

namespace base
{

namespace musicDevice { class TransportControl; }

class AbletonLinkWrapper
{
public:
    AbletonLinkWrapper(musicDevice::TransportControl& rTransportControl);
    ~AbletonLinkWrapper(); // Dummy for unique_ptr forward decl
    void enable(bool enable);
    [[nodiscard]] bool isEnabled() const;
    void update();
private:
    musicDevice::TransportControl& m_rTransportControl;
    std::unique_ptr<ableton::Link> m_pAbletonLink;
};

} // namespace base

#endif // BASE_ABLETON_LINK_WRAPPER_H