#pragma once

#include <string>

namespace nv {

struct Version {
    int major;
    int minor;
    int patch;
    std::string preRelease;

    std::string toString() const;
    int toInt() const;
};

const Version& currentVersion();
const Version& projectFormatVersion();

} // namespace nv
