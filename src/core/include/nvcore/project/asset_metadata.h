#pragma once
#include <string>
#include <cstdint>
namespace nv::core {
struct AssetMetadata {
    int64_t duration_ms = 0;
    int32_t width = 0;
    int32_t height = 0;
    int32_t fps_numerator = 0;
    int32_t fps_denominator = 0;
    bool has_audio = false;
    std::string codec;
    
    // Phase 2 additions
    std::string probe_status;  // "ok", "failed", "pending"
    std::string error_message;
    int64_t file_size_bytes = 0;
    std::string format;  // "mp4", "mov", "png", etc.
};
}
