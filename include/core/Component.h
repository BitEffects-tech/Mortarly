#pragma once

#include <functional>
#include <initializer_list>
#include <optional>
#include <memory>
#include <variant>
#include <vector>

#include <iostream>

#include "Result.h"

/**
 * @brief The base class for components
 */
class Component {
public:
    virtual ~Component() = default;

    /**
     * @brief Call a lambda for every node in the component tree
     * 
     * @param callback Callback function
     */
    virtual void recurse(std::function<void(const Component &)> callback) const {
        callback(*this);
    }
};

/**
 * @brief Container class capable of holding multiple components
 */
class Container : public Component {
    std::vector<std::unique_ptr<Component>> children;
public:
    Container() = default;
    Container(const Container& other) = delete;
    Container(Container&& other) noexcept = default;
    Container& operator=(const Container& other) = delete;
    Container& operator=(Container&& other) noexcept = default;

    /**
     * @brief Constructor to accept any number of arguments of any type derived from Component
     * 
     * @tparam Components 
     * @param components Initializer list of child components
     */
    template<typename... Components>
    Container(Components&&... components) {
        (children.emplace_back(std::make_unique<Components>(std::forward<Components>(components))), ...);
    }

    /**
     * @brief Find a child Component and cast it to the specified type
     * 
     * @param idx The index of the child to be checked
     * @return A Result<Component> monad containing the found object, if any
     */
    template<typename T>
    requires std::derived_from<T, Component>
    Result<T> child(unsigned int idx) const {
        if (idx < children.size()) {
            // Use dynamic_cast to safely attempt casting
            T* derived = dynamic_cast<T*>(children[idx].get());
            if (derived) {
                return Result<T>(derived);
            } else {
                return Result<T>("Type mismatch or dynamic cast failed");
            }
        }
        return Result<T>("Index out of bounds");
    }

    /**
     * @brief Find a child Component
     * 
     * @param idx The index of the child to be returned
     * @return A Result<Component> monad containing the found object, if any
     */
    const Result<Component> child(unsigned int idx) const {
        if (idx < children.size()) {
            return Result<Component>(std::make_shared<Container>(std::move(*children[idx])));
        }
        return Result<Component>("Index out of bounds");
    }

    /**
     * @brief Call a lambda for every node in the component tree
     * 
     * @param callback Callback function
     */
    void recurse(std::function<void(const Component &)> callback) const override {
        Component::recurse(callback);
        for (const auto& child : children) {
            child->recurse(callback);
        }
    }
};
