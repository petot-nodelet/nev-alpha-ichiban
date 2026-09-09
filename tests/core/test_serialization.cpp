#include "framework/test_framework.h"
#include "nvcore/project/project.h"
#include "nvcore/serialization/project_serializer.h"
#include "serialization/zip_project_serializer.h"
#include "serialization/zip_project_deserializer.h"
#include <filesystem>
#include <fstream>
#include <iostream>

NV_TEST_CASE(Serialization_RoundTrip) {
    // 1. Create Project
    nv::core::Project p;
    p.info().setName("Test Project");
    p.settings().setWidth(1280);
    
    // Add Asset
    auto* asset = p.addAsset(nv::core::AssetType::Video);
    asset->setPath("media/clip.mp4", nv::core::PathType::Relative);
    uint64_t originalAssetId = asset->id();

    // Add Composition/Track/Clip
    auto* comp = p.addComposition();
    comp->setName("Main");
    auto* track = comp->timeline().addTrack(nv::core::TrackType::Video);
    auto* clip = track->addClip();
    clip->setAssetId(originalAssetId);
    clip->setStartTimeMs(1000);
    uint64_t originalClipId = clip->id();

    // 2. Serialize - use temp directory that is definitely writable
    std::string testFile = "/tmp/nirvanaedit_test_project.nvp";
    
    // Clean up any existing file
    std::filesystem::remove(testFile);
    std::filesystem::remove(testFile + ".tmp");
    
    nv::infrastructure::ZipProjectSerializer serializer;
    auto res = serializer.serialize(p, testFile);
    
    if (res.isErr()) {
        std::cerr << "[DEBUG] Serialize error: " << res.error().message << std::endl;
    }
    
    NV_ASSERT(res.isOk());
    NV_ASSERT(std::filesystem::exists(testFile));

    // 3. Deserialize
    nv::infrastructure::ZipProjectDeserializer deserializer;
    auto resLoad = deserializer.deserialize(testFile);
    
    if (resLoad.isErr()) {
        std::cerr << "[DEBUG] Deserialize error: " << resLoad.error().message << std::endl;
    }
    
    NV_ASSERT(resLoad.isOk());
    
    const auto& p2 = resLoad.value();

    // 4. Verify Data
    NV_ASSERT_EQ(p2.info().name(), std::string("Test Project"));
    NV_ASSERT_EQ(p2.settings().width(), 1280);
    
    // FIX: Use 'u' suffix for unsigned literals to avoid sign-compare and old-style cast warnings
    NV_ASSERT_EQ(p2.assets().size(), 1u);
    NV_ASSERT_EQ(p2.assets()[0]->id(), originalAssetId);
    
    // The loaded project will have 2 compositions: 1 default + 1 from file
    NV_ASSERT_EQ(p2.compositions().size(), 2u);
    const auto& loadedComp = p2.compositions()[1];
    NV_ASSERT_EQ(loadedComp->timeline().tracks().size(), 1u);
    const auto& loadedTrack = loadedComp->timeline().tracks()[0];
    NV_ASSERT_EQ(loadedTrack->clips().size(), 1u);
    NV_ASSERT_EQ(loadedTrack->clips()[0]->id(), originalClipId);
    NV_ASSERT_EQ(loadedTrack->clips()[0]->startTimeMs(), 1000LL); // Use LL for long long

    // 5. Cleanup
    std::filesystem::remove(testFile);
    std::filesystem::remove(testFile + ".tmp");
}

NV_TEST_CASE(Serialization_FileNotFound) {
    nv::infrastructure::ZipProjectDeserializer deserializer;
    auto res = deserializer.deserialize("/tmp/nirvanaedit_nonexistent_12345.nvp");
    NV_ASSERT(res.isErr());
}

NV_TEST_CASE(Serialization_InvalidFile) {
    std::string badFile = "/tmp/nirvanaedit_bad.nvp";
    {
        std::ofstream ofs(badFile);
        ofs << "this is not a valid zip file";
    }
    
    nv::infrastructure::ZipProjectDeserializer deserializer;
    auto res = deserializer.deserialize(badFile);
    NV_ASSERT(res.isErr());
    std::filesystem::remove(badFile);
}
