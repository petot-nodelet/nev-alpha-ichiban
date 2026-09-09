#include "nvcore/version.h"
namespace nv {
std::string Version::toString() const {
    std::string s = std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
    if (!preRelease.empty()) {
        s += "-" + preRelease;
    }
    return s;
}
const Version& currentVersion() { static const Version v{0, 0, 1, "dev"}; return v; }
const Version& projectFormatVersion() { static const Version v{1, 0, 0, ""}; return v; }
}
