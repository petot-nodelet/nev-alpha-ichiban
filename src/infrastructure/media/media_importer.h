#pragma once

#include "nvcore/project/asset.h"
#include "nvcore/result.h"
#include <string>

namespace nv::infrastructure {

/**
 * MediaImporter - Import media files into project
 * 
 * Import modes:
 * - Reference: Keep original location, store path only
 * - Copy: Copy file to project folder
 * - Move: Move file to project folder
 */
class MediaImporter {
public:
    enum class ImportMode {
        Reference,  // Keep original location
        Copy,       // Copy to project folder
        Move        // Move to project folder
    };
    
    /**
     * Import a media file
     * @param sourcePath Path to source media file
     * @param projectFolder Path to project folder
     * @param mode Import mode (default: Reference)
     * @return Result containing the final path of imported file
     */
    static nv::Result<std::string> importFile(
        const std::string& sourcePath,
        const std::string& projectFolder,
        ImportMode mode = ImportMode::Reference
    );
    
    /**
     * Detect asset type from file extension and content
     * @param filePath Path to the file
     * @return Detected AssetType
     */
    static nv::core::AssetType detectAssetType(const std::string& filePath);
    
    /**
     * Generate relative path from project folder
     * @param absolutePath Absolute path to file
     * @param projectFolder Absolute path to project folder
     * @return Relative path, or absolute if cannot be made relative
     */
    static std::string makeRelativePath(
        const std::string& absolutePath,
        const std::string& projectFolder
    );
};

} // namespace nv::infrastructure
