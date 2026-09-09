#pragma once
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "../id.h"
namespace nv::core {
class ProjectInfo {
public:
    ProjectInfo() : id_(generateId()), name_("Untitled Project"), version_("0.0.1-dev") { touch(); createdAt_ = modifiedAt_; }
    const std::string& id() const { return id_; }
    void setId(const std::string& id) { id_ = id; }
    const std::string& name() const { return name_; }
    void setName(const std::string& name) { name_ = name; touch(); }
    const std::string& createdAt() const { return createdAt_; }
    const std::string& modifiedAt() const { return modifiedAt_; }
    void touch() {
        auto now = std::chrono::system_clock::now(); auto t = std::chrono::system_clock::to_time_t(now); std::tm tm_now;
        gmtime_r(&t, &tm_now); std::ostringstream oss; oss << std::put_time(&tm_now, "%Y-%m-%dT%H:%M:%SZ"); modifiedAt_ = oss.str();
    }
    const std::string& version() const { return version_; }
    void setVersion(const std::string& v) { version_ = v; }
    void setId(const std::string& id) { id_ = id; }
private:
    std::string id_, name_, createdAt_, modifiedAt_, version_;
};
}
