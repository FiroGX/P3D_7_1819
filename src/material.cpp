#include "material.hpp"

p3d::material::material(math::vec3 color, float kd, float ks, float shine, float t, float i) {
	_color = color; _kd = kd; _ks = ks; _shine = shine; _t = t; _ref_index = i;
}

p3d::material::material() {}

p3d::material::~material() {}

math::vec3 p3d::material::color() const {
	return _color;
}

float p3d::material::kd() const {
	return _kd;
}

float p3d::material::ks() const {
	return _ks;
}

float p3d::material::shine() const {
	return _shine;
}

float p3d::material::t() const {
	return _t;
}

float p3d::material::ref_index() const {
	return _ref_index;
}
