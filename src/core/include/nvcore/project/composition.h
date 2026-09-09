#pragma once
#include <string>
#include <cstdint>
#include "timeline.h"
#include "../id.h"
namespace nv::core {
class Composition {
public:
    Composition() : id_(std::stoull(generateId().substr(0, 16), nullptr, 16)) {}
    uint64_t id() const { return id_; }
    void setId(uint64_t id) { id_ = id; }
    const std::string& name() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    Timeline& timeline() { return timeline_; }
    const Timeline& timeline() const { return timeline_; }
    void setId(uint64_t id) { id_ = id; }
private:
    uint64_t id_; std::string name_ = "Main Composition"; Timeline timeline_;
};
}
