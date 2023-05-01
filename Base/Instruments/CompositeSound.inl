#include "CompositeSound.h"

namespace base::instruments
{
inline CompositeSound::CompositeSound(std::string name) noexcept :
    name(std::move(name))
{
}

inline
bool CompositeSound::operator==(const CompositeSound& rhs) const
{
    return name == rhs.name &&
           voices == rhs.voices;
}


}   // namespace base::instruments