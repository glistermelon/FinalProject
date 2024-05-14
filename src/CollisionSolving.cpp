#include "../include/CollisionSolving.h"

Collision::Collision(Block* b1, Block* b2, Point p1, Point p2, Vect2 normal)
        : block1(b1), block2(b2), point1(p1), point2(p2), normal(normal) {}

double Collision::calc_impulse(Point p) {
    Block* block = p == point1 ? block1 : block2;
    auto n = normal;
    if (block == block2) n.flip();
    Vect2 r = block->displacement_rel(p);
    auto z = r.magnitude() * sin(r.direction() - n.direction());
    auto v = block1->velocity - block2->velocity;
    auto w = block1->angular_velocity - block2->angular_velocity;
    return pow(
            (pow(n.x, 2) + pow(n.y, 2))
            /
            block->mass + pow(z, 2) / block->moment_of_inertia(),
            -1
    ) * (n.x * v.x + n.y * v.y + z * w);
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

        auto n = normal; // normal should always point away from p1 surface of block2
        if (block == block2) n.flip();

        block->apply_accel(
                Vect2(normal.x / block->mass * delta_impulse, n.y / block->mass * delta_impulse)
        );

        Vect2 r = block->displacement_rel(point);
        block->apply_angular_accel(
                r.magnitude() * sin(
                        r.direction() - n.direction()
                ) / block->moment_of_inertia() * delta_impulse
        );

    }
}

CollisionGroup::CollisionGroup(Block *b1, Block *b2) : block1(b1), block2(b2) {}

Vect2 CollisionGroup::calc_normal(bool& optimal) {
    auto critical_points = block1->find_critical_vertices(*block2);
    auto prev_loc = block1->get_shadow();
    std::vector<Segment> cast_rays;
    cast_rays.reserve(critical_points.size());
    for (const auto& p : critical_points)
        cast_rays.emplace_back(p, prev_loc.to_world_coords(block1->to_relative_coords(p)));
    Segment s;
    bool segment_found = false;
    for (const auto& ray : cast_rays) {
        for (auto edge : block2->get_edges()) {
            if (ray.intersects(edge)) {
                if (s == edge) continue;
                if (!segment_found) {
                    s = edge;
                    segment_found = true;
                }
                else {
                    optimal = false;
                    return block2->displacement_rel(
                            block2->to_relative_coords(block1->to_world_coords(block1->center_of_mass()))
                    );
                }
            }
        }
    }
    optimal = true;
    return block2->get_normal(s);

}

void CollisionGroup::solve() {

    bool optimal_normal;
    normal = calc_normal(optimal_normal);
    if (!optimal_normal) {
        std::swap(block1, block2);
        Vect2 n = calc_normal(optimal_normal);
        if (optimal_normal) normal = n;
        else std::swap(block1, block2);
    }

    // TODO: replace {} with collision point in block 2
    for (auto p : block1->find_critical_vertices(*block2))
        collisions.emplace_back(Collision(block1, block2, p, {}, normal));

    for (unsigned int i = 0; i < num_iterations; ++i)
        for (auto& c : collisions) c.solve();

}