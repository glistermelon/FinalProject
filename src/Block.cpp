#include "../include/Block.h"

#include <list>
#include <stack>
#include <vector>
#include <algorithm>
#include <limits>
#include <stdexcept>

#ifndef REPLIT
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaders.h"
#include "cmake_gen_shaders.hpp"
#endif

// Doubly connected edge list specialized for hole-less polygon triangulation
// The following rules are NOT ENFORCED BY THE IMPLEMENTATION. Break them and you'll get unexpected behavior.
// Instances should be initialized via a list of vertices, being along the outside edge of a closed polygon
// Vertex class functions are notably dependent on this assumption. is_merge_vertex() and is_split_vertex()
//   both make the assumption -that there are no interior edges in the DCEL, even as they are added.
//   It is not a problem as long as the PolygonDCEL class is initialized as described.
// This one is done for you: the face of every half edge is to its left,
//   thus interior half edges to a face always go counterclockwise around it
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

        // sorts half edges going into a vertex in counterclockwise order starting from unit circle 0 rad
        struct CcwSorter {
            Point center;
            double split_angle = 0;
            bool comp(HalfEdge* e1, HalfEdge* e2) const;
            explicit CcwSorter(Point center);
        };

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

bool PolygonDCEL::Face::is_reflex(PolygonDCEL::Vertex* v) const {

    HalfEdge* e = half_edge;
    while (e->origin != v) e = e->next;

    double angle_in, angle_out, angle;
    const Point& origin = e->origin->point, &p_in = e->prev->origin->point, &p_out = e->next->origin->point;

    if (abs(p_in.x - origin.x) < 0.0001) angle_in = (p_in.y > origin.y ? 1 : 3) * M_PI / 2;
    else {
        angle_in = atan((p_in.y - origin.y) / (p_in.x - origin.x));
        if (p_in.x < origin.x) angle_in += M_PI;
        angle_in = normalize_angle(angle_in);
    }
    if (abs(p_out.x - origin.x) < 0.0001) angle_out = (p_out.y > origin.y ? 1 : 3) * M_PI / 2;
    else {
        angle_out = atan((p_out.y - origin.y) / (p_out.x - origin.x));
        if (p_out.x < origin.x) angle_out += M_PI;
        angle_out = normalize_angle(angle_out);
    }

    angle = angle_in - angle_out;
    if (angle_in < angle_out) angle += 2 * M_PI;

    return angle > M_PI;

}

bool PolygonDCEL::HalfEdge::CcwSorter::comp(PolygonDCEL::HalfEdge* e1, PolygonDCEL::HalfEdge* e2) const {
    const Point& p1 = e1->origin->point, &p2 = e2->origin->point;
    double angle1 = atan((p1.y - center.y) / (p1.x - center.x));
    double angle2 = atan((p2.y - center.y) / (p2.x - center.x));
    if (p1.x < center.x) angle1 += M_PI;
    if (p2.x < center.x) angle2 += M_PI;
    return normalize_angle(angle1 - split_angle) < normalize_angle(angle2 - split_angle);
}

PolygonDCEL::HalfEdge::CcwSorter::CcwSorter(Point center) : center(center) {}

PolygonDCEL::HalfEdge* PolygonDCEL::Vertex::get_interior_half_edge() {
    return half_edge->face ? half_edge : half_edge->twin;
}

bool PolygonDCEL::Vertex::is_merge_vertex() const {
    std::list<HalfEdge*> inward_edges;
    HalfEdge* e = half_edge->twin;
    do {
        inward_edges.push_back(e);
        if (e->origin->point.x > point.x) return false;
        e = e->next->twin;
    } while (e != half_edge->twin);
    HalfEdge::CcwSorter sorter(point);
    inward_edges.sort([&](const auto& a, const auto& b) {
        return sorter.comp(a, b);
    });
    return inward_edges.front()->face;
}

bool PolygonDCEL::Vertex::is_split_vertex() const {
    std::list<HalfEdge*> inward_edges;
    HalfEdge* e = half_edge->twin;
    do {
        inward_edges.push_back(e);
        if (e->origin->point.x < point.x) return false;
        e = e->next->twin;
    } while (e != half_edge->twin);
    HalfEdge::CcwSorter sorter(point);
    sorter.split_angle = M_PI;
    inward_edges.sort([&](const auto& a, const auto& b) {
        return sorter.comp(a, b);
    });
    return inward_edges.front()->face;
}

