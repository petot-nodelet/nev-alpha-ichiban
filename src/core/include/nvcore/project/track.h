#pragma once
#include <vector>
#include <memory>
#include <cstdint>
#include <algorithm>
#include "clip.h"
#include "../id.h"
#include "nvcore/result.h"

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
        auto clip = std::make_unique<Clip>();
        clip->setTrackId(id_);
        Clip* ptr = clip.get();
        clips_.push_back(std::move(clip));
        return ptr;
    }
    
    void removeClip(uint64_t clipId) {
        clips_.erase(std::remove_if(clips_.begin(), clips_.end(),
            [clipId](const std::unique_ptr<Clip>& c) { return c->id() == clipId; }), clips_.end());
    }

    // Phase 3: Computed duration & Operations
    int64_t durationMs() const;
    nv::Result<void> moveClip(uint64_t clipId, int64_t newPositionMs);
    nv::Result<void> trimClipStart(uint64_t clipId, int64_t newStartTimeMs);
    nv::Result<void> trimClipEnd(uint64_t clipId, int64_t newEndTimeMs);
    nv::Result<Clip*> splitClip(uint64_t clipId, int64_t splitPointMs);
    nv::Result<Clip*> duplicateClip(uint64_t clipId);

private:
    uint64_t id_;
    TrackType type_ = TrackType::Video;
    std::vector<std::unique_ptr<Clip>> clips_;
};

} // namespace nv::core
