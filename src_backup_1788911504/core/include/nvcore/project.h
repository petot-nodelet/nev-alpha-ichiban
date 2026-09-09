#pragma once
#include <algorithm>
#include <cstdint>
#include <vector>
#include <memory>
#include "project_info.h"
#include "project_setting.h"
#include "asset.h"
#include "composition.h"

namespace nv::core {

class Project {
public:
    Project();

    ProjectInfo& info() { return info_; }
    const ProjectInfo& info() const { return info_; }

    ProjectSettings& settings() { return settings_; }
    const ProjectSettings& settings() const { return settings_; }

    const std::vector<std::unique_ptr<Asset>>& assets() const { return assets_; }
    
    Asset* addAsset(AssetType type) {
        auto asset = std::make_unique<Asset>();
        asset->setType(type);
        Asset* ptr = asset.get();
        assets_.push_back(std::move(asset));
        info_.touch();
        return ptr;
    }

    void removeAsset(uint64_t assetId) {
        assets_.erase(
            std::remove_if(assets_.begin(), assets_.end(), 
                [assetId](const std::unique_ptr<Asset>& a) { return a->id() == assetId; }),
            assets_.end()
        );
        info_.touch();
    }

    Asset* getAsset(uint64_t assetId) {
        for (auto& a : assets_) {
            if (a->id() == assetId) return a.get();
        }
        return nullptr;
    }

    const std::vector<std::unique_ptr<Composition>>& compositions() const { return compositions_; }
    
    Composition* addComposition() {
        auto comp = std::make_unique<Composition>();
        Composition* ptr = comp.get();
        compositions_.push_back(std::move(comp));
        info_.touch();
        return ptr;
    }

    void removeComposition(uint64_t compId) {
        compositions_.erase(
            std::remove_if(compositions_.begin(), compositions_.end(), 
                [compId](const std::unique_ptr<Composition>& c) { return c->id() == compId; }),
            compositions_.end()
        );
        info_.touch();
    }

    Composition* getComposition(uint64_t compId) {
        for (auto& c : compositions_) {
            if (c->id() == compId) return c.get();
        }
        return nullptr;
    }

private:
    ProjectInfo info_;
    ProjectSettings settings_;
    std::vector<std::unique_ptr<Asset>> assets_;
    std::vector<std::unique_ptr<Composition>> compositions_;
};

} // namespace nv::core