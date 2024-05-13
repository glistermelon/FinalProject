#include "Quadtree.h"
#include <algorithm>

double QuadtreeNode::surface_area(QuadtreeNode * q1, QuadtreeNode * q2) {
    return (std::max(q1->topright.x, q2->topright.x) - std::min(q1->botleft.x, q2->botleft.x)) 
        * (std::max(q1->topright.y, q2->topright.y) - std::min(q1->botleft.y, q2->botleft.y));
}

void QuadtreeNode::insert_block(QuadtreeNode * root, Block* block) {
    
}

int QuadtreeNode::HEIGHT(QuadtreeNode * tree) {
    if (tree == nullptr) return 0;
    return tree->height;
}

QuadtreeNode * QuadtreeNode::left_rotate(QuadtreeNode* tree) {
    QuadtreeNode * right = tree->right;
    QuadtreeNode * left = right->left;
    right->left = tree;
    tree->right = left;
    tree->height = std::max(HEIGHT(tree->left), HEIGHT(tree->right)) + 1;  
    right->height = std::max(HEIGHT(right->left), HEIGHT(right->right)) + 1;  
    //return new tree  
    return right;  

}