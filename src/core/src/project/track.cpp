#include "nvcore/project/track.h"
#include "nvcore/result.h"
#include <algorithm>

namespace {
    // Helper: Cek apakah rentang waktu [newStart, newEnd) tumpang tindih dengan clip lain
    bool hasOverlap(const std::vector<std::unique_ptr<nv::core::Clip>>& clips, uint64_t excludeClipId, int64_t newStart, int64_t newEnd) {
        for (const auto& clip : clips) {
            if (clip->id() == excludeClipId) continue;
            int64_t clipStart = clip->positionMs();
            int64_t clipEnd = clipStart + clip->durationMs();
            
            // Kondisi overlap: newStart < clipEnd && newEnd > clipStart
            if (newStart < clipEnd && newEnd > clipStart) {
                return true;
            }
        }
        return false;
    }
}

namespace nv::core {

int64_t Track::durationMs() const {
    int64_t maxEnd = 0;
    for (const auto& clip : clips_) {
        int64_t clipEnd = clip->positionMs() + clip->durationMs();
        if (clipEnd > maxEnd) maxEnd = clipEnd;
    }
    return maxEnd;
}

nv::Result<void> Track::moveClip(uint64_t clipId, int64_t newPositionMs) {
    if (newPositionMs < 0) return nv::Result<void>::Err(nv::ErrorCode::InvalidArgument, "Position cannot be negative");

    Clip* targetClip = nullptr;
    for (auto& clip : clips_) {
        if (clip->id() == clipId) { targetClip = clip.get(); break; }
    }
    if (!targetClip) return nv::Result<void>::Err(nv::ErrorCode::NotFound, "Clip not found");

    int64_t duration = targetClip->durationMs();
    int64_t newEnd = newPositionMs + duration;

    if (hasOverlap(clips_, clipId, newPositionMs, newEnd)) {
        return nv::Result<void>::Err(nv::ErrorCode::InvalidArgument, "Move would cause overlap");
    }

    targetClip->setPositionMs(newPositionMs);
    return nv::Result<void>::Ok();
}

nv::Result<void> Track::trimClipStart(uint64_t clipId, int64_t newStartTimeMs) {
    Clip* targetClip = nullptr;
    for (auto& clip : clips_) {
        if (clip->id() == clipId) { targetClip = clip.get(); break; }
    }
    if (!targetClip) return nv::Result<void>::Err(nv::ErrorCode::NotFound, "Clip not found");

    if (newStartTimeMs >= targetClip->endTimeMs()) {
        return nv::Result<void>::Err(nv::ErrorCode::InvalidArgument, "Start time must be less than end time");
    }

    targetClip->setStartTimeMs(newStartTimeMs);
    return nv::Result<void>::Ok();
}

nv::Result<void> Track::trimClipEnd(uint64_t clipId, int64_t newEndTimeMs) {
    Clip* targetClip = nullptr;
    for (auto& clip : clips_) {
        if (clip->id() == clipId) { targetClip = clip.get(); break; }
    }
    if (!targetClip) return nv::Result<void>::Err(nv::ErrorCode::NotFound, "Clip not found");

    if (newEndTimeMs <= targetClip->startTimeMs()) {
        return nv::Result<void>::Err(nv::ErrorCode::InvalidArgument, "End time must be greater than start time");
    }

    targetClip->setEndTimeMs(newEndTimeMs);
    return nv::Result<void>::Ok();
}

nv::Result<Clip*> Track::splitClip(uint64_t clipId, int64_t splitPointMs) {
    Clip* targetClip = nullptr;
    for (auto& clip : clips_) {
        if (clip->id() == clipId) { targetClip = clip.get(); break; }
    }
    if (!targetClip) return nv::Result<Clip*>::Err(nv::ErrorCode::NotFound, "Clip not found");

    int64_t clipStart = targetClip->positionMs();
    int64_t clipEnd = clipStart + targetClip->durationMs();

    if (splitPointMs <= clipStart || splitPointMs >= clipEnd) {
        return nv::Result<Clip*>::Err(nv::ErrorCode::InvalidArgument, "Split point must be strictly inside the clip");
    }

    int64_t offsetInClip = splitPointMs - clipStart;
    int64_t originalStartTime = targetClip->startTimeMs();
    int64_t originalEndTime = targetClip->endTimeMs();
    uint64_t originalAssetId = targetClip->assetId();
    uint64_t originalTrackId = targetClip->trackId();

    // 1. Kecilkan clip asli
    targetClip->setEndTimeMs(originalStartTime + offsetInClip);

    // 2. Buat clip baru untuk sisa bagian kanan
    auto newClip = std::make_unique<Clip>();
    newClip->setAssetId(originalAssetId);
    newClip->setTrackId(originalTrackId);
    newClip->setStartTimeMs(originalStartTime + offsetInClip);
    newClip->setEndTimeMs(originalEndTime);
    newClip->setPositionMs(splitPointMs);

    Clip* ptr = newClip.get();
    clips_.push_back(std::move(newClip));

    return nv::Result<Clip*>::Ok(ptr);
}

nv::Result<Clip*> Track::duplicateClip(uint64_t clipId) {
    Clip* targetClip = nullptr;
    for (auto& clip : clips_) {
        if (clip->id() == clipId) { targetClip = clip.get(); break; }
    }
    if (!targetClip) return nv::Result<Clip*>::Err(nv::ErrorCode::NotFound, "Clip not found");

    auto newClip = std::make_unique<Clip>();
    newClip->setAssetId(targetClip->assetId());
    newClip->setTrackId(targetClip->trackId());
    newClip->setStartTimeMs(targetClip->startTimeMs());
    newClip->setEndTimeMs(targetClip->endTimeMs());
    
    int64_t newStart = targetClip->positionMs() + targetClip->durationMs();
    int64_t newEnd = newStart + newClip->durationMs();

    if (hasOverlap(clips_, 0, newStart, newEnd)) {
        return nv::Result<Clip*>::Err(nv::ErrorCode::InvalidArgument, "Cannot duplicate: no space immediately after clip");
    }

    newClip->setPositionMs(newStart);
    Clip* ptr = newClip.get();
    clips_.push_back(std::move(newClip));

    return nv::Result<Clip*>::Ok(ptr);
}

} // namespace nv::core
