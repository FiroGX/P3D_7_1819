#include "camera.hpp"

p3d::camera::camera(float eye_x, float eye_y, float eye_z,
	float at_x, float at_y, float at_z,
	float up_x, float up_y, float up_z,
	float hither, float fov, float width, float height) {
	_eye = new math::vec3(eye_x, eye_y, eye_z);
	_at = new math::vec3(at_x, at_y, at_z);
	_up = new math::vec3(up_x, up_y, up_z);
	_hither = hither; _fov = fov; _width = width; _height = height;
}

p3d::camera::~camera() {}

math::vec3 *p3d::camera::eye() const {
	return _eye;
}

math::vec3 *p3d::camera::at() const {
	return _at;
}

math::vec3 *p3d::camera::up() const {
	return _up;
}

float p3d::camera::hither() const {
	return _hither;
}

float p3d::camera::fov() const {
	return _fov;
}

float p3d::camera::width() const {
	return _width;
}

float p3d::camera::height() const {
	return _height;
}