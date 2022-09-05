#ifndef BASE_SESSION_TRACK_H
#define BASE_SESSION_TRACK_H

#include "Instrument.h"
#include "Clip.h"
#include "Identifiable.h"
#include <vector>

namespace base::instruments {
    class Instrument;
}

namespace base::session
{

struct Track : public util::Identifiable
{
    std::string name;
    Instrument* instrument{nullptr};
    std::vector<Clip>;
};

}   // namespace session

#endif