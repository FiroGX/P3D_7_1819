#include "light.hpp"

p3d::light::light(float x, float y , float z, float r, float g, float b) {
	_pos = new math::vec3(x, y, z);
	_color = new math::vec3(r, g, b);
}

p3d::light::~light() {}

math::vec3 *p3d::light::pos() const {
	return _pos;
}

math::vec3 *p3d::light::color() const {
	return _color;
}
