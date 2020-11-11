#ifndef UTIL_ITC_ACTION_RECEIVER_H
#define UTIL_ITC_ACTION_RECEIVER_H

#include "itcAction.h"

namespace util::itc
{

class ActionReceiver
{
   static constexpr auto _is(...) -> std::false_type;
   template<typename Receiver, typename Tag, typename ...Args,
            typename = std::void_t< decltype( std::declval<Receiver>().action( std::declval<Tag>(), std::declval<Args>()... ) ) >
   >
   static constexpr auto _is(Receiver&&, Action<Tag, Args...>) -> std::true_type;

public:
   template<typename Receiver, typename ActionType>
   static constexpr bool is = decltype(_is(std::declval<Receiver>(), std::declval<ActionType>()))::value;
};

#define FWD(arg) std::forward<decltype(arg)>(arg)

template<typename Receiver, typename Tag, typename ...Args>
void invokeReceiver(Receiver&& receiver, const ActionData<Action<Tag, Args...>>& data)
{
  /* 
   auto f = [&receiver](auto&&... params){
      std::cout << "Here" << std::endl;
      FWD(receiver).action(Tag(), FWD(params)...);
   };
   std::apply(f, data.data);
   */
   std::apply([](auto&& actionReceiver, auto&&... params){
         FWD(actionReceiver).action(FWD(params)...);
      },
      std::tuple_cat(
         std::forward_as_tuple<Receiver>(receiver),
         std::tuple<Tag>(),
         data.data
      ) 
   );
}

} // namespace util::itc
#endif