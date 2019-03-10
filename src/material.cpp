#include "material.hpp"

p3d::material::material(float r, float g, float b, float kd, float ks, float shine, float t, float i) {
	_color = math::vec3(r, g, b); _kd = kd; _ks = ks; _shine = shine; _t = t; _ref_index = i;
}

p3d::material::~material() {}

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
