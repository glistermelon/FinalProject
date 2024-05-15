#include "Quadtree.h"
#include <algorithm>
#include <stack>

QuadtreeNode::QuadtreeNode() {
    is_leaf = false;
    block = nullptr;
    parent = left = right = nullptr;
}

QuadtreeNode::QuadtreeNode(Block * b) {
    is_leaf = true;
    block = b;
    Rect bBox = b->bounding_box();
    botleft.x = bBox.bot_left.x;
    botleft.y = bBox.bot_left.y;
    topright.x = bBox.bot_left.x + bBox.width;
    topright.y = bBox.bot_left.y + bBox.height;
    parent = left = right = nullptr;
}

double QuadtreeNode::surface_area(QuadtreeNode * q1, QuadtreeNode * q2) {
    return (std::max(q1->topright.x, q2->topright.x) - std::min(q1->botleft.x, q2->botleft.x)) 
        * (std::max(q1->topright.y, q2->topright.y) - std::min(q1->botleft.y, q2->botleft.y));
}

double QuadtreeNode::surface_area() {
    return (topright.x - botleft.x) * (topright.y - botleft.y);
}

void QuadtreeNode::swap(QuadtreeNode * q1, QuadtreeNode * q2) {
    QuadtreeNode * p1 = q1->parent;
    QuadtreeNode * p2 = q2->parent;
    if (p2->left == q2) {
        p2->left = q1;
        q1->parent = p2;
    } else {
        p2->right = q1;
        q1->parent = p2;
    }
    if (p1->left == q1) {
        p1->left = q2;
        q2->parent = p1;
    } else {
        p1->right = q2;
        q2->parent = p1;
    }
    p1->botleft.x = std::min(p1->left->botleft.x, p1->right->botleft.x);
    p1->botleft.y = std::min(p1->left->botleft.y, p1->right->botleft.y);
    p1->topright.x = std::max(p1->left->topright.x, p1->right->topright.x);
    p1->topright.y = std::max(p1->left->topright.y, p1->right->topright.y);
    p2->botleft.x = std::min(p2->left->botleft.x, p2->right->botleft.x);
    p2->botleft.y = std::min(p2->left->botleft.y, p2->right->botleft.y);
    p2->topright.x = std::max(p2->left->topright.x, p2->right->topright.x);
    p2->topright.y = std::max(p2->left->topright.y, p2->right->topright.y);
}

void QuadtreeNode::insert_block(QuadtreeNode * tree, Block* block) {
    if (tree->block == nullptr && tree->is_leaf) {
        tree->block = block;
        tree->is_leaf = true;
        Rect bBox = block->bounding_box();
        tree->botleft.x = bBox.bot_left.x;
        tree->botleft.y = bBox.bot_left.y;
        tree->topright.x = bBox.bot_left.x + bBox.width;
        tree->topright.y = bBox.bot_left.y + bBox.height;
        return;
    }
    QuadtreeNode * qBlock = new QuadtreeNode(block);

    // Find the best sibling
    double bCost = std::numeric_limits<double>::max();
    QuadtreeNode * bestSibling = find_bestSibling(tree, qBlock, bCost, 0);

    // Create a new parent
    QuadtreeNode * p = new QuadtreeNode();
    p->left = bestSibling;
    p->right = qBlock;
    p->botleft.x = std::min(bestSibling->botleft.x, qBlock->botleft.x);
    p->botleft.y = std::min(bestSibling->botleft.y, qBlock->botleft.y);
    p->topright.x = std::max(bestSibling->topright.x, qBlock->topright.x);
    p->topright.y = std::max(bestSibling->topright.y, qBlock->topright.y);
    p->parent = bestSibling->parent;
    bestSibling->parent = p;
    qBlock->parent = p;
    if (p->parent->left == bestSibling) p->parent->left = p;
    else p->parent->right = p;

    // Refit the AABBs
    QuadtreeNode * ptr = p;
    while (ptr->left != nullptr) {
        double minSA;
        if (!ptr->left->is_leaf) {
            minSA = ptr->left->surface_area();
            if (surface_area(ptr->left->right, ptr->right) < minSA) {
                swap(ptr->left->left, ptr->right);
                minSA = ptr->left->surface_area();
            }
            if (surface_area(ptr->left->left, ptr->right) < minSA) {
                swap(ptr->left->right, ptr->right);
                minSA = ptr->left->surface_area();
            }
        }
        if (!ptr->right->is_leaf) {
            minSA = ptr->right->surface_area();
            if (surface_area(ptr->left, ptr->right->left) < minSA) {
                swap(ptr->left, ptr->right->right);
                minSA = ptr->right->surface_area();
            }
            if (surface_area(ptr->left, ptr->right->right) < minSA) {
                swap(ptr->left, ptr->right->left);
                minSA = ptr->right->surface_area();
            }
        }

        ptr->botleft.x = std::min(ptr->left->botleft.x, ptr->right->botleft.x);
        ptr->botleft.y = std::min(ptr->left->botleft.y, ptr->right->botleft.y);
        ptr->topright.x = std::max(ptr->left->topright.x, ptr->right->topright.x);
        ptr->topright.y = std::max(ptr->left->topright.y, ptr->right->topright.y);
        ptr = ptr->parent;
    }
}

