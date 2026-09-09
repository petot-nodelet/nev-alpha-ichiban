#pragma once

// Fundamental type aliases for NirvanaEdit.
// Core domain uses explicit types to avoid primitive obsession.

#include <cstdint>
#include <cstddef>

namespace nv {

// Time and frame types
using FrameIndex   = std::int64_t;   // Frame number (>= 0)
using TimeMs       = std::int64_t;   // Milliseconds (>= 0)
using DurationMs   = std::int64_t;   // Duration in ms (> 0)
using SampleRate   = std::int32_t;   // Audio sample rate (Hz)

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
