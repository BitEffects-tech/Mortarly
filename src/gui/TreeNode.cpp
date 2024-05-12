#include "GUI.h"
#include "TreeNode.h"

using namespace gui;
using std::shared_ptr;
using std::make_shared;
using std::string;
using std::get;

StyleMap DEFAULT_STYLES = {
    { Style::Color, "black" },
    { Style::BorderColor, "black" },
    { Style::Padding, 0.0f },
};

void inheritStyles(const StyleMap &parentStyles, StyleMap &destStyles) {
    for (auto kvp : parentStyles) {
        if (destStyles.find(kvp.first) == destStyles.end()){
            destStyles[kvp.first] = kvp.second;
        }
    }
}

TreeNode::TreeNode() {}
TreeNode::TreeNode(const shared_ptr<TreeNode> parent, const shared_ptr<ControlState> &state)
    : parent(parent), state(state) {
    
}

StyleMap TreeNode::buildStyles() const {
    StyleMap res(this->styles);

    shared_ptr<TreeNode> parent = this->parent;
    while (parent != nullptr) {
        inheritStyles(parent->styles, res);
        parent = parent->parent;
    }
    inheritStyles(DEFAULT_STYLES, res);

    return res;
}

float TreeNode::minWidth() const {
    float res = 0;

    for (const auto& child : children) {
        res = std::max(res, child->minWidth());
    }

    auto paddingValue = styles.find(Style::Padding);
    if (paddingValue != styles.end()) {
        res += get<float>(paddingValue->second) * 2;
    }

    return res;
}

float TreeNode::minHeight() const {
    float res = 0;

    for (const auto& child : children) {
        res = std::max(res, child->minHeight());
    }

    auto paddingValue = styles.find(Style::Padding);
    if (paddingValue != styles.end()) {
        res += get<float>(paddingValue->second) * 2;
    }

    return res;
}

#if USING_GTEST

TEST(GUI, StylesInheritAsExpected) {
    auto state = make_shared<ControlState>();    
    auto node1 = make_shared<TreeNode>();
    auto node2 = make_shared<TreeNode>(node1, state);

    node1->styles[Style::Color] = "red";
    node1->styles[Style::BorderColor] = "gray";
    node2->styles[Style::Color] = "blue";
    
    auto styles = node2->buildStyles();

    EXPECT_EQ(DEFAULT_STYLES.size(), styles.size());
    EXPECT_EQ("gray", get<string>(styles[Style::BorderColor]));
    EXPECT_EQ("blue", get<string>(styles[Style::Color]));
}

#endif