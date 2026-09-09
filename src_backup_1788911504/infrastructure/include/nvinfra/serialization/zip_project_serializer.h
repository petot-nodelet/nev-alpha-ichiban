#pragma once
#include "nvcore/project_serializer.h"

namespace nv::infrastructure {

class ZipProjectSerializer : public nv::core::ProjectSerializer {
public:
    nv::core::Result<void> serialize(const nv::core::Project& project, const std::string& outputPath) override;
};

class ZipProjectDeserializer : public nv::core::ProjectDeserializer {
public:
    nv::core::Result<nv::core::Project> deserialize(const std::string& inputPath) override;
    nv::core::Result<nv::core::SchemaVersion> readSchemaVersion(const std::string& path) override;
};

} // namespace nv::infrastructure