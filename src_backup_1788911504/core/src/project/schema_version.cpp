#include "nvcore/serialization/schema_version.h"

namespace nv::core {

std::string SchemaVersion::toString() const {
    return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
}

bool SchemaVersion::operator>=(const SchemaVersion& other) const {
    if (major != other.major) return major > other.major;
    if (minor != other.minor) return minor > other.minor;
    return patch >= other.patch;
}

bool SchemaVersion::operator==(const SchemaVersion& other) const {
    return major == other.major && minor == other.minor && patch == other.patch;
}

bool SchemaVersion::operator<(const SchemaVersion& other) const {
    return !(*this >= other);
}

} // namespace nv::core
