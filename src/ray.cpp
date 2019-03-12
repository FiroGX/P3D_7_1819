#include "ray.hpp"

p3d::ray::ray() {}

p3d::ray::ray(math::vec3 o, math::vec3 d) {
	_o = o;
	_d = d;
}

p3d::ray::~ray() {}

math::vec3 p3d::ray::o() const {
	return _o;
}

math::vec3 p3d::ray::d() const {
	return _d;
}

void p3d::ray::o(const math::vec3 o) {
	_o = o;
}

void p3d::ray::d(const math::vec3 d) {
	_d = d;
}
