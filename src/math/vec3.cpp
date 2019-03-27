#include "vec3.hpp"
#include <cmath>

math::vec3::vec3() : _x(0.0f), _y(0.0f), _z(0.0f) {
}

math::vec3::vec3(float x, float y, float z) {
	_x = x;
	_y = y;
	_z = z;
}

float math::vec3::x() const {
	return _x;
}

float math::vec3::y() const {
	return _y;
}

float math::vec3::z() const {
	return _z;
}

math::vec3 math::vec3::operator-() const {
	return vec3(-_x, -_y, -_z);
}

math::vec3 math::vec3::operator-(const vec3 &v) const {
	return vec3(_x - v.x(), _y - v.y(), _z - v.z());
}

math::vec3 &math::vec3::operator-=(const vec3 &v) {
	_x -= v.x();
	_y -= v.y();
	_z -= v.z();
	return *this;
}

math::vec3 math::vec3::operator+(const vec3 &v) const {
	return vec3(_x + v.x(), _y + v.y(), _z + v.z());
}

math::vec3 &math::vec3::operator+=(const vec3 &v) {
	_x += v.x();
	_y += v.y();
	_z += v.z();
	return *this;
}

math::vec3 math::vec3::operator*(const vec3 &v) {
	return vec3(_x*v.x(), _y*v.y(), _z*v.z());
}

math::vec3 math::vec3::operator*(float s) const {
	return vec3(_x*s,_y*s,_z*s);
}

math::vec3 &math::vec3::operator*=(float s) {
	_x *= s;
	_y *= s;
	_z *= s;
	return *this;
}

math::vec3 math::vec3::operator/(float s) const {
	return vec3(_x/s, _y/s, _z/s);
}

math::vec3 &math::vec3::operator/=(float s) {
	_x /= s;
	_y /= s;
	_z /= s;
	return *this;
}

float math::vec3::magnitude_sqrd() const {
	return _x * _x + _y * _y + _z * _z;
}

float math::vec3::magnitude() const {
	return std::sqrt(magnitude_sqrd());
}
