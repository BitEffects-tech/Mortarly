#include "GUI.h"
#include "TreeNode.h"
#include "GUIContext.h"
#include "../core/testing.h"

using namespace gui;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::string;

GUIContext context;

shared_ptr<ControlState> gui::begin(LayoutDirection direction, float width, float height) {
    return context.begin(direction, width, height);
}

void gui::end() {
    context.end();
}

void gui::style(Style key, const string &value) {
    context.current->styles[key] = value;
}

void gui::style(Style key, const float value) {
    context.current->styles[key] = value;
}

void gui::style(Style key, const bool value) {
    context.current->styles[key] = value;
}

void gui::text(const string &text) {

}

bool gui::onClick() {
    return false;
}

bool gui::onMouseDown() {
    return false;
}

bool gui::onHover() {
    return false;
}

#if USING_GTEST

test::GraphicalTestResults guiTestResults("GUI");

class TestGUIContext : public GUIContext {
public:
    shared_ptr<TreeNode> savedRoot;
    TestGUIContext() {
        this->savedRoot = make_shared<TreeNode>();
    }

    void render() override {
        this->savedRoot->children = vector<shared_ptr<TreeNode>>(this->root->children);
        GUIContext::render();
    }
};

void assertRectEq(const Rectangle &rect, float x, float y, float w, float h) {
    ASSERT_FLOAT_EQ(rect.x, x);
    ASSERT_FLOAT_EQ(rect.y, y);
    ASSERT_FLOAT_EQ(rect.w, w);
    ASSERT_FLOAT_EQ(rect.h, h);
}

TEST(GUI, HorizontalLayoutProportions) {
    auto graphics = test::GraphicalTest();

    context = TestGUIContext();

    gui::begin(gui::LayoutDirection::Horizontal, 100, 100);
        gui::begin(gui::LayoutDirection::Vertical, 1, 10); gui::end();
        gui::begin(gui::LayoutDirection::Vertical, 1, 10); gui::end();
        gui::begin(gui::LayoutDirection::Vertical, 1, 0);
            gui::style(Style::Padding, 5.0f);
        gui::end();
        gui::begin(gui::LayoutDirection::Vertical, 1, 0);
            gui::style(Style::Padding, 5.0f);
        gui::end();

        ASSERT_NE(context.root, nullptr);
        ASSERT_EQ(context.root->children.size(), 1);
        const auto &rootNode = *context.root->children[0];

        ASSERT_EQ(rootNode.children.size(), 4);
        assertRectEq(rootNode.state->rect, 0, 0, 100, 100);

    ASSERT_NE(rootNode.children[0]->state, nullptr);
    gui::end();

    assertRectEq(rootNode.children[0]->state->rect, 00, 0, 25, 10);
    assertRectEq(rootNode.children[1]->state->rect, 25, 0, 25, 10);
    assertRectEq(rootNode.children[2]->state->rect, 50, 0, 25, 10);
    assertRectEq(rootNode.children[3]->state->rect, 75, 0, 25, 10);

    graphics.assertScreenshot("HorizontalLayoutProportions");
}

TEST(GUI, VerticalLayoutProportions) {
    context = TestGUIContext();

    gui::begin(gui::LayoutDirection::Vertical, 100, 100);
        gui::begin(gui::LayoutDirection::Horizontal, 10, 1); gui::end();
        gui::begin(gui::LayoutDirection::Horizontal, 10, 1); gui::end();
        gui::begin(gui::LayoutDirection::Horizontal, 0, 1);
            gui::style(Style::Padding, 5.0f);
        gui::end();
        gui::begin(gui::LayoutDirection::Horizontal, 0, 1);
            gui::style(Style::Padding, 5.0f);
        gui::end();

        ASSERT_NE(context.root, nullptr);
        ASSERT_EQ(context.root->children.size(), 1);
        const auto &rootNode = *context.root->children[0];

        ASSERT_EQ(rootNode.children.size(), 4);
        assertRectEq(rootNode.state->rect, 0, 0, 100, 100);

    ASSERT_NE(rootNode.children[0]->state, nullptr);
    gui::end();


    assertRectEq(rootNode.children[0]->state->rect, 0, 00, 10, 25);
    assertRectEq(rootNode.children[1]->state->rect, 0, 25, 10, 25);
    assertRectEq(rootNode.children[2]->state->rect, 0, 50, 10, 25);
    assertRectEq(rootNode.children[3]->state->rect, 0, 75, 10, 25);
}

TEST(GUI, NestedLayoutProportions) {
    context = TestGUIContext();

    gui::begin(gui::LayoutDirection::Vertical);
        gui::begin(gui::LayoutDirection::Vertical);
        gui::end();

        gui::begin(gui::LayoutDirection::Vertical);
        gui::end();
    gui::end();
}

TEST(GUI, TreeStructureMatches) {
    context = TestGUIContext();

    gui::begin(gui::LayoutDirection::Vertical);
        ASSERT_NE(context.root, nullptr);
        EXPECT_EQ(context.root->children.size(), 1);

        gui::begin(gui::LayoutDirection::Vertical);
        gui::end();

        gui::begin(gui::LayoutDirection::Vertical);
            EXPECT_EQ(1, context.root->children.size());
            EXPECT_EQ(2, context.root->children[0]->children.size());
        gui::end();

    gui::end();

    EXPECT_EQ(0, context.root->children.size());
}

#endif