decltype(PolygonDCEL::left_most) PolygonDCEL::get_left_most() const {
    return left_most;
}

decltype(PolygonDCEL::right_most) PolygonDCEL::get_right_most() const{
    return right_most;
}

PolygonDCEL::HalfEdge* PolygonDCEL::add_edge(PolygonDCEL::Vertex *a, PolygonDCEL::Vertex *b) {

    // Edges are not allowed to intersect
    // Edges are not allowed to extend beyond the existing boundary of the PolygonDCEL
    // Edges can only be added between existing vertices
    // Assuming no improper input is provided, adding any edge will cause one face to split into two

    // check for collisions (intersections)
    if (a == b) return nullptr;
    double slope = (a->point.y - b->point.y) / (a->point.x - b->point.x);
    for (const auto& edge : edges) {
        if ((edge->origin == a && edge->twin->origin == b) || (edge->origin == b && edge->twin->origin == a))
            return nullptr;
        else if ((edge->origin == a) != (edge->twin->origin == b) || (edge->origin == b) != (edge->twin->origin == a))
            continue;
        Point& p1 = edge->origin->point, p2 = edge->twin->origin->point, p3 = a->point, p4 = b->point;
        // find where the segments, should they be lines, would intersect
        double x = (p3.y - p1.y + edge->slope * p1.x - slope * p3.x) / (edge->slope - slope);
        double left_x = p3.x, right_x = p4.x;
        double left_x2 = p1.x, right_x2 = p2.x;
        if (left_x > right_x) std::swap(left_x, right_x);
        if (left_x2 > right_x2) std::swap(left_x2, right_x2);
        if (x > left_x && x < right_x && x > left_x2 && x < right_x2) return nullptr;
    }

    auto half_edge = new HalfEdge(a);
    edges.push_back(half_edge);
    half_edge->twin = new HalfEdge(b);
    half_edge->twin->twin = half_edge;
    {
        const Point& p_a = a->point, &p_b = b->point;
        half_edge->slope = (p_a.y - p_b.y) / (p_a.x - p_b.x);
        half_edge->twin->slope = half_edge->slope;
    }

    std::vector<HalfEdge*> edges_ccw;
    for (HalfEdge* e = a->half_edge->twin; e != a->half_edge->prev; e = e->next->twin)
        edges_ccw.push_back(e);
    edges_ccw.push_back(a->half_edge->prev);
    edges_ccw.push_back(half_edge->twin);
    HalfEdge::CcwSorter sorter(a->point);
    std::sort(edges_ccw.begin(), edges_ccw.end(),
              [&](const auto& a, const auto& b) {
                        return sorter.comp(a, b);
    });

    int64_t index = 1 + std::find(edges_ccw.begin(), edges_ccw.end(), half_edge->twin) - edges_ccw.begin();
    if (index == edges_ccw.size()) index = 0;
    auto i_edge = edges_ccw[index];
    half_edge->prev = i_edge;
    half_edge->face = i_edge->face;
    half_edge->face->half_edge = half_edge; // just to make sure

    // if it works I suppose...
    half_edge->next = i_edge;
    while (half_edge->next->origin != b) half_edge->next = half_edge->next->prev;

    // again, and split the face this time

    auto new_face = new Face;
    faces.push_back(new_face);
    new_face->half_edge = half_edge->twin;
    half_edge->twin->face = new_face;

    index -= 2;
    if (index < 0) index += (decltype(index))edges_ccw.size();
    i_edge = edges_ccw[index];
    half_edge->twin->next = i_edge->twin;

    half_edge->twin->prev = i_edge->twin;
    while (half_edge->twin->prev->next->origin != b) {
        half_edge->twin->prev->face = new_face;
        half_edge->twin->prev = half_edge->twin->prev->next;
    }
    half_edge->twin->prev->face = new_face;

    // update the surrounding half edges
    half_edge->prev->next = half_edge;
    half_edge->next->prev = half_edge;
    half_edge->twin->prev->next = half_edge->twin;
    half_edge->twin->next->prev = half_edge->twin;

    return half_edge;

}

decltype(PolygonDCEL::faces) PolygonDCEL::get_faces() const {
    return faces;
}

PolygonDCEL::~PolygonDCEL() {
    for (const auto& edge : edges) {
        delete edge->twin;
        delete edge;
    }
    for (const auto& vertex : vertices) delete vertex;
    for (const auto& face : faces) delete face;
}

