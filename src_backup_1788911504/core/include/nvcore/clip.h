#pragma once
#include <cstdint>
namespace nv::core {
class Clip {
public:
    uint64_t id() const { return id_; }
    void setId(uint64_t id) { id_ = id; }
    uint64_t assetId() const { return assetId_; }
    void setAssetId(uint64_t id) { assetId_ = id; }
    int64_t startTimeMs() const { return startTimeMs_; }
    void setStartTimeMs(int64_t ms) { startTimeMs_ = ms; }
    int64_t endTimeMs() const { return endTimeMs_; }
    void setEndTimeMs(int64_t ms) { endTimeMs_ = ms; }
    int64_t positionMs() const { return positionMs_; }
    void setPositionMs(int64_t ms) { positionMs_ = ms; }
    uint64_t trackId() const { return trackId_; }
    void setTrackId(uint64_t id) { trackId_ = id; }
private:
    uint64_t id_ = 0;
    uint64_t assetId_ = 0;
    int64_t startTimeMs_ = 0;
    int64_t endTimeMs_ = 0;
    int64_t positionMs_ = 0;
    uint64_t trackId_ = 0;
};
}