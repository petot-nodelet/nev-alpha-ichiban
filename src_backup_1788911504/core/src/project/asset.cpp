#include "nvcore/project/asset.h"
#include "nvcore/id.h"
#include <cstdlib> // for strtoull

namespace nv::core {

Asset::Asset() 
    : id_(std::strtoull(generateId().substr(0, 16).c_str(), nullptr, 16))
    , type_(AssetType::Video)
    , pathType_(PathType::Relative)
    , isExternal_(false) 
{
}

void Asset::setPath(const std::string& path, PathType type) {
    path_ = path;
    pathType_ = type;
    if (type == PathType::Absolute) {
        // Default to external if absolute, can be overridden
        isExternal_ = true; 
    }
}

} // namespace nv::core
