#include "sphere.hpp"

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

p3d::hit p3d::sphere::calculate_intersection(p3d::ray &ray) {
	return hit();
}