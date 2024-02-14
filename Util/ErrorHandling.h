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
   mdDescrNotFound,
   descriptionNotFound,
   indexOutOfRange,
};
template<typename T>
using Ret = tl::expected<T, Error>;
using Void = Ret<void>;

template<typename T>
Ret<T&> safe_at(const std::vector<T>& vec, size_t index) {
    if (index < vec.size()) {
        return vec[index]; // Return the value if in range.
    } else {
        return tl::unexpected{Error::indexOutOfRange}; // Return an error.
    }
}
#endif // ERRORHANDLING_H

