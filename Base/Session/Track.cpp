#include "Track.h"

using namespace base;

session::Track::Track(std::string name) noexcept :
    name(std::move(name))
{}