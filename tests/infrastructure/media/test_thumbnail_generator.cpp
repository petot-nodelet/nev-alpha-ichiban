#include "framework/test_framework.h"
#include "media/thumbnail_generator.h"

NV_TEST_CASE(ThumbnailGenerator_GetDefaultPath) {
    std::string path = nv::infrastructure::ThumbnailGenerator::getDefaultThumbnailPath("/tmp/project", 12345);
    NV_ASSERT(path.find("12345") != std::string::npos);
    NV_ASSERT(path.find(".jpg") != std::string::npos);
    NV_ASSERT(path.find("thumbnails") != std::string::npos);
}
