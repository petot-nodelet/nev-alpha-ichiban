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
