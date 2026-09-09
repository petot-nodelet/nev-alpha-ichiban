#pragma once
#include <vector>
#include <memory>
#include <cstdint>
#include "clip.h"
#include "nvcore/id.h"
#include <cstdlib>

namespace nv::core {

enum class TrackType { Video, Audio };

class Track {
public:
    Track() : id_(std::strtoull(generateId().substr(0, 16).c_str(), nullptr, 16)) {}

    uint64_t id() const { return id_; }
    TrackType type() const { return type_; }
    void setType(TrackType type) { type_ = type; }

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

    Clip* getClip(uint64_t clipId) {
        for (auto& c : clips_) {
            if (c->id() == clipId) return c.get();
        }
        return nullptr;
    }

private:
    uint64_t id_;
    TrackType type_ = TrackType::Video;
    std::vector<std::unique_ptr<Clip>> clips_;
};

} // namespace nv::core