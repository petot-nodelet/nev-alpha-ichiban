#pragma once

#include "nvcore/result.h"
#include <string>

namespace nv::infrastructure {

/**
 * MediaValidator - Validate and check media files
 * 
 * Responsibilities:
 * - Check if file exists
 * - Check if file is readable
 * - Get file size
 * - Check if file is accessible
 */
class MediaValidator {
public:
    /**
     * Check if file exists on disk
     * @param filePath Path to the file
     * @return true if file exists, false otherwise
     */
    static bool fileExists(const std::string& filePath);
    
    /**
     * Check if file is readable
     * @param filePath Path to the file
     * @return true if readable, false otherwise
     */
    static bool isReadable(const std::string& filePath);
    
    /**
     * Get file size in bytes
     * @param filePath Path to the file
     * @return File size in bytes, or -1 if error
     */
    static int64_t fileSize(const std::string& filePath);
    
    /**
     * Validate media file and return detailed status
     * @param filePath Path to the media file
     * @return Result containing validation details or error
     */
    struct ValidationResult {
        bool exists = false;
        bool readable = false;
        int64_t size = 0;
        std::string error;
    };
    
    static ValidationResult validate(const std::string& filePath);
};

} // namespace nv::infrastructure
