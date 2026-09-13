#pragma once
#include <variant>
#include <string>
#include <ostream>

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

// FIX: Tambahkan operator<< agar ErrorCode bisa dicetak oleh test framework
inline std::ostream& operator<<(std::ostream& os, ErrorCode code) {
    switch (code) {
        case ErrorCode::Ok: return os << "Ok";
        case ErrorCode::InvalidArgument: return os << "InvalidArgument";
        case ErrorCode::NotFound: return os << "NotFound";
        case ErrorCode::AlreadyExists: return os << "AlreadyExists";
        case ErrorCode::IoError: return os << "IoError";
        case ErrorCode::ParseError: return os << "ParseError";
        case ErrorCode::SerializationError: return os << "SerializationError";
        case ErrorCode::MediaError: return os << "MediaError";
        case ErrorCode::UnsupportedOperation: return os << "UnsupportedOperation";
        case ErrorCode::InternalError: return os << "InternalError";
        default: return os << "UnknownErrorCode(" << static_cast<int>(code) << ")";
    }
}

struct Error {
    ErrorCode code;
    std::string message;
    Error(ErrorCode c, std::string m) : code(c), message(std::move(m)) {}
};

template <typename T>
class Result {
public:
    static Result Ok(T v) { Result r; r.storage_ = std::move(v); return r; }
    static Result Err(Error e) { Result r; r.storage_ = std::move(e); return r; }
    static Result Err(ErrorCode c, std::string m) { return Err(Error{c, std::move(m)}); }
    bool isOk() const { return std::holds_alternative<T>(storage_); }
    bool isErr() const { return std::holds_alternative<Error>(storage_); }
    const T& value() const { return std::get<T>(storage_); }
    const Error& error() const { return std::get<Error>(storage_); }
private:
    std::variant<T, Error> storage_;
};

template <>
class Result<void> {
public:
    static Result Ok() { Result r; r.ok_ = true; return r; }
    static Result Err(Error e) { Result r; r.err_ = std::move(e); r.ok_ = false; return r; }
    static Result Err(ErrorCode c, std::string m) { return Err(Error{c, std::move(m)}); }
    bool isOk() const { return ok_; }
    bool isErr() const { return !ok_; }
    const Error& error() const { return err_; }
private:
    bool ok_ = false;
    Error err_{ErrorCode::InternalError, "uninitialized"};
};

} // namespace nv
