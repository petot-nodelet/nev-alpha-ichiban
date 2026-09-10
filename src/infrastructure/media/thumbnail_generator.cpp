#include "thumbnail_generator.h"
#include <filesystem>

namespace nv::infrastructure {

nv::Result<void> ThumbnailGenerator::generateVideoThumbnail(
    const std::string& videoPath,
    const std::string& outputPath,
    double timestamp,
    int width,
    int height
) {
    (void)videoPath;
    (void)outputPath;
    (void)timestamp;
    (void)width;
    (void)height;
    return nv::Result<void>::Err(
        nv::ErrorCode::UnsupportedOperation,
        "ThumbnailGenerator::generateVideoThumbnail not implemented yet"
    );
}

nv::Result<void> ThumbnailGenerator::generateImageThumbnail(
    const std::string& imagePath,
    const std::string& outputPath,
    int width,
    int height
) {
    (void)imagePath;
    (void)outputPath;
    (void)width;
    (void)height;
    return nv::Result<void>::Err(
        nv::ErrorCode::UnsupportedOperation,
        "ThumbnailGenerator::generateImageThumbnail not implemented yet"
    );
}

std::string ThumbnailGenerator::getDefaultThumbnailPath(
    const std::string& projectFolder,
    uint64_t assetId
) {
    std::filesystem::path thumbDir = std::filesystem::path(projectFolder) / "assets" / "thumbnails";
    
    // FIX: Gabungkan string DULU, baru masukkan ke path operator /
    std::string filename = std::to_string(assetId) + ".jpg";
    return (thumbDir / filename).string();
}

} // namespace nv::infrastructure
