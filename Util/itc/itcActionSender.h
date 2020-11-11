#ifndef ITC_ACTION_SENDER_H
#define ITC_ACTION_SENDER_H

#include <tuple>

#include "itcQueue.h"

namespace util::itc
{
template<typename... Receivers>
class ActionSender
{
public:
   ActionSender(Queue& queue, Receivers&... receivers) noexcept;
   template<typename Tag, typename... Args>
   void push(ActionData<Action<Tag, Args...>> data) noexcept;
   template<typename Tag, typename... Args>
   void push(Tag tag, Args... args) noexcept;

private:
   Queue& m_queue;
   std::tuple<Receivers&...> m_receivers;

   /*
      template<typename T, typename... Ts>
      constexpr static bool is_one_of_v = (std::is_same_v<T, Ts> || ...);

      template<typename Receivers>
      auto receiver() noexcept -> std::enable_if_t<is_one_of_v<Receivers,
      Receivers...>, Receivers&>
      {
        return std::get<Receivers&>(m_receivers);
      }
   */
};

} // namespace util::itc

#include "itcActionSender.tcc"

#endif