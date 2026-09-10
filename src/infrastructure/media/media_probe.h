#pragma once

#include "nvcore/project/asset_metadata.h"
#include "nvcore/result.h"
#include <string>

namespace nv::infrastructure {

/**
 * MediaProbe - Extract metadata from media files using FFmpeg
 * 
 * Supported formats: video, audio, image
 * Extracted metadata: duration, resolution, fps, codec, has_audio, etc.
 */
class MediaProbe {
public:
    /**
     * Probe a media file and extract metadata
     * @param filePath Path to the media file
     * @return Result containing AssetMetadata or error
     */
    static nv::Result<nv::core::AssetMetadata> probe(const std::string& filePath);
    
    /**
     * Check if a file is a supported media format
     * @param filePath Path to the file
     * @return true if supported, false otherwise
     */
    static bool isSupportedFormat(const std::string& filePath);
    
    /**
     * Get file extension from path
     * @param filePath Path to the file
     * @return File extension (e.g., "mp4", "jpg")
     */
    static std::string getFileExtension(const std::string& filePath);
};

} // namespace nv::infrastructure
