#include "sphere.hpp"

#include <cmath>

p3d::sphere::sphere(math::vec3 center, float radius, material mat) : scene_obj(mat) {
	_center = center;
	_radius = radius;
}

p3d::sphere::~sphere() {
	//TODO
}

math::vec3 p3d::sphere::center() {
	return _center;
}

float p3d::sphere::radius() {
	return _radius;
}

p3d::hit p3d::sphere::calculate_intersection(const p3d::ray &ray) const {
	math::vec3 oc = _center - ray.o();
	float td_min = math::dot(oc, ray.d());
	if (td_min < 0.0f) return hit();
	else {
		float a = math::dot(ray.d(), ray.d());
		float b = math::dot(-oc,ray.d()) * 2.0f;
		float c = math::dot(-oc, -oc) - _radius * _radius;

		float disc = b * b - 4.0f * a * c;
		if (disc < 0.0f) return hit();

		float e = std::sqrt(disc);

		float t = (-b - e) / (2.0f * a);  // smaller root

		if (t > KEPSILON) {
			return hit(ray.o() + ray.d() * t, (-oc + ray.d() * t) / _radius, _mat, t, true);
		}

		t = (-b + e) / (2.0f * a);  // larger root

		if (t > KEPSILON) {
			return hit(ray.o() + ray.d() * t, (-oc + ray.d() * t) / _radius, _mat, t, true);
		}

		return hit();
	}
}