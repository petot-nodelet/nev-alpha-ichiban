#include "nvcore/project_info.h"
#include "nvcore/id.h"
#include <sstream>
#include <iomanip>
#include <ctime>

namespace nv::core {

ProjectInfo::ProjectInfo() {
    id_ = generateId();
    name_ = "Untitled Project";
    version_ = "0.0.1-dev";
    touch();
    createdAt_ = modifiedAt_;
}

void ProjectInfo::touch() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    gmtime_r(&time_t_now, &tm_now); // Thread-safe UTC

    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%dT%H:%M:%SZ");
    modifiedAt_ = oss.str();
}

} // namespace nv::core