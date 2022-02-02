namespace base
{
template <typename CB> void AbletonLinkWrapper::registerTransportControlCb(CB&& cb)
{
    m_pAbletonLink->setStartStopCallback(std::forward<CB>(cb));
}

template <typename CB> void AbletonLinkWrapper::registerNumPeersChangedCb(CB&& cb)
{
    m_pAbletonLink->setNumPeersCallback(std::forward<CB>(cb));
}

}   // namespace base