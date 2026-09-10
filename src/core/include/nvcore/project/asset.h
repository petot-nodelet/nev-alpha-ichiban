#pragma once
#include <string>
#include <cstdint>
#include "asset_metadata.h"
#include "../id.h"

namespace nv::core {

enum class AssetType { Video, Audio, Image };
enum class PathType { Relative, Absolute };

class Asset {
public:
    Asset() : id_(std::stoull(generateId().substr(0, 16), nullptr, 16)), type_(AssetType::Video), pathType_(PathType::Relative), isExternal_(false) {}
    
    uint64_t id() const { return id_; }
    void setId(uint64_t id) { id_ = id; }
    
    AssetType type() const { return type_; }
    void setType(AssetType t) { type_ = t; }
    
    const std::string& path() const { return path_; }
    void setPath(const std::string& p, PathType pt) { 
        path_ = p; 
        pathType_ = pt; 
        if (pt == PathType::Absolute) isExternal_ = true; 
    }
    
    PathType pathType() const { return pathType_; }
    bool isExternal() const { return isExternal_; }
    void setExternal(bool e) { isExternal_ = e; }
    
    const AssetMetadata& metadata() const { return metadata_; }
    AssetMetadata& metadata() { return metadata_; }

private:
    uint64_t id_; 
    AssetType type_; 
    std::string path_; 
    PathType pathType_; 
    bool isExternal_; 
    AssetMetadata metadata_;
};

} // namespace nv::core
