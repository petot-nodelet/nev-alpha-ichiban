# 1. Pastikan folder include ada
mkdir -p src/core/include/nvcore

# 2. Buat file types.h (dari Phase 0)
cat > src/core/include/nvcore/types.h << 'EOF'
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
EOF

# 3. Buat file result.h (dari Phase 0)
cat > src/core/include/nvcore/result.h << 'EOF'
#pragma once

#include <variant>
#include <string>
#include <utility>

namespace nv {

enum class ErrorCode {
    Ok = 0,
    InvalidArgument,
    NotFound,
    AlreadyExists,
    IoError,
    ParseError,
    SerializationError,
    MediaError,
    UnsupportedOperation,
    InternalError,
};

struct Error {
    ErrorCode code;
    std::string message;

    Error(ErrorCode c, std::string msg)
        : code(c), message(std::move(msg)) {}
};

template <typename T>
class Result {
public:
    static Result Ok(T value) {
        Result r;
        r.storage_ = std::move(value);
        return r;
    }

    static Result Err(Error error) {
        Result r;
        r.storage_ = std::move(error);
        return r;
    }

    static Result Err(ErrorCode code, std::string msg) {
        return Err(Error{code, std::move(msg)});
    }

    bool isOk() const {
        return std::holds_alternative<T>(storage_);
    }

    bool isErr() const {
        return std::holds_alternative<Error>(storage_);
    }

    const T& value() const { return std::get<T>(storage_); }
    T& value() { return std::get<T>(storage_); }

    const Error& error() const { return std::get<Error>(storage_); }

private:
    std::variant<T, Error> storage_;
};

template <>
class Result<void> {
public:
    static Result Ok() {
        Result r;
        r.has_value_ = true;
        return r;
    }

    static Result Err(Error error) {
        Result r;
        r.error_ = std::move(error);
        r.has_value_ = false;
        return r;
    }

    static Result Err(ErrorCode code, std::string msg) {
        return Err(Error{code, std::move(msg)});
    }

    bool isOk() const { return has_value_; }
    bool isErr() const { return !has_value_; }

    const Error& error() const { return error_; }

private:
    bool has_value_ = false;
    Error error_{ErrorCode::InternalError, "uninitialized"};
};

} // namespace nv
EOF

# 4. Buat file version.h (dari Phase 0, tapi update dengan yang baru)
cat > src/core/include/nvcore/version.h << 'EOF'
#pragma once

#include <string>

namespace nv {

struct Version {
    int major;
    int minor;
    int patch;
    std::string preRelease;

    std::string toString() const;
    int toInt() const;
};

const Version& currentVersion();
const Version& projectFormatVersion();

} // namespace nv
EOF

# 5. Pastikan file .cpp untuk version juga ada
touch src/core/src/version.cpp

# 6. Sekarang coba build ulang
cd build
cmake ..
cmake --build . -j$(nproc)
