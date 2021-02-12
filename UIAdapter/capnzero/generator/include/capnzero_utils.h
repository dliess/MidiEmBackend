#ifndef CAPNZERO_UTILS_H
#define CAPNZERO_UTILS_H

#include <type_traits>

namespace capnzero
{

template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e)
{
   return static_cast<typename std::underlying_type<E>::type>(e);
}

} //namespace capnzero
#endif