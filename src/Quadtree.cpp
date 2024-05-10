#include "Quadtree.h"

bool QuadtreeNode::are_colliding(Block *b1, Block *b2) { // TODO
    return false;
}

void QuadtreeNode::handle_collisions() {
    if (top_left) top_left->handle_collisions();
    if (top_right) top_right->handle_collisions();
    if (bottom_left) bottom_left->handle_collisions();
    if (bottom_right) bottom_right->handle_collisions();
    std::vector<Collision> collisions;
    for (Block* b1 : blocks) {
        for (Block* b2 : blocks) {
            if (b1 == b2) continue;
            if (are_colliding(b1, b2))
                collisions.push_back(Collision(b1, b2));
        }
    }
}

std::pair<Point, Point> Collision::calc_points() { // TODO
    return {};
}

Vect2 Collision::calc_normal() { // TODO
    return {};
}

double Collision::calc_impulse(Point p, Block *b) { // TODO
    return 0;
}

Collision::Collision(Block *b1, Block *b2) : block1(b1), block2(b2) {}

void Collision::solve() { // TODO

}

void Collision::apply() { // TODO

}
