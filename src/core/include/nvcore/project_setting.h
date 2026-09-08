#pragma once
#include <string>
#include <cstdint>

namespace nv::core {

class ProjectSettings {
public:
    int32_t width() const { return width_; }
    void setWidth(int32_t w) { width_ = w > 0 ? w : 1920; }

    int32_t height() const { return height_; }
    void setHeight(int32_t h) { height_ = h > 0 ? h : 1080; }

    int32_t fpsNumerator() const { return fpsNumerator_; }
    int32_t fpsDenominator() const { return fpsDenominator_; }
    void setFps(int32_t num, int32_t den) { 
        fpsNumerator_ = num > 0 ? num : 30; 
        fpsDenominator_ = den > 0 ? den : 1; 
    }

    int64_t durationMs() const { return durationMs_; }
    void setDurationMs(int64_t ms) { durationMs_ = ms > 0 ? ms : 60000; }

    const std::string& backgroundColor() const { return backgroundColor_; }
    void setBackgroundColor(const std::string& color) { backgroundColor_ = color; }

private:
    int32_t width_ = 1920;
    int32_t height_ = 1080;
    int32_t fpsNumerator_ = 30;
    int32_t fpsDenominator_ = 1;
    int64_t durationMs_ = 60000;
    std::string backgroundColor_ = "#000000FF";
};

} // namespace nv::core