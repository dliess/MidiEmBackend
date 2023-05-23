#include "InstrumentVoice.h"

namespace base::instruments
{
inline Voice::Voice(std::string name) noexcept :
    name(std::move(name))
{
}

inline
bool Voice::operator==(const Voice& rhs) const
{
    return name == rhs.name &&
           components == rhs.components;
}


}   // namespace base::instruments