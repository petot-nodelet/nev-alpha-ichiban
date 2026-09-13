#!/bin/bash
set -e

echo "🔧 Memperbaiki header files..."

# Fix timeline.h
cat > src/core/include/nvcore/project/timeline.h << 'HEADER'
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
        auto track = std::make_unique<Track>();
        track->setType(type);
        Track* ptr = track.get();
        tracks_.push_back(std::move(track));
        return ptr;
    }
    
    void removeTrack(uint64_t trackId) {
        tracks_.erase(
            std::remove_if(tracks_.begin(), tracks_.end(),
                [trackId](const std::unique_ptr<Track>& t) { return t->id() == trackId; }),
            tracks_.end()
        );
    }

private:
    std::vector<std::unique_ptr<Track>> tracks_;
};

} // namespace nv::core
HEADER

# Fix track.h
cat > src/core/include/nvcore/project/track.h << 'HEADER'
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
        auto clip = std::make_unique<Clip>();
        clip->setTrackId(id_);
        Clip* ptr = clip.get();
        clips_.push_back(std::move(clip));
        return ptr;
    }
    
    void removeClip(uint64_t clipId) {
        clips_.erase(
            std::remove_if(clips_.begin(), clips_.end(),
                [clipId](const std::unique_ptr<Clip>& c) { return c->id() == clipId; }),
            clips_.end()
        );
    }

private:
    uint64_t id_;
    TrackType type_ = TrackType::Video;
    std::vector<std::unique_ptr<Clip>> clips_;
};

} // namespace nv::core
HEADER

# Fix clip.h
cat > src/core/include/nvcore/project/clip.h << 'HEADER'
#pragma once
#include <cstdint>
#include "../id.h"

namespace nv::core {

class Clip {
public:
    Clip() : id_(std::stoull(generateId().substr(0, 16), nullptr, 16)) {}
    
    uint64_t id() const { return id_; }
    void setId(uint64_t id) { id_ = id; }
    
    uint64_t assetId() const { return assetId_; }
    void setAssetId(uint64_t id) { assetId_ = id; }
    
    int64_t startTimeMs() const { return startTimeMs_; }
    void setStartTimeMs(int64_t ms) { startTimeMs_ = ms >= 0 ? ms : 0; }
    
    int64_t endTimeMs() const { return endTimeMs_; }
    void setEndTimeMs(int64_t ms) { endTimeMs_ = ms >= 0 ? ms : 0; }
    
    int64_t durationMs() const { return endTimeMs_ - startTimeMs_; }
    
    int64_t positionMs() const { return positionMs_; }
    void setPositionMs(int64_t ms) { positionMs_ = ms >= 0 ? ms : 0; }
    
    uint64_t trackId() const { return trackId_; }
    void setTrackId(uint64_t id) { trackId_ = id; }

private:
    uint64_t id_, assetId_ = 0, trackId_ = 0;
    int64_t startTimeMs_ = 0, endTimeMs_ = 0, positionMs_ = 0;
};

} // namespace nv::core
HEADER

echo "✅ Header files diperbaiki!"
echo ""
echo "📝 Perubahan:"
echo "  - timeline.h: Fixed typo 'trac ks_' → 'tracks_'"
echo "  - track.h: Fixed typo 'ret urn' → 'return', removed spaces in includes"
echo "  - clip.h: Removed spaces in includes"
echo ""
echo "🔨 Sekarang build ulang:"
echo "  cd build && cmake --build . -j\$(nproc)"
