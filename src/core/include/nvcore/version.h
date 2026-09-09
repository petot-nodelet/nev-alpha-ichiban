#pragma once
#include <string>
namespace nv {
struct Version { int major, minor, patch; std::string preRelease; std::string toString() const; };
const Version& currentVersion();
const Version& projectFormatVersion();
}
