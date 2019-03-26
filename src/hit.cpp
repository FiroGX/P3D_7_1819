#include "hit.hpp"

#include <limits>

p3d::hit::hit(math::vec3 point, math::vec3 normal, material mat, float distance, bool collided) {
	_point = point; _normal = math::normalize(normal); _mat = mat; _distance = distance; _collided = collided;
}

p3d::hit::hit() {
	_collided = false;
	_distance = std::numeric_limits<float>::max();
}

p3d::hit::~hit() {}

math::vec3 p3d::hit::point() const {
	return _point;
}

math::vec3 p3d::hit::normal() const {
	return _normal;
}

p3d::material p3d::hit::mat() const {
	return _mat;
}

bool p3d::hit::collided() const {
	return _collided;
}

float p3d::hit::distance() const {
	return _distance;
}

void p3d::hit::distance(float distance) {
	_distance = distance;
}
