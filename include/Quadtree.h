#pragma once

#include <list>

#include "Block.h"
#include "Vect2.h"

// https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf
class QuadtreeNode {
public:
    bool is_leaf = true;
    int height = 0;
    Block * block = nullptr;
    QuadtreeNode * left, * right;
    Vect2 botleft, topright; 

    double surface_area(QuadtreeNode *, QuadtreeNode *);
    void insert_block(QuadtreeNode *, Block*);
    void remove_block(QuadtreeNode*, Block*);
    void find_collisions(QuadtreeNode*);
    void handle_collision(Block*, Block*);
    QuadtreeNode* left_rotate(QuadtreeNode*);
    QuadtreeNode* right_rotate(QuadtreeNode*);
    int HEIGHT(QuadtreeNode*);
};