#pragma once

#include <vector>
#include <Point.h>
#include "Block.h"
#include "Rect.h"

#include "Quadtree.h"

class Screen {
    public:
        int fps = 30;
        std::vector<Block*> blocks;
        QuadtreeNode qtree;
        void update();
};