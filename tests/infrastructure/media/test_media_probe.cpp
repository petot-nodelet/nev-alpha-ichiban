#include "framework/test_framework.h"
#include "media/media_probe.h"

NV_TEST_CASE(MediaProbe_FileNotFound) {
    auto result = nv::infrastructure::MediaProbe::probe("/tmp/nirvanaedit_nonexistent_media.mp4");
    NV_ASSERT(result.isErr());
    NV_ASSERT_EQ(result.error().code, nv::ErrorCode::NotFound);
}

NV_TEST_CASE(MediaProbe_GetFileExtension) {
    std::string ext = nv::infrastructure::MediaProbe::getFileExtension("/path/to/video.mp4");
    NV_ASSERT_EQ(ext, std::string(".mp4"));
}
