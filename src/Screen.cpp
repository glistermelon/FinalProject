#include "../include/Screen.h"

#include <iostream>

Screen::Screen() {
    gravity_accel.x = 0;
    gravity_accel.y = -9.81;

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

    // Handle Collisions
    

    // Add gravity
    for (Block* block : blocks) {
        block->apply_accel(gravity_accel);
    }

    // Update render cache
    for (Block* block : blocks) {
        block->update_render_cache();
    }
}