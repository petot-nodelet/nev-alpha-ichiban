#pragma once
#include <string>
#include <cstdint>
#include "asset_metadata.h"

namespace nv::core {

enum class AssetType { Video, Audio, Image };
enum class PathType { Relative, Absolute };

class Asset {
public:
    Asset();

    uint64_t id() const { return id_; }
    AssetType type() const { return type_; }
    void setType(AssetType type) { type_ = type; }

    const std::string& path() const { return path_; }
    void setPath(const std::string& path, PathType type);

    PathType pathType() const { return pathType_; }
    bool isExternal() const { return isExternal_; }
    void setExternal(bool ext) { isExternal_ = ext; }

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
