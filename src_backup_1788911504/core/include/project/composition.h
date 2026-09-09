#pragma once
#include <string>
#include <cstdint>
#include "timeline.h"
#include "nvcore/id.h"
#include <cstdlib>

namespace nv::core {

class Composition {
public:
    Composition() : id_(std::strtoull(generateId().substr(0, 16).c_str(), nullptr, 16)) {}

    uint64_t id() const { return id_; }
    const std::string& name() const { return name_; }
    void setName(const std::string& name) { name_ = name; }

    Timeline& timeline() { return timeline_; }
    const Timeline& timeline() const { return timeline_; }

private:
    uint64_t id_;
    std::string name_ = "Main Composition";
    Timeline timeline_;
};

} // namespace nv::core
