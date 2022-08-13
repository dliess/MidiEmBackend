#include "CompositeSound.h"

namespace base::instruments
{
inline CompositeSound::CompositeSound(std::string name) noexcept :
    name(std::move(name))
{
}

}   // namespace base::instruments