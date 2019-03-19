#include "triangle.hpp"

p3d::triangle::triangle(math::vec3 a, math::vec3 b, math::vec3 c, p3d::material mat) :
    scene_obj(mat), _a(a), _b(b), _c(c) {
}

p3d::triangle::~triangle() {}

const math::vec3 &p3d::triangle::a() { return _a; }

const math::vec3 &p3d::triangle::b() { return _b; }

const math::vec3 &p3d::triangle::c() { return _c; }

p3d::hit p3d::triangle::calculate_intersection(const p3d::ray &ray) const {
    math::vec3 normal = math::normalize(math::cross(_b - _a, _c - _a));
    math::vec3 v1 = _b - _a;
    math::vec3 v2 = _c - _a;
    math::vec3 v3 = -ray.d();
    math::vec3 vs = ray.o() - _a;

    float denominator = v1.x() * (v2.y()*v3.z() - v3.y()*v2.z()) +
        v2.x() * (v3.y()*v1.z() - v1.y()*v3.z()) +
        v3.x() * (v1.y()*v2.z() - v2.y()*v1.z());

    float beta = (vs.x() * (v2.y()*v3.z() - v3.y()*v2.z()) +
        v2.x() * (v3.y()*vs.z() - vs.y()*v3.z()) +
        v3.x() * (vs.y()*v2.z() - v2.y()*vs.z())) /
        denominator;

    float gamma = (v1.x() * (vs.y()*v3.z() - v3.y()*vs.z()) +
        vs.x() * (v3.y()*v1.z() - v1.y()*v3.z()) +
        v3.x() * (v1.y()*vs.z() - vs.y()*v1.z())) /
        denominator;

    float t = (v1.x() * (v2.y()*vs.z() - vs.y()*v2.z()) +
        v2.x() * (vs.y()*v1.z() - v1.y()*vs.z()) +
        vs.x() * (v1.y()*v2.z() - v2.y()*v1.z())) /
        denominator;

    if (beta < 0 || beta > 1 || gamma < 0 || gamma > 1 ||
        beta + gamma < 0 || beta + gamma > 1 || t < math::KEPSILON)
        return hit();

    return hit(ray.o() + ray.d() * t, normal, _mat, t, true);
}
