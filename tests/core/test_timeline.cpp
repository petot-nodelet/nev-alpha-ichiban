#include "framework/test_framework.h"
#include "nvcore/project/project.h"

NV_TEST_CASE(Timeline_Duration) {
    nv::core::Project p;
    auto* comp = p.addComposition();
    
    NV_ASSERT_EQ(comp->timeline().totalDurationMs(), 0LL);

    auto* track = comp->timeline().addTrack(nv::core::TrackType::Video);
    auto* clip = track->addClip();
    clip->setPositionMs(1000);
    clip->setStartTimeMs(0);
    clip->setEndTimeMs(5000); // duration 5000
    
    NV_ASSERT_EQ(track->durationMs(), 6000LL); // 1000 + 5000
    NV_ASSERT_EQ(comp->timeline().totalDurationMs(), 6000LL);
}

NV_TEST_CASE(Track_MoveClip_WithOverlapCheck) {
    nv::core::Project p;
    auto* track = p.addComposition()->timeline().addTrack(nv::core::TrackType::Video);
    
    // FIX: Gunakan durasi kecil agar ada ruang untuk move
    auto* clip1 = track->addClip();
    clip1->setPositionMs(0);
    clip1->setStartTimeMs(0);
    clip1->setEndTimeMs(1000);  // duration 1000, occupies [0, 1000)

    auto* clip2 = track->addClip();
    clip2->setPositionMs(6000);
    clip2->setStartTimeMs(0);
    clip2->setEndTimeMs(1000);  // duration 1000, occupies [6000, 7000)

    // Move clip1 to 2000 (should succeed, occupies [2000, 3000), no overlap with [6000, 7000))
    auto res1 = track->moveClip(clip1->id(), 2000);
    NV_ASSERT(res1.isOk());
    NV_ASSERT_EQ(clip1->positionMs(), 2000LL);

    // Move clip1 to 5500 (should fail, would occupy [5500, 6500), overlaps with [6000, 7000))
    auto res2 = track->moveClip(clip1->id(), 5500);
    NV_ASSERT(res2.isErr());
    NV_ASSERT_EQ(res2.error().code, nv::ErrorCode::InvalidArgument);
    
    // State should remain unchanged after failed move
    NV_ASSERT_EQ(clip1->positionMs(), 2000LL);
}

NV_TEST_CASE(Track_SplitClip) {
    nv::core::Project p;
    auto* track = p.addComposition()->timeline().addTrack(nv::core::TrackType::Video);
    
    auto* clip = track->addClip();
    uint64_t clipId = clip->id();
    clip->setPositionMs(1000);
    clip->setStartTimeMs(0);
    clip->setEndTimeMs(6000); // duration 6000

    // Split at position 4000 (offset 3000 from start)
    auto res = track->splitClip(clipId, 4000);
    NV_ASSERT(res.isOk());
    
    nv::core::Clip* newClip = res.value();
    NV_ASSERT(newClip != nullptr);
    NV_ASSERT_EQ(newClip->assetId(), clip->assetId());
    
    // Original clip should be shrunk
    NV_ASSERT_EQ(clip->positionMs(), 1000LL);
    NV_ASSERT_EQ(clip->startTimeMs(), 0LL);
    NV_ASSERT_EQ(clip->endTimeMs(), 3000LL);
    NV_ASSERT_EQ(clip->durationMs(), 3000LL);

    // New clip should start at split point
    NV_ASSERT_EQ(newClip->positionMs(), 4000LL);
    NV_ASSERT_EQ(newClip->startTimeMs(), 3000LL);
    NV_ASSERT_EQ(newClip->endTimeMs(), 6000LL);
    NV_ASSERT_EQ(newClip->durationMs(), 3000LL);
}

NV_TEST_CASE(Track_TrimClip) {
    nv::core::Project p;
    auto* track = p.addComposition()->timeline().addTrack(nv::core::TrackType::Video);
    
    auto* clip = track->addClip();
    clip->setPositionMs(1000);
    clip->setStartTimeMs(1000);
    clip->setEndTimeMs(6000); // duration 5000

    // Trim start
    auto resStart = track->trimClipStart(clip->id(), 2000);
    NV_ASSERT(resStart.isOk());
    NV_ASSERT_EQ(clip->startTimeMs(), 2000LL);
    NV_ASSERT_EQ(clip->durationMs(), 4000LL);

    // Trim end
    auto resEnd = track->trimClipEnd(clip->id(), 5000);
    NV_ASSERT(resEnd.isOk());
    NV_ASSERT_EQ(clip->endTimeMs(), 5000LL);
    NV_ASSERT_EQ(clip->durationMs(), 3000LL);

    // Invalid trim (start >= end)
    auto resInvalid = track->trimClipStart(clip->id(), 6000);
    NV_ASSERT(resInvalid.isErr());
}
