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
    // for (Block* block : blocks) {
    //     for (Block* other : blocks) {
    //         Rect bBox = block->bounding_box();
    //         Rect oBox = other->bounding_box();
    //         if (block == other) continue;
    //         if (block->is_static) {
    //             // std::cout << "Other box bottom: " << oBox.top_left.y - oBox.height << std::endl;
    //             // std::cout << "Other box center: " << other->vertices[0].y << std::endl;
    //             // std::cout << "Velocity.y: " << other->velocity.y << std::endl;
    //             // std::cout << "Ground top: " << bBox.top_left.y << std::endl;
    //             // std::cout << "Ground bottom: " << bBox.top_left.y - bBox.height << std::endl;
    //         }
    //         if (block->is_static &&
    //             oBox.top_left.y - oBox.height < bBox.top_left.y &&
    //             oBox.top_left.y - oBox.height > bBox.top_left.y - bBox.height &&
    //             other->velocity.y < 0) {
    //             std::cout<< "Collision" << std::endl;
    //             block->velocity.y = -block->velocity.y;
    //         }
    //     }
    // }

    // Add gravity
    for (Block* block : blocks) {
        block->apply_accel(gravity_accel);
    }

    // Update render cache
    for (Block* block : blocks) {
        block->update_render_cache();
    }
}