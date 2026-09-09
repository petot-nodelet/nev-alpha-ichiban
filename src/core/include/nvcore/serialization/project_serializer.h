#pragma once
#include "../project/project.h"
#include "../result.h"
#include "schema_version.h"
#include <string>
namespace nv::core {
class ProjectSerializer {
public:
    virtual ~ProjectSerializer() = default;
    virtual Result<void> serialize(const Project& project, const std::string& outputPath) = 0;
};
class ProjectDeserializer {
public:
    virtual ~ProjectDeserializer() = default;
    virtual Result<Project> deserialize(const std::string& inputPath) = 0;
    virtual Result<SchemaVersion> readSchemaVersion(const std::string& path) = 0;
};
}
