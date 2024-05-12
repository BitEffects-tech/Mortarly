#include <iostream>

#include "../src/pch.h"
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

#if USING_GTEST


TEST(COMPONENT, Recursion) {
    Container root {
        Container {
            Component {},
            Component {},
        },
        Component {},
        Component {},
        Component {},
    };

    int count = 0;
    root.recurse([&count](const Component& component) {
        count++;
    });

    ASSERT_EQ(count, 7);
}

TEST(COMPONENT, InitializerList) {
    Container root {
        Container {
            Component {},
            Component {},
        },
        Component {},
        Component {},
        Component {},
    };

    root.child<Container>(0)
        .result([](const Container& first) {
            first.child<Component>(0)
                .result([](const Component& second) {
                    /* do nothing */
                })
                .error([](auto& err) {
                    std::cerr << err << "\n";
                    GTEST_FAIL();
                });
        })
        .error([](auto& err) {
            std::cerr << err << "\n";
            GTEST_FAIL();
        });
}
#endif