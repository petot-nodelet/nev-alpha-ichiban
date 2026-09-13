#include "framework/test_framework.h"
#include "media/media_probe.h"
#include <filesystem>
#include <fstream>

NV_TEST_CASE(MediaProbe_FileNotFound) {
    auto res = nv::infrastructure::MediaProbe::probe("/tmp/nirvanaedit_nonexistent_12345.mp4");
    NV_ASSERT(res.isErr());
    NV_ASSERT_EQ(res.error().code, nv::ErrorCode::NotFound);
}

NV_TEST_CASE(MediaProbe_InvalidFile) {
    // Create a fake file that is not valid media
    std::string badFile = "/tmp/nirvanaedit_bad_media.tmp";
    {
        std::ofstream ofs(badFile);
        ofs << "this is not a media file";
    }
    
    auto res = nv::infrastructure::MediaProbe::probe(badFile);
    // FFmpeg should return error for invalid format
    NV_ASSERT(res.isErr());
    
    std::filesystem::remove(badFile);
}

NV_TEST_CASE(MediaProbe_GetFileExtension) {
    std::string ext = nv::infrastructure::MediaProbe::getFileExtension("/path/to/video.mp4");
    NV_ASSERT_EQ(ext, std::string(".mp4"));
    
    std::string ext2 = nv::infrastructure::MediaProbe::getFileExtension("/path/to/audio.wav");
    NV_ASSERT_EQ(ext2, std::string(".wav"));
}

NV_TEST_CASE(MediaProbe_IsSupportedFormat) {
    NV_ASSERT(nv::infrastructure::MediaProbe::isSupportedFormat("video.mp4"));
    NV_ASSERT(nv::infrastructure::MediaProbe::isSupportedFormat("audio.wav"));
    NV_ASSERT(nv::infrastructure::MediaProbe::isSupportedFormat("image.png"));
    NV_ASSERT(!nv::infrastructure::MediaProbe::isSupportedFormat("document.txt"));
    NV_ASSERT(!nv::infrastructure::MediaProbe::isSupportedFormat("data.bin"));
}
