# 1. Fix asset.h (Pastikan setId berada DI DALAM class)
cat > src/core/include/nvcore/project/asset.h << 'EOF'
#pragma once
#include <string>
#include <cstdint>
#include "asset_metadata.h"
#include "../id.h"

namespace nv::core {

enum class AssetType { Video, Audio, Image };
enum class PathType { Relative, Absolute };

class Asset {
public:
    Asset() : id_(std::stoull(generateId().substr(0, 16), nullptr, 16)), type_(AssetType::Video), pathType_(PathType::Relative), isExternal_(false) {}
    
    uint64_t id() const { return id_; }
    void setId(uint64_t id) { id_ = id; } // <-- INI HARUS DI DALAM CLASS
    
    AssetType type() const { return type_; }
    void setType(AssetType t) { type_ = t; }
    
    const std::string& path() const { return path_; }
    void setPath(const std::string& p, PathType pt) { 
        path_ = p; 
        pathType_ = pt; 
        if (pt == PathType::Absolute) isExternal_ = true; 
    }
    
    PathType pathType() const { return pathType_; }
    bool isExternal() const { return isExternal_; }
    void setExternal(bool e) { isExternal_ = e; }
    
    const AssetMetadata& metadata() const { return metadata_; }
    AssetMetadata& metadata() { return metadata_; }

private:
    uint64_t id_; 
    AssetType type_; 
    std::string path_; 
    PathType pathType_; 
    bool isExternal_; 
    AssetMetadata metadata_;
};

} // namespace nv::core
EOF

# 2. Fix zip_project_serializer.cpp (Gunakan nv::Result, bukan nv::core::Result)
cat > src/infrastructure/serialization/zip_project_serializer.cpp << 'EOF'
#include "zip_project_serializer.h"
#include <nlohmann/json.hpp>
#include <zip.h>
#include <fstream>
#include <filesystem>

namespace nv::infrastructure {

using json = nlohmann::json;

// Helper: Convert Project to JSON
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
    (void)project;
    (void)outputPath;
    try {
        json j = projectToJson(project);
        std::string jsonStr = j.dump(4);

        std::string tempPath = outputPath + ".tmp";
        {
            std::ofstream ofs(tempPath);
            if (!ofs) return nv::Result<void>::Err(nv::ErrorCode::IoError, "Failed to open temp file for writing");
            ofs << jsonStr;
        }

        int err = 0;
        zip_t* z = zip_open(tempPath.c_str(), ZIP_CREATE | ZIP_TRUNCATE, &err);
        if (!z) return nv::Result<void>::Err(nv::ErrorCode::IoError, "Failed to create ZIP archive");

        zip_source_t* src = zip_source_buffer_create(jsonStr.data(), jsonStr.size(), 0, &err);
        if (!src) {
            zip_close(z);
            return nv::Result<void>::Err(nv::ErrorCode::IoError, "Failed to create ZIP source");
        }

        if (zip_file_add(z, "project.json", src, ZIP_FL_OVERWRITE) < 0) {
            zip_source_free(src);
            zip_close(z);
            return nv::Result<void>::Err(nv::ErrorCode::SerializationError, "Failed to add project.json to ZIP");
        }

        if (zip_close(z) < 0) {
            return nv::Result<void>::Err(nv::ErrorCode::IoError, "Failed to close ZIP archive");
        }

        if (std::rename(tempPath.c_str(), outputPath.c_str()) != 0) {
            return nv::Result<void>::Err(nv::ErrorCode::IoError, "Failed to rename temp file to final output");
        }

        return nv::Result<void>::Ok();

    } catch (const std::exception& e) {
        return nv::Result<void>::Err(nv::ErrorCode::InternalError, std::string("Serialization exception: ") + e.what());
    }
}

} // namespace nv::infrastructure
EOF

# 3. Fix zip_project_deserializer.cpp (Gunakan nv::Result)
cat > src/infrastructure/serialization/zip_project_deserializer.cpp << 'EOF'
#include "zip_project_deserializer.h"
#include <nlohmann/json.hpp>
#include <zip.h>
#include <fstream>
#include <filesystem>

namespace nv::infrastructure {

using json = nlohmann::json;

static nv::core::Result<nv::core::Project> jsonToProject(const json& j) {
    nv::core::Project p;

    if (!j.contains("schema_version") || !j["schema_version"].is_string()) {
        return nv::Result<nv::core::Project>::Err(nv::ErrorCode::ParseError, "Missing or invalid schema_version");
    }
    std::string ver = j["schema_version"].get<std::string>();
    if (ver != "1.0.0") {
        return nv::Result<nv::core::Project>::Err(nv::ErrorCode::UnsupportedOperation, "Unsupported schema version: " + ver);
    }

    if (j.contains("project")) {
        const auto& pj = j["project"];
        p.info().setId(pj.value("id", ""));
        p.info().setName(pj.value("name", "Untitled"));
        p.info().setCreatedAt(pj.value("created_at", ""));
        p.info().setModifiedAt(pj.value("modified_at", ""));
        p.info().setVersion(pj.value("version", ""));
    }

    if (j.contains("settings")) {
        const auto& s = j["settings"];
        p.settings().setWidth(s.value("width", 1920));
        p.settings().setHeight(s.value("height", 1080));
        p.settings().setFps(s.value("fps_numerator", 30), s.value("fps_denominator", 1));
        p.settings().setDurationMs(s.value("duration_ms", 60000));
        p.settings().setBackgroundColor(s.value("background_color", "#000000FF"));
    }

    if (j.contains("assets") && j["assets"].is_array()) {
        for (const auto& a : j["assets"]) {
            auto* asset = p.addAsset(nv::core::AssetType::Video);
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
    (void)inputPath;
    try {
        int err = 0;
        zip_t* z = zip_open(inputPath.c_str(), 0, &err);
        if (!z) return nv::Result<nv::core::Project>::Err(nv::ErrorCode::IoError, "Failed to open .nvp file");

        zip_stat_t st;
        if (zip_stat(z, "project.json", 0, &st) != 0) {
            zip_close(z);
            return nv::Result<nv::core::Project>::Err(nv::ErrorCode::NotFound, "project.json not found in archive");
        }

        if (std::string(st.name).find("..") != std::string::npos) {
            zip_close(z);
            return nv::Result<nv::core::Project>::Err(nv::ErrorCode::InternalError, "Path traversal detected");
        }

        zip_file_t* f = zip_fopen(z, "project.json", 0);
        if (!f) {
            zip_close(z);
            return nv::Result<nv::core::Project>::Err(nv::ErrorCode::IoError, "Failed to open project.json inside ZIP");
        }

        std::string content(st.size, '\0');
        zip_fread(f, content.data(), st.size);
        zip_fclose(f);
        zip_close(z);

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
EOF
