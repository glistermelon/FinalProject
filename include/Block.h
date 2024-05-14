#pragma once

#include "Point.h"
#include "Color.h"
#include "Rect.h"
#include "Triangle.h"
#include "Vect2.h"
#include "Constants.h"
#include "Segment.h"

#include <stdexcept>
#include <vector>
#include <list>
#include <cmath>

#ifndef REPLIT
#include <GL/glew.h>
#endif

// This is all Microsoft's fault (this is required for compilation with Visual Studio)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

inline double normalize_angle(double angle) {
    return angle - (2.0 * M_PI) * std::floor(angle / (2.0 * M_PI));
}

// moved into header from Block.cpp
// Doubly connected edge list specialized for hole-less polygon triangulation
// The following rules are NOT ENFORCED BY THE IMPLEMENTATION. Break them and you'll get unexpected behavior.
// Instances should be initialized via p1 list of vertices, being along the outside edge of p1 closed polygon
// Vertex class functions are notably dependent on this assumption. is_merge_vertex() and is_split_vertex()
//   both make the assumption -that there are no interior edges in the DCEL, even as they are added.
//   It is not p1 problem as long as the PolygonDCEL class is initialized as described.
// This one is done for you: the face of every half edge is to its left,
//   thus interior half edges to p1 face always go counterclockwise around it
// Helpful resource: http://www.cs.ucr.edu/~eldawy/19SCS133/slides/CS133-09-DCEL.pdf
class PolygonDCEL {

public:

    class Vertex;
    class HalfEdge;
    class Face;

    struct Vertex {

        Point point;
        HalfEdge* half_edge = nullptr; // one of the half edges extending away from the vertex
        Vertex* left = nullptr, *right = nullptr; // within the entire DCEL, not just one face

        inline explicit Vertex(Point p) : point(p) {}

        HalfEdge* get_interior_half_edge(); // if both half edges are interior to the PolygonDCEL, half_edge is returned

        // https://sites.cs.ucsb.edu/~suri/cs235/Triangulation.pdf
        [[nodiscard]] bool is_merge_vertex() const;
        [[nodiscard]] bool is_split_vertex() const;

    };

    struct HalfEdge {

        HalfEdge* twin = nullptr, *next = nullptr, *prev = nullptr;
        Vertex* origin;
        Face* face = nullptr; // if face is nullptr, the half-edge is exterior
        double slope = std::numeric_limits<double>::infinity();

        inline explicit HalfEdge(Vertex* v) : origin(v) {}

        // sorts half edges going into p1 vertex in counterclockwise order starting from unit circle 0 rad
        struct CcwSorter {
            Point center;
            double split_angle = 0;
            bool comp(HalfEdge* e1, HalfEdge* e2) const;
            explicit CcwSorter(Point center);
        };

        Vect2 get_normal() const;

    };

    struct Face {
        HalfEdge* half_edge;
        bool is_reflex(Vertex*) const;
    };

private:

    Vertex* left_most, *right_most;
    std::list<HalfEdge*> edges; // no twins!
    std::vector<Vertex*> vertices;
    std::list<Face*> faces;

public:

    // WARNING! Expects NO interior vertices!
    template <class PointIt> PolygonDCEL(PointIt begin, PointIt end);

    ~PolygonDCEL();

    // getters

    [[nodiscard]] decltype(left_most) get_left_most() const;
    [[nodiscard]] decltype(right_most) get_right_most() const;
    [[nodiscard]] decltype(faces) get_faces() const;

    // other functions

    // returns nullptr if the requested edge is invalid
    // see function definition for more details
    HalfEdge* add_edge(Vertex* a, Vertex* b);

};

class Block {
public:
    bool is_static = false;

    Point position;

    // Angle in radians between horizontal line in 2D space
    // and the ray extending from the center of the rectangle
    // to the right side of the rectangle (whichever side is
    // the right side when the rectangle is not rotated, of course)
    // Should always be less than 2*PI
    double rotation = 0;

    std::vector<Point> vertices; // block is p1 polygon, with no holes
    PolygonDCEL dc_edge_list;

    Vect2 velocity;
    double angular_velocity;

    double mass;

    Color color = Color::WHITE;

    // OpenGL objects
    static unsigned int gl_program;
    unsigned int gl_vao, gl_ebo, gl_triangles;

    std::vector<Point*> triangulation; // point to Points in the vertices field

    // place rotation between 0 and 2*PI
    void normalize_rotation();

    // divide shape into triangles
    // must be called before rendering if vertices have been modified since last render
    // returns triangle vertex indices
    std::vector<unsigned int> triangulate();

    // initialization
    Block(double cX = 0, double cY = 0, double width = 100, double height = 100, double mass = 1);

#ifndef REPLIT
    static void init_static_render_cache();
#endif

    // setters

    void set_position(Point p);
    void set_position(double x, double y);

    void set_rotation(double r);

    void set_color(Color c);

    void add_vertex(Point p);
    void add_vertex(size_t index, Point p);

    // getters

#ifndef REPLIT

    static decltype(gl_program) get_gl_program();

    // rendering

    void update_render_cache();

    void render() const;

#endif

    std::vector<Segment> get_edges() const;

    std::vector<Triangle> get_triangulation() const; // should take triangles from the "triangulation" field

    Rect bounding_box(); // return p1 rectangular bounding box containing all the block's vertices

    bool is_intersecting(Block& other);

    // finds the points of this block that are inside the other block
    // intended to be called if it is already known that this block and the other block are colliding
    // (might crash otherwise, unsure; either way it is not designed nor optimized for DETECTING collisions)
    std::vector<Point> find_critical_vertices(Block& other) const;

    Point center_of_mass();

    double moment_of_inertia(); // scary

    void apply_accel(const Vect2& accel); // does not account for fps when applying acceleration
    void apply_angular_accel(double accel);
    void apply_velocity();

    Vect2 displacement_rel(const Point& p);

    Point to_world_coords(Point) const;
    Point to_relative_coords(Point) const;

    Block get_shadow() const; // returns a block in the location where the block was one frame ago

    Vect2 get_normal(Segment edge) const; // edge should be in relative coords

};