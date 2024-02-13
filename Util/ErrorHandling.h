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
   melodicVoiceFull
};
template<typename T>
using Ret = tl::expected<T, Error>;
using Void = Ret<void>;


#endif // ERRORHANDLING_H

