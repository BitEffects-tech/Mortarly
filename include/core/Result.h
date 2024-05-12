#pragma once

#include <memory>
#include <optional>
#include <string>
#include <functional>
#include <variant>

/**
 * @brief A monad-like Result class that can contain either a Component pointer or an error message
 * 
 * @tparam T The optional object type
 */
template<typename T>
class Result {
public:
    using ValueType = std::shared_ptr<T>;
    using ErrorType = std::string;

private:
    std::variant<ValueType, ErrorType> content;

public:
    /** Constructor for success */
    explicit Result(T* success) : content(std::make_shared<T>(std::move(*success))) {}
    /** Constructor for success */
    explicit Result(ValueType success) : content(std::move(success)) {}

    /** Constructor for error */
    explicit Result(const char* error) : content(std::string(error)) {}
    /** Constructor for error */
    explicit Result(const ErrorType& error) : content(error) {}
    /** Constructor for error */
    explicit Result(ErrorType&& error) : content(std::move(error)) {}

    /** Check if this result was successful */
    bool isSuccess() const {
        return std::holds_alternative<ValueType>(content);
    }

    /** accessor for the success value */
    const ValueType& value() const {
        return std::get<ValueType>(content);
    }

    /** accessor for the error value */
    const ErrorType& error() const {
        return std::get<ErrorType>(content);
    }

    /**
     * @brief If the result is valid, perform an action
     * 
     * @param successCallback Callback function to call if this is a valid result
     * @return this result for chaining
     */
    const Result<T>& result(std::function<void(const T&)> successCallback) const {
        if (isSuccess()) {
            successCallback(*value());
        }

        return *this;
    }

    /**
     * @brief If the result is not valid, perform an action
     * 
     * @param successCallback Callback function to call if this is not a valid result
     * @return this result for chaining
     */
    const Result<T>& error(std::function<void(const ErrorType&)> errorCallback) const {
        if (!isSuccess()) {
            errorCallback(error());
        }
        
        return *this;
    }
};
