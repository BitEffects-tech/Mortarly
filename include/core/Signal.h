#pragma once

#include <functional>

/**
 * @brief Indicates a reactable type
 * 
 * @tparam T The return type of the reactable
 */
template<typename T>
using Reactable = std::function<T()>;

/**
 * @brief Reactable wrapper for a constant value
 * 
 * @tparam T The return type of the reactable
 */
template<typename T>
class ConstantValue {
public:
    ConstantValue(const T& value)
        : value(value)
    {
    }

    const T& operator() () const {
        return value;
    }

private:
    T value;
};

/**
 * @brief Accessor that tracks the state of any accessed signals
 * 
 * @tparam T The return type of the accessor
 */
template<typename T>
class Computed {
public:
    Computed(const std::function<const T&()> &callback)
        : callback(std::move(callback)) {
    }

    const T& operator() () const {
        return callback();
    }

private:
    std::function<const T&()> callback;
};

/**
 * @brief A reactive variable that can be subscribed to
 * 
 * @tparam T The return type of the Signal
 */
template<typename T>
class Signal {
public:
    Signal(const T& val) : value(val) {
    }

    const T& operator() () const {
        return value;
    }

    void operator()(const T& val) {
        value = val;
    }

private:
    T value;
};
