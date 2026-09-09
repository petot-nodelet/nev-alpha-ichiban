#include "zip_project_serializer.h"
#include <nlohmann/json.hpp>
#include <zip.h>
#include <fstream>
#include <filesystem>
#include <iostream>

namespace nv::infrastructure {

using json = nlohmann::json;

static json projectToJson(const nv::core::Project& p) {
    json j;
    j["schema_version"] = "1.0.0";

    json projInfo;
    projInfo["id"] = p.info().id();
    projInfo["name"] = p.info().name();
    projInfo["created_at"] = p.info().createdAt();
    projInfo["modified_at"] = p.info().modifiedAt();
    projInfo["version"] = p.info().version();
    j["project"] = projInfo;

    json settings;
    settings["width"] = p.settings().width();
    settings["height"] = p.settings().height();
    settings["fps_numerator"] = p.settings().fpsNumerator();
    settings["fps_denominator"] = p.settings().fpsDenominator();
    settings["duration_ms"] = p.settings().durationMs();
    settings["background_color"] = p.settings().backgroundColor();
    j["settings"] = settings;

    json assets = json::array();
    for (const auto& asset : p.assets()) {
        json a;
        a["id"] = std::to_string(asset->id());
        a["type"] = (asset->type() == nv::core::AssetType::Video) ? "video" : 
                    (asset->type() == nv::core::AssetType::Audio) ? "audio" : "image";
        a["path"] = asset->path();
        a["path_type"] = (asset->pathType() == nv::core::PathType::Relative) ? "relative" : "absolute";
        a["is_external"] = asset->isExternal();
        
        json meta;
        meta["duration_ms"] = asset->metadata().duration_ms;
        meta["width"] = asset->metadata().width;
        meta["height"] = asset->metadata().height;
        meta["fps_numerator"] = asset->metadata().fps_numerator;
        meta["fps_denominator"] = asset->metadata().fps_denominator;
        meta["has_audio"] = asset->metadata().has_audio;
        meta["codec"] = asset->metadata().codec;
        a["metadata"] = meta;
        
        assets.push_back(a);
    }
    j["assets"] = assets;

    json comps = json::array();
    for (const auto& comp : p.compositions()) {
        json c;
        c["id"] = std::to_string(comp->id());
        c["name"] = comp->name();
        
        json timeline;
        json tracks = json::array();
        for (const auto& track : comp->timeline().tracks()) {
            json t;
            t["id"] = std::to_string(track->id());
            t["type"] = (track->type() == nv::core::TrackType::Video) ? "video" : "audio";
            
            json clips = json::array();
            for (const auto& clip : track->clips()) {
                json cl;
                cl["id"] = std::to_string(clip->id());
                cl["asset_id"] = std::to_string(clip->assetId());
                cl["start_time_ms"] = clip->startTimeMs();
                cl["end_time_ms"] = clip->endTimeMs();
                cl["position_ms"] = clip->positionMs();
                cl["track_id"] = std::to_string(clip->trackId());
                clips.push_back(cl);
            }
            t["clips"] = clips;
            tracks.push_back(t);
        }
        timeline["tracks"] = tracks;
        c["timeline"] = timeline;
        comps.push_back(c);
    }
    j["compositions"] = comps;

    return j;
}

nv::Result<void> ZipProjectSerializer::serialize(const nv::core::Project& project, const std::string& outputPath) {
    try {
        // 1. Convert to JSON
        json j = projectToJson(project);
        std::string jsonStr = j.dump(4);

        // 2. Write JSON to temp file first
        std::string tempPath = outputPath + ".tmp";
        {
            std::ofstream ofs(tempPath);
            if (!ofs) {
                return nv::Result<void>::Err(nv::ErrorCode::IoError, "Failed to open temp file: " + tempPath);
            }
            ofs << jsonStr;
            if (!ofs.good()) {
                return nv::Result<void>::Err(nv::ErrorCode::IoError, "Failed to write to temp file");
            }
        }

        // 3. Create ZIP archive
        int err = 0;
        zip_t* z = zip_open(tempPath.c_str(), ZIP_CREATE | ZIP_TRUNCATE, &err);
        if (!z) {
            return nv::Result<void>::Err(nv::ErrorCode::IoError, "Failed to create ZIP archive");
        }

        // 4. Add project.json to ZIP
        // FIX: Use nullptr for zip_error_t* argument (libzip 1.x API)
        zip_source_t* src = zip_source_buffer_create(jsonStr.data(), jsonStr.size(), 0, nullptr);
        if (!src) {
            zip_close(z);
            return nv::Result<void>::Err(nv::ErrorCode::IoError, "Failed to create ZIP source buffer");
        }

        if (zip_file_add(z, "project.json", src, ZIP_FL_OVERWRITE) < 0) {
            zip_source_free(src);
            zip_close(z);
            return nv::Result<void>::Err(nv::ErrorCode::SerializationError, "Failed to add project.json to ZIP");
        }

        // 5. Close ZIP (this writes the archive)
        if (zip_close(z) < 0) {
            return nv::Result<void>::Err(nv::ErrorCode::IoError, "Failed to close ZIP archive");
        }

        // 6. Rename temp to final (atomic replace)
        if (std::rename(tempPath.c_str(), outputPath.c_str()) != 0) {
            return nv::Result<void>::Err(nv::ErrorCode::IoError, "Failed to rename temp file to final output");
        }

        return nv::Result<void>::Ok();

    } catch (const std::exception& e) {
        return nv::Result<void>::Err(nv::ErrorCode::InternalError, std::string("Serialization exception: ") + e.what());
    }
}

} // namespace nv::infrastructure
