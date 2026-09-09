#include "framework/test_framework.h"
#include "nvcore/project/project.h"
NV_TEST_CASE(Project_creation) { nv::core::Project p; NV_ASSERT(!p.info().id().empty()); NV_ASSERT_EQ(p.compositions().size(), 1); }
