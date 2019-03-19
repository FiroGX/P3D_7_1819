#include "plane.hpp"

p3d::plane::plane(math::vec3 a, math::vec3 b, math::vec3 c, material mat) : scene_obj(mat) {
	_point = a;
	_normal = math::normalize(math::cross(b - a, c - a));
}

p3d::plane::~plane() {}

math::vec3 p3d::plane::point() {
	return _point;
}

math::vec3 p3d::plane::normal() {
	return _normal;
}

p3d::hit p3d::plane::calculate_intersection(const ray &ray) const {
	if (math::dot(_normal, ray.d()) == 0.0f) return hit();

	float t = math::dot(_point - ray.o(), _normal) / math::dot(ray.d(), _normal);
	if (t > math::KEPSILON) return hit((ray.o() + ray.d() * t), _normal, _mat, t, true);
	return hit();
}
