#pragma once

#include <cstdint>
#include <cstddef>

namespace nv {

// Time and frame types
using FrameIndex   = std::int64_t;
using TimeMs       = std::int64_t;
using DurationMs   = std::int64_t;
using SampleRate   = std::int32_t;

// Identifiers
using AssetId      = std::uint64_t;
using TrackId      = std::uint64_t;
using ClipId       = std::uint64_t;
using EffectId     = std::uint64_t;
using KeyframeId   = std::uint64_t;

// Dimensions
using PixelWidth   = std::int32_t;
using PixelHeight  = std::int32_t;

// Constants
constexpr FrameIndex INVALID_FRAME = -1;
constexpr TimeMs     INVALID_TIME  = -1;

} // namespace nv
