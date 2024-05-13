#pragma once

#include <vector>

#include "Point.h"
#include "Vect2.h"
#include "Rect.h"
#include "Block.h"
// #include "Quadtree.h"

// Handles updating blocks on screen, collisions
class Screen {
    Vect2 gravity_accel;
    unsigned int fps;

    public:
        int ind = 0;
        Screen(unsigned int fps);
        std::vector<Block*> blocks;
        void update();
};