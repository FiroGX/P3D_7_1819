#include "sphere.hpp"

#include <cmath>

p3d::sphere::sphere(math::vec3 center, float radius, material mat) : scene_obj(mat) {
	_center = center;
	_radius = radius;
}

p3d::sphere::~sphere() {}

math::vec3 p3d::sphere::center() {
	return _center;
}

float p3d::sphere::radius() {
	return _radius;
}

p3d::hit p3d::sphere::calculate_intersection(const p3d::ray &ray) const {
	math::vec3 oc = _center - ray.o();
	float b = math::dot(oc, ray.d());
	float c = math::dot(oc, oc) - _radius * _radius;
	if (c > 0.0f && b < 0.0f) return hit();
	else {
		float root = b * b - c;
		if (root < 0.0f) return hit();

		float e = std::sqrt(root);

		float t;
		if (c > 0)
			t = b - std::sqrt(root);
		else
			t = b + std::sqrt(root);

		if (t > KEPSILON)
			return hit(ray.o() + ray.d()*t, (-oc + ray.d() * t) / _radius, _mat, t, true);

		return hit();
	}
}