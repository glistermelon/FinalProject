#pragma once

#include <vector>
#include <Point.h>
#include "Vect2.h"
#include "Rect.h"
#include "Block.h"

// #include "Quadtree.h"

// Handles updating blocks on screen, collisions
class Screen {
    Vect2 gravity_accel;

    public:
        Screen();
        std::vector<Block*> blocks;
        // QuadtreeNode qtree;
        void update();
};