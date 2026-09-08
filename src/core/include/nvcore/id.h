#pragma once
#include <string>
#include <random>
#include <sstream>
#include <iomanip>

namespace nv::core {

inline std::string generateId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint64_t> dis;

    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    oss << std::setw(16) << dis(gen);
    oss << std::setw(16) << dis(gen);
    return oss.str();
}

} // namespace nv::core
