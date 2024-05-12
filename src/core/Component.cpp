#include <iostream>
#include <functional>
#include <vector>
#include <memory>

#include "core/Component.h"
#include "core/Signal.h"

class Column : public Container {
};

// Horizontal box: a container that arranges its children horizontally
class HBox : public Container {
public:
    using Container::Container;  // Inherit the constructor
};

// Vertical box: a container that arranges its children vertically
class VBox : public Container {
public:
    using Container::Container;  // Inherit the constructor
};

// A simple text input component
class TextInput : public Component {
};

// A list component that might contain list items
class List : public Component {
};

class Button : public Component {
public:
    Button(std::string label)
        : label(label), onClick(nullptr) {
    }

    Button(std::string label, std::function<void()> onClick)
        : label(label), onClick(onClick) {
    }

    virtual void click() {
        if (onClick)
            onClick();
    }

private:
    std::string label;
    std::function<void()> onClick;
};

class MyButton : public Button {
public:
    void click() override {
        std::cout << "Do Something?\n";
    }
};

int test() {
    Signal count(0);
    
    Container root {
        HBox {
            VBox {
                Button {
                    "Increment",
                    [&count]() { count(count() + 1); }
                },
                TextInput { },
                List { }
            },
            VBox {
                TextInput { },
                TextInput { }
            }
        },        
    };

    return 0;
}
