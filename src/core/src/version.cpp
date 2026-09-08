#include "nvcore/version.h"
#include <sstream>

namespace nv {

std::string Version::toString() const {
    std::ostringstream oss;
    oss << major << "." << minor << "." << patch;
    if (!preRelease.empty()) {
        oss << "-" << preRelease;
    }
    return oss.str();
}

int Version::toInt() const {
    return major * 10000 + minor * 100 + patch;
}

const Version& currentVersion() {
    static const Version v{0, 0, 1, "dev"};
    return v;
}

const Version& projectFormatVersion() {
    // Project format has independent versioning.
    // Bump this when .nvp schema changes.
    static const Version v{1, 0, 0, ""};
    return v;
}

} // namespace nv
