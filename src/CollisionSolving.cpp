#include "../include/CollisionSolving.h"

// https://box2d.org/files/ErinCatto_SequentialImpulses_GDC2006.pdf
// http://www.richardtonge.com/presentations/Tonge-2012-GDC-solvingRigidBodyContacts.pdf

Collision::Collision(Block* b1, Block* b2, Point p, Vect2 normal)
        : block1(b1), block2(b2), point(p), normal(normal) {
    r1 = block1->displacement_rel(block1->to_relative_coords(p));
    r2 = block2->displacement_rel(block1->to_relative_coords(p));
}

Vect2 Collision::calc_rel_velocity() {
    auto a = (block2->velocity - block1->velocity);
    auto b = Vect3::cross_product(block2->angular_velocity_vector(), r2.to_3d());
    auto c = Vect3::cross_product(block1->angular_velocity_vector(), r1.to_3d());
    return a
           + b.to_2d()
           - c.to_2d();
}

Vect2 Collision::calc_impulse() {
    auto i1 = block1->moment_of_inertia();
    auto i2 = block2->moment_of_inertia();
    Vect3 v = Vect3::cross_product(1/i1 * Vect3::cross_product(r1.to_3d(), normal.to_3d()), r1.to_3d())
              + Vect3::cross_product(1/i2 * Vect3::cross_product(r2.to_3d(), normal.to_3d()), r2.to_3d());
    double k = 1/block1->mass + 1/block2->mass + Vect2::dot_product(v.to_2d(), normal);
    auto vr = calc_rel_velocity();
    return normal * (Vect2::dot_product(-vr, normal) / k);
}

void Collision::solve() {

    double impulse = total_impulse;
    double prev_impulse = impulse;
    auto delta_impulse = calc_impulse();
    double delta_magnitude = delta_impulse.magnitude();
    if ((normal.x < 0 && delta_impulse.x > 0) || (normal.x > 0 && delta_impulse.x < 0))
        delta_magnitude = -delta_magnitude;
    impulse += delta_magnitude;
    if (impulse < 0) impulse = 0;
    delta_impulse.set_magnitude(impulse - prev_impulse);

    block1->apply_accel(-delta_impulse / block1->mass * 2);
    block2->apply_accel(delta_impulse / block2->mass * 2);
    block1->apply_angular_accel(
            (-1/block1->moment_of_inertia() * Vect3::cross_product(r1.to_3d(), delta_impulse.to_3d())).magnitude()
    );
    block2->apply_angular_accel(
            (1/block2->moment_of_inertia() * Vect3::cross_product(r2.to_3d(), delta_impulse.to_3d())).magnitude()
    );

}

CollisionGroup::CollisionGroup(Block *b1, Block *b2) : block1(b1), block2(b2) {}

Vect2 CollisionGroup::calc_normal(bool& optimal) {
    auto critical_points = block1->find_critical_vertices(*block2);
    auto prev_loc = block1->get_shadow();
    std::vector<Segment> cast_rays;
    cast_rays.reserve(critical_points.size());
    for (const auto& p : critical_points)
        cast_rays.emplace_back(block1->to_world_coords(p), prev_loc.to_world_coords(p));
    Segment s;
    bool segment_found = false;
    for (const auto& ray : cast_rays) {
        for (auto edge : block2->get_edges()) {
            edge = Segment(block2->to_world_coords(edge.p1), block2->to_world_coords(edge.p2));
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
    return block2->get_normal(Segment(block2->to_relative_coords(s.p1), block2->to_relative_coords(s.p2)));

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

    for (auto p : block1->find_critical_vertices(*block2))
        collisions.emplace_back(block1, block2, p, normal);

    for (unsigned int i = 0; i < 1; ++i)
        for (auto& c : collisions) c.solve();

}