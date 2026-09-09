#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "track.h"
namespace nv::core {
class Timeline {
public:
    const std::vector<std::unique_ptr<Track>>& tracks() const { return tracks_; }
    Track* addTrack(TrackType type) {
        auto track = std::make_unique<Track>(); track->setType(type);
        Track* ptr = track.get(); tracks_.push_back(std::move(track)); return ptr;
    }
    void removeTrack(uint64_t trackId) {
        tracks_.erase(std::remove_if(tracks_.begin(), tracks_.end(), [trackId](const std::unique_ptr<Track>& t) { return t->id() == trackId; }), tracks_.end());
    }
private:
    std::vector<std::unique_ptr<Track>> tracks_;
};
}
