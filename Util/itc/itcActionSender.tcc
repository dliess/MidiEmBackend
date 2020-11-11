#include "itcActionSender.h"

namespace util::itc
{
template<typename... Receivers>
ActionSender<Receivers...>::ActionSender(Queue& queue,
                                        Receivers&... receivers) noexcept :
   m_queue(queue),
   m_receivers(receivers...)
{
}

template<typename... Receivers>
template<typename Tag, typename... Args>
void ActionSender<Receivers...>::push(ActionData<Action<Tag, Args...>> data) noexcept
{
   (m_queue.tryPush(std::get<Receivers&>(m_receivers), data), ...);
}

template<typename... Receivers>
template<typename Tag, typename... Args>
void ActionSender<Receivers...>::push(Tag tag, Args... args) noexcept
{
  // push(ActionData<Action<Tag, Args...>>({ std::make_tuple(args...) }));
   push(ActionData<Action<Tag, Args...>>({ std::forward_as_tuple(args...) }));
}

} // namespace util::itc