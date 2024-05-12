#pragma once

#include "core/Component.h"

/**
 * @brief A simple button control
 */
class Button : public Component {
private:
    std::string label;
    std::function<void()> onClick;

public:
    /**
     * @brief Construct a new Button object
     * 
     * @param label The text that shows on the button
     * @param onClick The callback for when the user activates the button
     */
    Button(const std::string &label, std::function<void()> onClick)
        : label(std::move(label)), onClick(onClick) {

    }

    /**
     * @brief Fire the callback for this button.
     * 
     */
    virtual void click() {
        onClick();
    }
};
