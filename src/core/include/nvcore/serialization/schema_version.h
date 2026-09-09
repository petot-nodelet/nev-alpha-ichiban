#pragma once
#include <string>
namespace nv::core {
struct SchemaVersion {
    int major = 1, minor = 0, patch = 0;
    std::string toString() const { return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch); }
    bool operator>=(const SchemaVersion& o) const {
        if (major != o.major) return major > o.major;
        if (minor != o.minor) return minor > o.minor;
        return patch >= o.patch;
    }
    bool operator==(const SchemaVersion& o) const { return major == o.major && minor == o.minor && patch == o.patch; }
    bool operator<(const SchemaVersion& o) const { return !(*this >= o); }
};
}
