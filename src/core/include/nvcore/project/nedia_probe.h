#pragma once
#include "nvcore/project/asset_metadata.h"
#include "nvcore/result.h"
#include <string>

namespace nv::infrastructure {

class MediaProbe {
public:
    static nv::Result<nv::core::AssetMetadata> probe(const std::string& filePath);
};

} // namespace nv::infrastructure