#include "framework/test_framework.h"
#include "nvcore/version.h"

NV_TEST_CASE(Version_toString_stable) {
    nv::Version v{1, 2, 3, ""};
    NV_ASSERT_EQ(v.toString(), std::string("1.2.3"));
}

NV_TEST_CASE(Version_toString_prerelease) {
    nv::Version v{0, 0, 1, "dev"};
    NV_ASSERT_EQ(v.toString(), std::string("0.0.1-dev"));
}

NV_TEST_CASE(Version_toInt) {
    nv::Version v{1, 2, 3, ""};
    NV_ASSERT_EQ(v.toInt(), 10203);
}

NV_TEST_CASE(CurrentVersion_isDefined) {
    const auto& v = nv::currentVersion();
    NV_ASSERT(v.major >= 0);
    NV_ASSERT(v.minor >= 0);
    NV_ASSERT(v.patch >= 0);
}

NV_TEST_CASE(ProjectFormatVersion_is1_0_0) {
    const auto& v = nv::projectFormatVersion();
    NV_ASSERT_EQ(v.major, 1);
    NV_ASSERT_EQ(v.minor, 0);
    NV_ASSERT_EQ(v.patch, 0);
}
