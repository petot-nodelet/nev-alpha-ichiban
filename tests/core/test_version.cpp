#include "framework/test_framework.h"
#include "nvcore/version.h"
NV_TEST_CASE(Version_toString) { nv::Version v{1, 2, 3, "dev"}; NV_ASSERT_EQ(v.toString(), std::string("1.2.3-dev")); }
NV_TEST_CASE(CurrentVersion_isDefined) { const auto& v = nv::currentVersion(); NV_ASSERT(v.major >= 0); }