QuadtreeNode* QuadtreeNode::remove_block() {
    if (!is_leaf || parent->left == nullptr) {
        return nullptr;
    }
    if (parent->left == this) {
        if (parent->parent->right == parent) {
            parent->parent->right = parent->right;
            parent->right->parent = parent->parent;
        } else {
            parent->parent->left = parent->right;
            parent->right->parent = parent->parent;
        } 
    } else {
        if (parent->parent->right == parent) {
            parent->parent->right = parent->left;
            parent->left->parent = parent->parent;
        } else {
            parent->parent->left = parent->left;
            parent->left->parent = parent->parent;
        }
    }
    parent = nullptr;
    return this;
}

QuadtreeNode * QuadtreeNode::find_bestSibling(QuadtreeNode* tree, QuadtreeNode* b, double& bCost, double iCost) {
    if (tree == nullptr) return nullptr;
    QuadtreeNode * ret = nullptr;
    double unionArea = surface_area(tree, b);
    double cost = unionArea + iCost;
    if (cost < bCost) {
        bCost = cost;
        ret = tree;
    }
    cost = cost + b->surface_area() - tree->surface_area();
    if (cost < bCost) {
        QuadtreeNode * temp = find_bestSibling(tree->left, b, bCost, cost - b->surface_area());
        if (temp != nullptr) ret = temp;
        temp = find_bestSibling(tree->right, b, bCost, cost - b->surface_area());
        if (temp != nullptr) ret = temp;
    }
    return ret;
}

bool QuadtreeNode::is_intersecting(QuadtreeNode* a, QuadtreeNode* b) {
    if (a == nullptr || b == nullptr) return false;
    return !(a->botleft.x > b->topright.x || a->botleft.y > b->topright.y || a->topright.x < b->botleft.x || a->topright.y < b->botleft.y);
}

void QuadtreeNode::search_tree(QuadtreeNode* n, std::stack<QuadtreeNode*> * intersects) {
    bool is_int = is_intersecting(n, this);
    if (is_leaf && is_int) {intersects->push(this); return;}
    if (is_leaf || !is_int) return;
    if (n->left != nullptr) search_tree(n->left, intersects);
    if (n->right != nullptr) search_tree(n->right, intersects);
}

bool QuadtreeNode::fully_contained(Block* b) {
    Rect bBox = b->bounding_box();
    return !(bBox.bot_left.x < botleft.x || 
        bBox.bot_left.y < botleft.y ||
        bBox.bot_left.x + bBox.width > topright.x ||
        bBox.bot_left.y + bBox.height > topright.y);
}

void QuadtreeNode::update_tree(std::stack<Block*> * blocks) {
    if (is_leaf) {
        if (!parent->fully_contained(block)) {
            blocks->push(block);
            remove_block();
        }
        return;
    }
    left->update_tree(blocks);
    right->update_tree(blocks);
    botleft.x = std::min(left->botleft.x, right->botleft.x);
    botleft.y = std::min(left->botleft.y, right->botleft.y);
    topright.x = std::max(left->topright.x, right->topright.x);
    topright.y = std::max(left->topright.y, right->topright.y);
}

std::vector<CollisionGroup*> QuadtreeNode::find_collisions(QuadtreeNode* tree) {
    if (tree == nullptr || tree->is_leaf) return std::vector<CollisionGroup*>();
    std::vector<CollisionGroup*> ret;
    std::stack <QuadtreeNode*> st;
    st.push(tree);
    QuadtreeNode * temp;
    while (!st.empty()) {
        temp = st.top(); st.pop();
        if (temp->is_leaf) {
            int i = ret.size(); // Check i's current size 
            if (temp == temp->parent->left) {
                std::stack<QuadtreeNode*> intersects;
                temp->parent->right->search_tree(temp, &intersects);
                while (!intersects.empty()) {
                    ret.push_back(new CollisionGroup(temp->block, intersects.top()->block));
                    intersects.pop();
                }
            } else {
                std::stack<QuadtreeNode*> intersects;
                temp->parent->left->search_tree(temp, &intersects);
                while (!intersects.empty()) {
                    ret.push_back(new CollisionGroup(temp->block, intersects.top()->block));
                    intersects.pop();
                }
            }
            // Find duplicates in ret
            for (; i < ret.size(); i++) {
                for (int j = i + 1; j < ret.size(); j++) {
                    if (ret[i]->is_same(ret[j])) {ret.erase(ret.begin() + i); i--; break;}
                }
            }
        } else {
            if (temp->left != nullptr) st.push(temp->left);
            if (temp->right != nullptr) st.push(temp->right);
        }
    }
    return ret;
}