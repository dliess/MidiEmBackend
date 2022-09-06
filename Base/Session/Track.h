#ifndef BASE_SESSION_TRACK_H
#define BASE_SESSION_TRACK_H

#include "Instrument.h"
#include "Clip.h"
#include "Identifiable.h"
#include <vector>
#include <optional>

namespace base::instruments {
    class Instrument;
}

namespace base::session
{

struct Track : public util::Identifiable
{
    std::string name;
    instruments::Instrument* instrument{nullptr};
    std::optional<int> activeClip;
    std::vector<Clip> clips;
};

}   // namespace session

#endif