template<class PointIt>
PolygonDCEL::PolygonDCEL(PointIt begin, PointIt end) {

    for (PointIt it = begin; it != end; ++it)
        vertices.push_back(new Vertex(*it));

    const size_t num_vertices = vertices.size();
    vertices.push_back(vertices.front()); // optimizes connecting the first and last vertex to complete loop

    // Create half edges and their twins
    for (size_t i = 0; i < num_vertices; ++i) {

        auto e = new HalfEdge(vertices[i]);
        vertices[i]->half_edge = e;
        edges.push_back(e); // no twins!

        auto e_twin = new HalfEdge(vertices[i + 1]);
        e->twin = e_twin;
        e_twin->twin = e;

    }

    // Configure next and prev of half edges
    for (size_t i = 0; i < num_vertices; ++i) {
        vertices[i]->half_edge->next = vertices[i + 1]->half_edge;
        vertices[i]->half_edge->twin->prev = vertices[i + 1]->half_edge->twin;
    }
    for (size_t i = 1; i < num_vertices + 1; ++i) {
        vertices[i]->half_edge->prev = vertices[i - 1]->half_edge;
        vertices[i]->half_edge->twin->next = vertices[i - 1]->half_edge->twin;
    }

    // Calculate half edge slopes
    for (size_t i = 0; i < num_vertices; ++i) {
        vertices[i]->half_edge->slope =
                (vertices[i + 1]->point.y - vertices[i]->point.y) /
                (vertices[i + 1]->point.x - vertices[i]->point.x);
    }

    // Find left-most and right-most vertices
    vertices.pop_back();
    std::stable_sort(
            vertices.begin(), vertices.end(),
            [](const auto& a, const auto& b) -> bool {
                return a->point.y < b->point.y;
            }
    );
    std::stable_sort(
            vertices.begin(), vertices.end(),
            [](const auto& a, const auto& b) -> bool {
                return a->point.x < b->point.x;
            }
    );

    left_most = vertices.front();
    right_most = vertices.back();

    // Configure vertices' left and right properties
    vertices.push_back(vertices.front());
    for (size_t i = 0; i < num_vertices; ++i)
        vertices[i]->right = vertices[i + 1];
    for (size_t i = 1; i < num_vertices + 1; ++i)
        vertices[i]->left = vertices[i - 1];
    vertices.pop_back();

    // Configure the edges' interior face
    auto face = new Face;
    faces.push_back(face);
    if (left_most->half_edge->next->origin->point.y < left_most->half_edge->prev->origin->point.y) {
        for (const auto &v: vertices) v->half_edge->face = face;
        face->half_edge = vertices.front()->half_edge;
    }
    else {
        for (const auto& v : vertices) v->half_edge->twin->face = face;
        face->half_edge = vertices.front()->half_edge->twin;
    }

    // Phew!

}

