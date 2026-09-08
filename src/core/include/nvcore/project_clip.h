#pragma once
#include <cstdint>
#include "nvcore/id.h"
#include <cstdlib>

namespace nv::core {

class Clip {
public:
    Clip() : id_(std::strtoull(generateId().substr(0, 16).c_str(), nullptr, 16)) {}

    uint64_t id() const { return id_; }
    
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
    uint64_t id_;
    uint64_t assetId_ = 0;
    int64_t startTimeMs_ = 0;
    int64_t endTimeMs_ = 0;
    int64_t positionMs_ = 0;
    uint64_t trackId_ = 0;
};

} // namespace nv::core