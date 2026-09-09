#pragma once
#include <string>

namespace nv::core {

struct SchemaVersion {
    int major = 1;
    int minor = 0;
    int patch = 0;

    std::string toString() const;
    bool operator>=(const SchemaVersion& other) const;
    bool operator==(const SchemaVersion& other) const;
    bool operator<(const SchemaVersion& other) const;
};

} // namespace nv::core