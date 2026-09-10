#include "media_importer.h"
#include <filesystem>
#include <fstream>

namespace nv::infrastructure {

nv::Result<std::string> MediaImporter::importFile(
    const std::string& sourcePath,
    const std::string& projectFolder,
    ImportMode mode
) {
    // TODO: Implement file import logic
    // Reference: Just return the path
    // Copy: Copy file to projectFolder/assets/
    // Move: Move file to projectFolder/assets/
    
    (void)sourcePath;
    (void)projectFolder;
    (void)mode;
    
    return nv::Result<std::string>::Err(
        nv::ErrorCode::UnsupportedOperation,
        "MediaImporter::importFile not implemented yet"
    );
}

nv::core::AssetType MediaImporter::detectAssetType(const std::string& filePath) {
    std::string ext = std::filesystem::path(filePath).extension().string();
    
    // Video extensions
    if (ext == ".mp4" || ext == ".mov" || ext == ".avi" || ext == ".mkv" || ext == ".webm") {
        return nv::core::AssetType::Video;
    }
    
    // Audio extensions
    if (ext == ".mp3" || ext == ".wav" || ext == ".aac" || ext == ".ogg" || ext == ".flac") {
        return nv::core::AssetType::Audio;
    }
    
    // Image extensions
    if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".gif" || ext == ".bmp") {
        return nv::core::AssetType::Image;
    }
    
    // Default to video
    return nv::core::AssetType::Video;
}

std::string MediaImporter::makeRelativePath(
    const std::string& absolutePath,
    const std::string& projectFolder
) {
    try {
        std::filesystem::path abs(absolutePath);
        std::filesystem::path proj(projectFolder);
        std::filesystem::path rel = std::filesystem::relative(abs, proj);
        return rel.string();
    } catch (const std::exception&) {
        return absolutePath;
    }
}

} // namespace nv::infrastructure
