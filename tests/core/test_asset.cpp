#include "framework/test_framework.h"
#include "nvcore/project/project.h"
NV_TEST_CASE(Asset_addition) {
    nv::core::Project p;
    auto* asset = p.addAsset(nv::core::AssetType::Video);
    NV_ASSERT(asset != nullptr);
    NV_ASSERT(!p.assets().empty());
}
