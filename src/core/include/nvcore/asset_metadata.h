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
};

} // namespace nv::core