#pragma once
#include <string>
#include <chrono>

namespace nv::core {

class ProjectInfo {
public:
    ProjectInfo();

    const std::string& id() const { return id_; }
    const std::string& name() const { return name_; }
    void setName(const std::string& name) { name_ = name; touch(); }

    const std::string& createdAt() const { return createdAt_; }
    const std::string& modifiedAt() const { return modifiedAt_; }
    void touch(); // Update modifiedAt to current ISO 8601 time

    const std::string& version() const { return version_; }
    void setVersion(const std::string& version) { version_ = version; }

private:
    std::string id_;
    std::string name_;
    std::string createdAt_;
    std::string modifiedAt_;
    std::string version_;
};

} // namespace nv::core
