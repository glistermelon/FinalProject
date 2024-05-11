#include "Quadtree.h"

void QuadtreeNode::handle_collisions() {
    if (top_left) top_left->handle_collisions();
    if (top_right) top_right->handle_collisions();
    if (bottom_left) bottom_left->handle_collisions();
    if (bottom_right) bottom_right->handle_collisions();
    std::vector<Collision> collisions;
    for (Block* b1 : blocks) {
        for (Block* b2 : blocks) {
            if (b1 == b2) continue;
            if (are_colliding(b1, b2)); // TODO
        }
    }
}

bool QuadtreeNode::are_colliding(Block* b1, Block* b2) { // TODO
    return false;
}

void QuadtreeNode::insert_block(Block*) { // TODO

}

void QuadtreeNode::remove_block(Block*) { // TODO

}

Collision::Collision(Block* b1, Block* b2, Point p1, Point p2, Vect2 normal)
    : block1(b1), block2(b2), point1(p1), point2(p2), normal(normal) {}

double Collision::calc_impulse(Point*) { // TODO
    return 0;
}

void Collision::solve() { // TODO

}

std::vector<Collision> CollisionGroup::find_collisions() { // TODO
    return {};
}

Vect2 CollisionGroup::calc_normal() { // TODO
    return {};
}