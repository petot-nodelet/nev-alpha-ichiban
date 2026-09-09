#pragma once
#include "nvcore/serialization/project_deserializer.h"
namespace nv::infrastructure {
class ZipProjectDeserializer : public nv::core::ProjectDeserializer {
public:
    nv::Result<nv::core::Project> deserialize(const std::string& inputPath) override;
    nv::Result<nv::core::SchemaVersion> readSchemaVersion(const std::string& path) override;
};
}
