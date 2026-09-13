#include "framework/test_framework.h"
#include "media/media_importer.h"

NV_TEST_CASE(MediaImporter_DetectAssetType) {
    NV_ASSERT(nv::infrastructure::MediaImporter::detectAssetType("video.mp4") == nv::core::AssetType::Video);
    NV_ASSERT(nv::infrastructure::MediaImporter::detectAssetType("audio.wav") == nv::core::AssetType::Audio);
    NV_ASSERT(nv::infrastructure::MediaImporter::detectAssetType("image.png") == nv::core::AssetType::Image);
}
