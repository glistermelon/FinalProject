#pragma once

#include <Point.h>
#include <Color.h>
#include <shaders.h>

#include <GL/glew.h>
#include <stdexcept>
#include <list>

class Block {

    Point position;

    std::list<Point> vertices; // block is a polygon, with no holes

    Color color = Color::WHITE;

};