std::vector<unsigned int> Block::triangulate() {

    PolygonDCEL dcel(vertices.begin(), vertices.end());

    // divide PolygonDCEL into monotone faces

    PolygonDCEL::Vertex* central_vertex = dcel.get_left_most();

    while (central_vertex != dcel.get_right_most()) {

        // convenience
        double x[3];
        x[0] = central_vertex->get_interior_half_edge()->prev->origin->point.x;
        x[1] = central_vertex->point.x;
        x[2] = central_vertex->get_interior_half_edge()->next->origin->point.x;

        if (central_vertex->is_merge_vertex()) {
            for (PolygonDCEL::Vertex* right = central_vertex->right; right; right = right->right)
                if(dcel.add_edge(central_vertex, right)) break;
        }
        else if (central_vertex->is_split_vertex()) {
            for (PolygonDCEL::Vertex* left = central_vertex->left; left; left = left->left)
                if (dcel.add_edge(central_vertex, left)) break;
        }

        central_vertex = central_vertex->right;

    }

    // triangulate monotone faces

    for (PolygonDCEL::Face* const& face : dcel.get_faces()) {

        std::vector<PolygonDCEL::Vertex*> sorted_vertices;
        for (auto e = face->half_edge->next; e != face->half_edge; e = e->next)
            sorted_vertices.push_back(e->origin);
        sorted_vertices.push_back(face->half_edge->origin);
        std::sort(sorted_vertices.begin(), sorted_vertices.end(),
                  [](const auto& a, const auto& b) {
            return a->point.y < b->point.y;
        });
        std::stable_sort(sorted_vertices.begin(), sorted_vertices.end(),
                  [](const auto& a, const auto& b) {
                      return a->point.x < b->point.x;
        });

        std::vector<PolygonDCEL::Vertex*> bottom_chain;
        {
            PolygonDCEL::HalfEdge* e = face->half_edge;
            while (e->origin != sorted_vertices.front()) e = e->next;
            while (e->origin != sorted_vertices.back()) {
                bottom_chain.push_back(e->origin);
                e = e->next;
            }
        }

        std::vector<PolygonDCEL::Vertex*> reflex;
        for (const auto& v : sorted_vertices)
            if (face->is_reflex(v)) reflex.push_back(v);

        std::stack<PolygonDCEL::Vertex*> reflex_chain;
        reflex_chain.push(sorted_vertices[0]);
        reflex_chain.push(sorted_vertices[1]);

        for (size_t i = 2; i < sorted_vertices.size() - 1; ++i) {

            auto vertex = sorted_vertices[i];
            auto left = sorted_vertices[i - 1];

            if (
                    (std::find(bottom_chain.begin(), bottom_chain.end(), vertex) != bottom_chain.end())
                    !=
                    (std::find(bottom_chain.begin(), bottom_chain.end(), reflex_chain.top()) != bottom_chain.end())
            ) { // vertices are on different chain
                while (reflex_chain.size() != 1) {
                    dcel.add_edge(vertex, reflex_chain.top());
                    reflex_chain.pop();
                }
                reflex_chain.pop();
                reflex_chain.push(left);
                reflex_chain.push(vertex);
            } // vertices are on same chains
            else if (std::find(reflex.begin(), reflex.end(), left) != reflex.end()) {
                reflex_chain.push(vertex);
            }
            else {
                if (reflex_chain.top() == left) reflex_chain.pop(); // these vertices are already connected!
                PolygonDCEL::Vertex* last;
                do {
                    last = reflex_chain.top();
                    reflex_chain.pop();
                } while (dcel.add_edge(vertex, last) && !reflex_chain.empty());
                reflex_chain.push(last);
                reflex_chain.push(vertex);
            }

        }

        reflex_chain.pop();
        while (reflex_chain.size() > 1) {
            dcel.add_edge(sorted_vertices.back(), reflex_chain.top());
            reflex_chain.pop();
        }

    }

    std::vector<unsigned int> indices;
    for (const auto& face : dcel.get_faces()) {
        // all faces should be triangular
        auto* half_edge = face->half_edge;
        for (unsigned char i = 0; i < 3; ++i, half_edge = half_edge->next) {
            auto it = std::find(vertices.begin(), vertices.end(), half_edge->origin->point);
            indices.push_back(it - vertices.begin());
            triangulation.push_back(&*it);
        }

    }
    return indices;

}

#ifndef REPLIT
void Block::init_static_render_cache() {

    // Set up shader program
    unsigned int vertex_shader = compile_shader(GL_VERTEX_SHADER, SHADERS_VERTEX);
    unsigned int frag_shader = compile_shader(GL_FRAGMENT_SHADER, SHADERS_FRAGMENT);
    gl_program = glCreateProgram();
    glAttachShader(gl_program, vertex_shader);
    glAttachShader(gl_program, frag_shader);
    glLinkProgram(gl_program);
    int program_link_success;
    glGetProgramiv(gl_program, GL_LINK_STATUS, &program_link_success);
    if (!program_link_success) {
        char err[512];
        glGetProgramInfoLog(gl_program, 512, nullptr, err);
        throw std::runtime_error("shader linking failed: " + std::string(err));
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

}
#endif

Block::Block(double cX, double cY, double width, double height, double mass) {
    width /= 2; height /= 2;
    set_position(cX, cY);
    add_vertex(Point(cX - width, cY - height));
    add_vertex(Point(cX + width, cY - height));
    add_vertex(Point(cX + width, cY + height));
    add_vertex(Point(cX - width, cY + height));
    this->mass = mass;
}

void Block::normalize_rotation() {
    rotation = normalize_angle(rotation);
}

void Block::set_position(Point p) {
    position = p;
}

void Block::set_position(double x, double y) {
    position = Point(x, y);
}

void Block::set_rotation(double r) {
    rotation = r;
    normalize_rotation();
}

void Block::set_color(Color c) {
    color = c;
}

void Block::add_vertex(Point p) {
    vertices.push_back(p);
}

void Block::add_vertex(size_t index, Point p) {
    vertices.insert(vertices.begin() + (decltype(vertices)::difference_type)index, p);
}

#ifndef REPLIT

decltype(Block::gl_program) Block::get_gl_program() {
    return gl_program;
}

void Block::update_render_cache() {

    glGenVertexArrays(1, &gl_vao);
    glBindVertexArray(gl_vao);

    // Give OpenGL the vertices
    const auto num_vertices = vertices.size();
    auto vertex_buffer = new float[2 * num_vertices];
    for (size_t v_i = 0, b_i = 0; v_i < num_vertices; ++v_i, b_i += 2) {
        vertex_buffer[b_i] = (float)vertices[v_i].x;
        vertex_buffer[b_i + 1] = (float)vertices[v_i].y;
    }

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(float) * 2 * num_vertices), vertex_buffer, GL_STATIC_DRAW);
    delete[] vertex_buffer;
    glVertexAttribPointer(
            0, 2, GL_FLOAT, GL_FALSE,
            2 * sizeof(float), (void*)nullptr
    );
    glEnableVertexAttribArray(0);

    // Split block into triangles and give them to OpenGL
    glGenBuffers(1, &gl_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_ebo);
    std::vector<unsigned int> indices = triangulate();
    gl_triangles = indices.size() / 3;

    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            (GLsizeiptr)(sizeof(unsigned int) * indices.size()),
            indices.data(),
            GL_STATIC_DRAW
    );

}

