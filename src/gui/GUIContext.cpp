#include "GUIContext.h"

using namespace gui;
using std::make_shared;
using std::shared_ptr;

GUIContext::GUIContext() {
    root = make_shared<TreeNode>();
    current = root;
}

const shared_ptr<ControlState> GUIContext::begin(gui::LayoutDirection direction, float width, float height) {
    auto state = make_shared<ControlState>();
    state->direction = direction;
    state->pWidth = width;
    state->pHeight = height;

    if (width > 1) {
        state->rect.w = width;
    }
    if (height > 1) {
        state->rect.h = height;
    }

    auto node = make_shared<TreeNode>(current, state);

    current->children.push_back(node);
    current = node;

    return state;
}

void GUIContext::end() {
    if (current != root) {
        current = current->parent;
    }

    if (current == root) {
        assert(root != nullptr);
        assert(root->children.size() == 1);
        assert(root->children[0]->state != nullptr);

        const auto rootNode = root->children[0];
        calculateLayout(rootNode);
        render();
        
        root->styles.clear();
        root->children.clear();
    }
}

void GUIContext::render() {
    // Traverse the tree and render each node
    renderRecursive(root);
}

void GUIContext::calculateLayout(shared_ptr<TreeNode> node) {
    assert(node != nullptr);
    assert(node->state != nullptr);

    node->state->styles = node->buildStyles();

    ControlState& state = *node->state;
    Rectangle& rect = state.rect;
    StyleMap& styles = state.styles;

    float leftPos = rect.x;
    float topPos = rect.y;
    float availableWidth = rect.w;
    float availableHeight = rect.h;

    assert(styles.find(Style::Padding) != styles.end());

    rect.w = std::max(rect.w, node->minWidth());
    rect.h = std::max(rect.h, node->minHeight());

    if (state.direction == LayoutDirection::Horizontal) {
        float totalPWidth = 0;

        // recompute the proportional sizes to be a percentage of the parent size
        for (const auto& child : node->children) {
            if (child->state->pWidth <= 1) {
                totalPWidth += child->state->pWidth;
            }
            else {
                availableWidth -= child->minWidth();
            }
        }

        for (const auto& child : node->children) {
            auto& cRect = child->state->rect;

            cRect.x = leftPos;
            if (child->state->pWidth <= 1 && totalPWidth > 0 && availableWidth > 0) {
                cRect.w = std::max(
                    child->minWidth(),
                    (child->state->pWidth / totalPWidth) * availableWidth
                );
            }
            else {
                cRect.w = child->minWidth();
            }
            
            leftPos += cRect.w;
        }
    }
    else {
        float totalPHeight = 0;

        // recompute the proportional sizes to be a percentage of the parent size
        for (const auto& child : node->children) {
            if (child->state->pHeight <= 1) {
                totalPHeight += child->state->pHeight;
            }
            else {
                availableHeight -= child->minHeight();
            }
        }

        for (const auto& child : node->children) {
            auto& cRect = child->state->rect;

            cRect.y = topPos;
            if (child->state->pHeight <= 1 && totalPHeight > 0 && availableHeight > 0) {
                cRect.h = std::max(
                    child->minHeight(),
                    (child->state->pHeight / totalPHeight) * availableHeight
                );
            }
            else {
                cRect.h = child->minHeight();
            }
            
            //printf("Rect: %.2f %.2f %.2f %.2f\n", cRect.x, cRect.y, cRect.w, cRect.h);

            topPos += cRect.h;
        }
    }

    for (const auto& child : node->children) {
        calculateLayout(child);
    }
}

void GUIContext::renderRecursive(const shared_ptr<TreeNode>& node) {
    if (node->onRender) {
        node->onRender();
    }

    for (const auto& child : node->children) {
        renderRecursive(child);
    }
}
