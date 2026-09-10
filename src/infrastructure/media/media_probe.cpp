#include "media_probe.h"
#include <filesystem>

namespace nv::infrastructure {

nv::Result<nv::core::AssetMetadata> MediaProbe::probe(const std::string& filePath) {
    // TODO: Implement FFmpeg probing
    // 1. Open file with avformat_open_input
    // 2. Find stream info with avformat_find_stream_info
    // 3. Extract metadata from streams
    // 4. Close and cleanup
    
    (void)filePath;
    return nv::Result<nv::core::AssetMetadata>::Err(
        nv::ErrorCode::UnsupportedOperation,
        "MediaProbe::probe not implemented yet"
    );
}

bool MediaProbe::isSupportedFormat(const std::string& filePath) {
    // TODO: Check file extension against supported formats
    (void)filePath;
    return false;
}

std::string MediaProbe::getFileExtension(const std::string& filePath) {
    std::filesystem::path p(filePath);
    return p.extension().string();
}

} // namespace nv::infrastructure
