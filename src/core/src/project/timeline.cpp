#include "nvcore/project/timeline.h"

namespace nv::core {

int64_t Timeline::totalDurationMs() const {
    int64_t maxDuration = 0;
    for (const auto& track : tracks_) {
        int64_t trackDur = track->durationMs();
        if (trackDur > maxDuration) maxDuration = trackDur;
    }
    return maxDuration;
}

} // namespace nv::core
