#pragma once
#include <cstdint>
namespace nv {
using TimeMs = std::int64_t;
using DurationMs = std::int64_t;
using PixelWidth = std::int32_t;
using PixelHeight = std::int32_t;
using AssetId = std::uint64_t;
using TrackId = std::uint64_t;
using ClipId = std::uint64_t;
using CompositionId = std::uint64_t;
constexpr TimeMs INVALID_TIME = -1;
}
