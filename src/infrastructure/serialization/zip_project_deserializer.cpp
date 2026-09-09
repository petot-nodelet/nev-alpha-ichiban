#include "zip_project_deserializer.h"
#include <nlohmann/json.hpp>
#include <zip.h>
#include <fstream>
#include <filesystem>

namespace nv::infrastructure {

using json = nlohmann::json;

// Helper: Parse JSON into Project
static nv::Result<nv::core::Project> jsonToProject(const json& j) {
    nv::core::Project p;
    p.clear(); // Clear the default composition created by the constructor

    // 1. Schema Version Check
    if (!j.contains("schema_version") || !j["schema_version"].is_string()) {
        return nv::Result<nv::core::Project>::Err(nv::ErrorCode::ParseError, "Missing or invalid schema_version");
    }
    std::string ver = j["schema_version"].get<std::string>();
    if (ver != "1.0.0") {
        return nv::Result<nv::core::Project>::Err(nv::ErrorCode::UnsupportedOperation, "Unsupported schema version: " + ver);
    }

    // 2. Project Info
    if (j.contains("project")) {
        const auto& pj = j["project"];
        p.info().setId(pj.value("id", ""));
        p.info().setName(pj.value("name", "Untitled"));
        p.info().setCreatedAt(pj.value("created_at", ""));
        p.info().setModifiedAt(pj.value("modified_at", ""));
        p.info().setVersion(pj.value("version", ""));
    }

    // 3. Settings
    if (j.contains("settings")) {
        const auto& s = j["settings"];
        p.settings().setWidth(s.value("width", 1920));
        p.settings().setHeight(s.value("height", 1080));
        p.settings().setFps(s.value("fps_numerator", 30), s.value("fps_denominator", 1));
        p.settings().setDurationMs(s.value("duration_ms", 60000));
        p.settings().setBackgroundColor(s.value("background_color", "#000000FF"));
    }

    // 4. Assets
    if (j.contains("assets") && j["assets"].is_array()) {
        for (const auto& a : j["assets"]) {
            auto* asset = p.addAsset(nv::core::AssetType::Video); // Default type, will override
            
            if (a.contains("id")) asset->setId(std::stoull(a["id"].get<std::string>()));
            
            if (a.contains("type")) {
                std::string t = a["type"];
                if (t == "audio") asset->setType(nv::core::AssetType::Audio);
                else if (t == "image") asset->setType(nv::core::AssetType::Image);
                else asset->setType(nv::core::AssetType::Video);
            }
            
            if (a.contains("path") && a.contains("path_type")) {
                nv::core::PathType pt = (a["path_type"] == "absolute") ? nv::core::PathType::Absolute : nv::core::PathType::Relative;
                asset->setPath(a["path"], pt);
            }
            if (a.contains("is_external")) asset->setExternal(a["is_external"]);

            if (a.contains("metadata")) {
                const auto& m = a["metadata"];
                asset->metadata().duration_ms = m.value("duration_ms", 0);
                asset->metadata().width = m.value("width", 0);
                asset->metadata().height = m.value("height", 0);
                asset->metadata().fps_numerator = m.value("fps_numerator", 0);
                asset->metadata().fps_denominator = m.value("fps_denominator", 0);
                asset->metadata().has_audio = m.value("has_audio", false);
                asset->metadata().codec = m.value("codec", "");
            }
        }
    }

    // 5. Compositions
    if (j.contains("compositions") && j["compositions"].is_array()) {
        for (const auto& c : j["compositions"]) {
            auto* comp = p.addComposition();
            if (c.contains("id")) comp->setId(std::stoull(c["id"].get<std::string>()));
            comp->setName(c.value("name", "Composition"));

            if (c.contains("timeline") && c["timeline"].contains("tracks")) {
                for (const auto& t : c["timeline"]["tracks"]) {
                    nv::core::TrackType tt = (t.value("type", "video") == "audio") ? nv::core::TrackType::Audio : nv::core::TrackType::Video;
                    auto* track = comp->timeline().addTrack(tt);
                    if (t.contains("id")) track->setId(std::stoull(t["id"].get<std::string>()));

                    if (t.contains("clips") && t["clips"].is_array()) {
                        for (const auto& cl : t["clips"]) {
                            auto* clip = track->addClip();
                            if (cl.contains("id")) clip->setId(std::stoull(cl["id"].get<std::string>()));
                            if (cl.contains("asset_id")) clip->setAssetId(std::stoull(cl["asset_id"].get<std::string>()));
                            if (cl.contains("track_id")) clip->setTrackId(std::stoull(cl["track_id"].get<std::string>()));
                            clip->setStartTimeMs(cl.value("start_time_ms", 0));
                            clip->setEndTimeMs(cl.value("end_time_ms", 0));
                            clip->setPositionMs(cl.value("position_ms", 0));
                        }
                    }
                }
            }
        }
    }

    return nv::Result<nv::core::Project>::Ok(std::move(p));
}

nv::Result<nv::core::Project> ZipProjectDeserializer::deserialize(const std::string& inputPath) {
    try {
        int err = 0;
        zip_t* z = zip_open(inputPath.c_str(), 0, &err);
        if (!z) return nv::Result<nv::core::Project>::Err(nv::ErrorCode::IoError, "Failed to open .nvp file");

        zip_stat_t st;
        if (zip_stat(z, "project.json", 0, &st) != 0) {
            zip_close(z);
            return nv::Result<nv::core::Project>::Err(nv::ErrorCode::NotFound, "project.json not found in archive");
        }

        zip_file_t* f = zip_fopen(z, "project.json", 0);
        if (!f) {
            zip_close(z);
            return nv::Result<nv::core::Project>::Err(nv::ErrorCode::IoError, "Failed to open project.json inside ZIP");
        }

        std::string content(st.size, '\0');
        zip_int64_t bytesRead = zip_fread(f, content.data(), st.size);
        zip_fclose(f);
        zip_close(z);

        if (bytesRead < 0) {
            return nv::Result<nv::core::Project>::Err(nv::ErrorCode::IoError, "Failed to read project.json content");
        }
        
        // Resize to actual bytes read just in case
        content.resize(bytesRead);

        json j = json::parse(content, nullptr, false);
        if (j.is_discarded()) {
            return nv::Result<nv::core::Project>::Err(nv::ErrorCode::ParseError, "Invalid JSON in project.json");
        }

        return jsonToProject(j);

    } catch (const json::exception& e) {
        return nv::Result<nv::core::Project>::Err(nv::ErrorCode::ParseError, std::string("JSON parse error: ") + e.what());
    } catch (const std::exception& e) {
        return nv::Result<nv::core::Project>::Err(nv::ErrorCode::InternalError, std::string("Deserialization exception: ") + e.what());
    }
}

nv::Result<nv::core::SchemaVersion> ZipProjectDeserializer::readSchemaVersion(const std::string& path) {
    (void)path;
    return nv::Result<nv::core::SchemaVersion>::Err(nv::ErrorCode::UnsupportedOperation, "Not implemented separately");
}

} // namespace nv::infrastructure
