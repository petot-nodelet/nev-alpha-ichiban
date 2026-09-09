#pragma once
#include "nvcore/serialization/project_serializer.h"
namespace nv::infrastructure {
class ZipProjectSerializer : public nv::core::ProjectSerializer {
public:
    nv::Result<void> serialize(const nv::core::Project& project, const std::string& outputPath) override;
};
}
