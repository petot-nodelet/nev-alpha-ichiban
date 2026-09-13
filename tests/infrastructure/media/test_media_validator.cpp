#include "framework/test_framework.h"
#include "media/media_validator.h"
#include <fstream>
#include <filesystem>

NV_TEST_CASE(MediaValidator_FileExists) {
    std::string testFile = "/tmp/nirvanaedit_test_exists.tmp";
    {
        std::ofstream ofs(testFile);
        ofs << "test";
    }
    
    NV_ASSERT(nv::infrastructure::MediaValidator::fileExists(testFile));
    
    std::filesystem::remove(testFile);
}

NV_TEST_CASE(MediaValidator_FileNotExists) {
    NV_ASSERT(!nv::infrastructure::MediaValidator::fileExists("/tmp/nirvanaedit_nonexistent_12345.tmp"));
}

NV_TEST_CASE(MediaValidator_Validate) {
    std::string testFile = "/tmp/nirvanaedit_test_validate.tmp";
    {
        std::ofstream ofs(testFile);
        ofs << "test content for validation";
    }
    
    auto result = nv::infrastructure::MediaValidator::validate(testFile);
    NV_ASSERT(result.exists);
    NV_ASSERT(result.readable);
    NV_ASSERT(result.size > 0);
    
    std::filesystem::remove(testFile);
}
