#include "hit.hpp"

p3d::hit::hit(math::vec3 point, math::vec3 normal, material mat, bool collided) {
	_point = point; _normal = normal; _mat = mat; _collided = collided;
}

p3d::hit::hit() {
	_collided = false;
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
