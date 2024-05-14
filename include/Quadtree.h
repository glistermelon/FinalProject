#pragma once

#include <list>
#include <vector>

#include "Block.h"
#include "Vect2.h"
#include "CollisionSolving.h"

// https://box2d.org/files/ErinCatto_DynamicBVH_Full.pdf
class QuadtreeNode {
public:
    bool is_leaf = true;
    Block * block = nullptr;
    QuadtreeNode * left, * right, * parent;
    Vect2 botleft, topright; 

    // Basically an AVL tree to insert the nodes, balance the tree
    QuadtreeNode();
    QuadtreeNode(Block*);
    double surface_area(QuadtreeNode *, QuadtreeNode *);
    double surface_area();
    int get_balance(QuadtreeNode *); 
    void insert_block(QuadtreeNode *, Block*);
    QuadtreeNode* find_bestSibling(QuadtreeNode*, QuadtreeNode*, double&, double); 
    QuadtreeNode* remove_block();
    std::vector<CollisionGroup*> find_collisions(QuadtreeNode*); //
    void swap(QuadtreeNode*, QuadtreeNode*);
};