void Block::render() const {

    glm::mat4 transform(1.0f); // transformation matrix

    GLint viewport[4]; // origin x, origin y, width, height
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Scale down to fit into the viewport
    transform = glm::scale(
            transform, glm::vec3(2.0 / viewport[2], 2.0 / viewport[3], 1.0)
    );

    // Rotate the block
    transform = glm::rotate(
            transform, (float)rotation,
            glm::vec3(0.0f, 0.0f, 1.0f) // about the center
    );

    // Move the block to proper position
    transform = glm::translate(transform, glm::vec3(
            position.x - (float)viewport[2] / 2 + viewport[0],
            position.y - (float)viewport[3] / 2 + viewport[1],
            0.0f
    ));

    // Give transformation matrix to vertex shader
    glUniformMatrix4fv(
            glGetUniformLocation(gl_program, "transform"),
            1, GL_FALSE, glm::value_ptr(transform)
    );

    // Give color to vertex shader
    glProgramUniform4f(
            gl_program, glGetUniformLocation(gl_program, "color"),
            (float)Block::color.r / 255,
            (float)Block::color.g / 255,
            (float)Block::color.b / 255,
            1.0f
    );

    glBindVertexArray(gl_vao);
    glDrawElements(GL_TRIANGLES, (GLsizei)(3 * gl_triangles), GL_UNSIGNED_INT, nullptr);

}

std::vector<Triangle> Block::get_triangulation() {
    size_t n = triangulation.size();
    std::vector<Triangle> triangles;
    for (size_t i = 0; i < n; i += 3) {
        triangles.push_back(Triangle(
                *triangulation[i],
                *triangulation[i + 1],
                *triangulation[i + 2]
        ));
    }
    return triangles;
}

// Gets the bounding box of the block, Top left point and width and height
Rect Block::bounding_box() { // TODO
    Point * tLeft = &vertices[0];
    double width = 0, height = 0;
    for (size_t i = 1; i < vertices.size(); i++) {
        if (width == 0 && tLeft->y == vertices[i].y) 
            width = std::abs(tLeft->x - vertices[i].x);
        if (height == 0 && tLeft->x == vertices[i].x)
            height = std::abs(tLeft->y - vertices[i].y);
        if (tLeft->x >= vertices[i].x && tLeft->y <= vertices[i].y) {
            tLeft = &vertices[i];
        }
    }
    return Rect{*tLeft, width, height};
}

Point Block::center_of_mass() { 
    double centerx = 0;
    double centery = 0;
    double area = 0;
    if (vertices.size() < 3){
        return Point(); // May want to add a signal if the polygon doesn't have enough vertices
    }

    for(size_t i = 0; i < vertices.size(); i++){
        const Point& current = vertices[i];
        const Point& next = vertices[(i+1) % vertices.size()];

        double a = current.x * next.y - next.x * current.y;
        centerx += (current.x + next.x) * a;
        centery += (current.y + next.y) * a;
        area += a;
    }
    
    area = area/2;
    if(area != 0){
        centerx /= (6*area);
        centery /= (6*area);
    }

    return Point(centerx, centery);
}

double Block::moment_of_inertia() { // TODO
    return 0;
}

unsigned int Block::gl_program;

#endif