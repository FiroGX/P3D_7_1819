#include "sphere.hpp"

#include <cmath>

p3d::sphere::sphere(math::vec3 center, float radius, material mat) : scene_obj(mat) {
	_center = center;
	_radius = radius;
}

p3d::sphere::~sphere() {}

const math::vec3 &p3d::sphere::center() {
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

		if (t > math::KEPSILON)
			return hit(ray.o() + ray.d() * t, math::normalize((ray.o() + ray.d() * t) - _center), _mat, t, true);

		return hit();
	}
}

p3d::b_box p3d::sphere::box() const {
    return b_box(_center.x() - _radius, _center.y() - _radius, _center.z() - _radius,
        _center.x() + _radius, _center.y() + _radius, _center.z() + _radius);
}
