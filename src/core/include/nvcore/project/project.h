#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "project_info.h"
#include "project_settings.h"
#include "asset.h"
#include "composition.h"
namespace nv::core {
class Project {
public:
    Project() { addComposition(); }
    ProjectInfo& info() { return info_; }
    const ProjectInfo& info() const { return info_; }
    ProjectSettings& settings() { return settings_; }
    const ProjectSettings& settings() const { return settings_; }
    const std::vector<std::unique_ptr<Asset>>& assets() const { return assets_; }
    Asset* addAsset(AssetType type) {
        auto asset = std::make_unique<Asset>(); asset->setType(type);
        Asset* ptr = asset.get(); assets_.push_back(std::move(asset)); info_.touch(); return ptr;
    }
    void removeAsset(uint64_t assetId) {
        assets_.erase(std::remove_if(assets_.begin(), assets_.end(), [assetId](const std::unique_ptr<Asset>& a) { return a->id() == assetId; }), assets_.end());
        info_.touch();
    }
    Asset* getAsset(uint64_t assetId) {
        for (auto& a : assets_) { if (a->id() == assetId) return a.get(); } return nullptr;
    }
    const std::vector<std::unique_ptr<Composition>>& compositions() const { return compositions_; }
    Composition* addComposition() {
        auto comp = std::make_unique<Composition>(); Composition* ptr = comp.get();
        compositions_.push_back(std::move(comp)); info_.touch(); return ptr;
    }
    void clear() { 
        assets_.clear(); 
        compositions_.clear(); 
    }
private:
    ProjectInfo info_; ProjectSettings settings_;
    std::vector<std::unique_ptr<Asset>> assets_;
    std::vector<std::unique_ptr<Composition>> compositions_;
};
}
