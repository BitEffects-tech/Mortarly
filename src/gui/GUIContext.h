#pragma once

#include "GUI.h"
#include "TreeNode.h"

namespace gui {
    class GUIContext {
    public:
        std::shared_ptr<TreeNode> root;
        std::shared_ptr<TreeNode> current;

        GUIContext();
        const std::shared_ptr<ControlState> begin(gui::LayoutDirection direction, float width = 0, float height = 0);
        void end();

        virtual void render();

    protected:
        void calculateLayout(std::shared_ptr<TreeNode> node);
        void renderRecursive(const std::shared_ptr<TreeNode>& node);
    };
}
