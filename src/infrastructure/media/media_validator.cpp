#include "media_validator.h"
#include <filesystem>
#include <fstream>

namespace nv::infrastructure {

bool MediaValidator::fileExists(const std::string& filePath) {
    return std::filesystem::exists(filePath);
}

bool MediaValidator::isReadable(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

int64_t MediaValidator::fileSize(const std::string& filePath) {
    if (!fileExists(filePath)) {
        return -1;
    }
    return static_cast<int64_t>(std::filesystem::file_size(filePath));
}

MediaValidator::ValidationResult MediaValidator::validate(const std::string& filePath) {
    ValidationResult result;
    result.exists = fileExists(filePath);
    
    if (!result.exists) {
        result.error = "File does not exist: " + filePath;
        return result;
    }
    
    result.readable = isReadable(filePath);
    if (!result.readable) {
        result.error = "File is not readable: " + filePath;
        return result;
    }
    
    result.size = fileSize(filePath);
    return result;
}

} // namespace nv::infrastructure
