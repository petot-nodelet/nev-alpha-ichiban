#pragma once

#include "nvcore/project/asset_metadata.h"
#include "nvcore/result.h"
#include <string>

namespace nv::infrastructure {

/**
 * MediaProbe - Extract metadata from media files using FFmpeg
 * 
 * Supported: video, audio, image
 * Extracted: duration, resolution, fps, codec, has_audio/video, format
 */
class MediaProbe {
public:
    static nv::Result<nv::core::AssetMetadata> probe(const std::string& filePath);
    static bool isSupportedFormat(const std::string& filePath);
    static std::string getFileExtension(const std::string& filePath);
};

} // namespace nv::infrastructure
