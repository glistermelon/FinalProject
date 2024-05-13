#include "Quadtree.h"

void QuadtreeNode::handle_collisions(
        std::vector<Block*> higher_ups,
        std::vector<std::pair<Block*, Block*>> already_found
) {
    higher_ups.insert(higher_ups.end(), blocks.begin(), blocks.end());
    if (top_left) top_left->handle_collisions(higher_ups, already_found);
    if (top_right) top_right->handle_collisions(higher_ups, already_found);
    if (bottom_left) bottom_left->handle_collisions(higher_ups, already_found);
    if (bottom_right) bottom_right->handle_collisions(higher_ups, already_found);
    std::vector<CollisionGroup> collisions;
    for (Block* b1 : blocks) {
        for (const std::vector<Block*>& other_blocks : { blocks, higher_ups }) {
            for (Block* b2 : other_blocks) {
                if (b1 == b2) continue;
                std::pair<Block*, Block*> pair(b1, b2);
                if (std::find(already_found.begin(), already_found.end(), pair) == already_found.end()
                    && are_colliding(b1, b2)) {
                    collisions.push_back(CollisionGroup(b1, b2));
                    already_found.push_back(pair);
                }
            }
        }
    }
    for (auto& collision_group : collisions)
        collision_group.solve();
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

double Collision::calc_impulse(Point) { // TODO
    return 0;
}

void Collision::solve() {
    for (const auto& point : { point1, point2 }) {

        Block* block = point == point1 ? block1 : block2;
        auto& impulse = point == point1 ? p1_total_impulse : p2_total_impulse;
        auto prev_impulse = impulse;
        auto delta_impulse = calc_impulse(point);
        impulse += delta_impulse;
        if (impulse < 0) impulse = 0;
        delta_impulse = impulse - prev_impulse;
        
        block->apply_accel(
                Vect2(normal.x / block->mass * delta_impulse, normal.y / block->mass * delta_impulse)
        );

        Vect2 r = block->distance(point);
        block->apply_angular_accel(
                r.magnitude() * sin(
                        r.direction() - normal.direction()
                ) / block->moment_of_inertia() * delta_impulse
        );

    }
}

CollisionGroup::CollisionGroup(Block *b1, Block *b2) : block1(b1), block2(b2) {}

std::vector<Collision> CollisionGroup::find_collisions() { // TODO
    return {};
}

Vect2 CollisionGroup::calc_normal() { // TODO
    return {};
}

void CollisionGroup::solve() { // TODO

}


