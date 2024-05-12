#pragma once

#include "GUI.h"

namespace gui {
    class TreeNode {
    public:
        TreeNode();
        TreeNode(const std::shared_ptr<TreeNode> parent, const std::shared_ptr<gui::ControlState> &state);
        
        std::shared_ptr<TreeNode> parent;
        std::shared_ptr<gui::ControlState> state;
        std::vector<std::shared_ptr<TreeNode>> children;
        gui::StyleMap styles;

        std::function<void()> onRender;

        gui::StyleMap buildStyles() const;
        float minWidth() const;
        float minHeight() const;
    };
}