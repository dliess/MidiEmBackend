#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

#include "FixedSizeString.h"
#include <tl/expected.hpp>

struct ErrorDescr
{
   template<typename Arg>
   ErrorDescr(Arg&& arg) : descr(std::forward<Arg>(arg)) {}
   util::FixedSizeString<32> descr;
};
enum class Error
{
   uuidNotFound,
   melodicVoicesFull,
   maxComponentsPerVoiceReached,
   mdDescrNotFound,
   descriptionNotFound,
   indexOutOfRange,
   soundHandlerNotAvailable,
};
template<typename T>
using Ret = tl::expected<T, Error>;
using Void = Ret<void>;

#include <concepts>
template<typename Container>
concept Indexable = requires(Container a, size_t index) {
    { a[index] } -> std::convertible_to<typename Container::value_type&>;
    { a.size() } -> std::convertible_to<size_t>;
};

template<Indexable Container>
Ret<typename Container::value_type*> safe_at(Container& container, size_t index) {
    if (index < container.size()) {
        return &container[index]; // Return a pointer to the value if in range.
    } else {
        return tl::unexpected{Error::indexOutOfRange}; // Return an error.
    }
}
// I would like to express that typename C is a container of T, but I don't know how to do it.
// template<typename T, typename C>
// Ret<T*> safe_at(const C& vec, size_t index) {
//     if (index < vec.size()) {
//         return &vec[index]; // Return the value if in range.
//     } else {
//         return tl::unexpected{Error::indexOutOfRange}; // Return an error.
//     }
// }
#endif // ERRORHANDLING_H

