#pragma once

#include "nvcore/result.h"
#include <string>

namespace nv::infrastructure {

/**
 * ThumbnailGenerator - Generate thumbnails from media files
 * 
 * Supports:
 * - Video: Extract frame at specific timestamp
 * - Image: Resize to thumbnail dimensions
 * - Audio: Generate waveform image (future)
 */
class ThumbnailGenerator {
public:
    /**
     * Generate thumbnail from video file
     * @param videoPath Path to video file
     * @param outputPath Path to save thumbnail (JPEG/PNG)
     * @param timestamp Time position in seconds (default: 1.0)
     * @param width Thumbnail width (default: 320)
     * @param height Thumbnail height (default: 180, 0 = auto)
     * @return Result<void> indicating success or failure
     */
    static nv::Result<void> generateVideoThumbnail(
        const std::string& videoPath,
        const std::string& outputPath,
        double timestamp = 1.0,
        int width = 320,
        int height = 180
    );
    
    /**
     * Generate thumbnail from image file
     * @param imagePath Path to image file
     * @param outputPath Path to save thumbnail
     * @param width Thumbnail width (default: 320)
     * @param height Thumbnail height (default: 180, 0 = auto)
     * @return Result<void> indicating success or failure
     */
    static nv::Result<void> generateImageThumbnail(
        const std::string& imagePath,
        const std::string& outputPath,
        int width = 320,
        int height = 180
    );
    
    /**
     * Generate default thumbnail path based on asset ID
     * @param projectFolder Path to project folder
     * @param assetId Asset ID
     * @return Path to thumbnail file
     */
    static std::string getDefaultThumbnailPath(
        const std::string& projectFolder,
        uint64_t assetId
    );
};

} // namespace nv::infrastructure
