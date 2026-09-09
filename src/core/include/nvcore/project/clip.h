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
}
