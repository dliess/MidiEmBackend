#ifndef ITC_ACTION_H
#define ITC_ACTION_H

#include <tuple>

namespace util::itc
{

// We need this, so the action data also carries Tag information
template<typename AtionType>
struct ActionData
{
   using ArgumentsTuple = typename AtionType::ArgumentsTuple;
   ArgumentsTuple data;
};

template<typename Tag, typename... Args>
struct Action
{
   using ArgumentsTuple = std::tuple<Args...>;
};

} // namespace util::itc

#endif