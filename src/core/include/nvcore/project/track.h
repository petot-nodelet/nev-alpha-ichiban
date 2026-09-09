#pragma once
#include <vector>
#include <memory>
#include <cstdint>
#include <algorithm>
#include "clip.h"
#include "../id.h"
namespace nv::core {
enum class TrackType { Video, Audio };
class Track {
public:
    Track() : id_(std::stoull(generateId().substr(0, 16), nullptr, 16)) {}
    uint64_t id() const { return id_; }
    void setId(uint64_t id) { id_ = id; }
    TrackType type() const { return type_; }
    void setType(TrackType t) { type_ = t; }
    const std::vector<std::unique_ptr<Clip>>& clips() const { return clips_; }
    Clip* addClip() {
        auto clip = std::make_unique<Clip>(); clip->setTrackId(id_);
        Clip* ptr = clip.get(); clips_.push_back(std::move(clip)); return ptr;
    }
    void removeClip(uint64_t clipId) {
        clips_.erase(std::remove_if(clips_.begin(), clips_.end(), [clipId](const std::unique_ptr<Clip>& c) { return c->id() == clipId; }), clips_.end());
    }
    void setId(uint64_t id) { id_ = id; }
private:
    uint64_t id_; TrackType type_ = TrackType::Video; std::vector<std::unique_ptr<Clip>> clips_;
};
}
