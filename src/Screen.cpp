#include "../include/Screen.h"

#include <iostream>
#include <stack>
#include <vector>

Screen::Screen() {
    gravity_accel.x = 0;
    gravity_accel.y = -9.81;

    head = QuadtreeNode();
    head.right = new QuadtreeNode();
    head.left = nullptr;
    // Add ground

}

void Screen::update() {
    for (Block* block : blocks) block->render();

    // Debug Statement
    // std::cout << blocks[0]->position.x << ", " << blocks[0]->position.y << std::endl;

    // Move blocks based on velocity
    for (Block* block : blocks) {
        block->apply_velocity();
    }

    // Update tree based on new displacements
    std::stack<Block*> reAdd;
    head.right->update_tree(&reAdd);
    while (!reAdd.empty()) {
        head.right->insert_block(head.right, reAdd.top());
        reAdd.pop();
    }

    // Handle Collisions
    std::vector<CollisionGroup*> collisions = head.right->find_collisions(head.right);

    // Add gravity
    for (Block* block : blocks) {
        block->apply_accel(gravity_accel);
    }

    // Update render cache
    for (Block* block : blocks) {
        block->update_render_cache();
    }
}