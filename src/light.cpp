#include "light.hpp"

p3d::light::light(math::vec3 position, math::vec3 color) : _pos(position), _color(color) {};

p3d::light::~light() {}

math::vec3 p3d::light::pos() const {
	return _pos;
}

math::vec3 p3d::light::color() const {
	return _color;
}

void p3d::light::pos(math::vec3 pos) {
	_pos = pos;
}

void p3d::light::color(math::vec3 color) {
	_color = color;
}
