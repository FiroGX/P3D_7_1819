#include "camera.hpp"

p3d::camera::camera(math::vec3 *eye, math::vec3 *at, math::vec3 *up, float hither, float fov, int resX, int resY) {

	_eye = eye; _at = at;	_up = up;	_hither = hither;
	_fov = fov; _resX = resX; _resY = resY;

	_ze = math::normalize(_eye - _at);
	_xe = math::normalize(math::cross(_up, _ze));
	_ye = math::cross(_ze, _xe);

	_height = 2 * _ze * tanf(_fovy / 2);
	_width = (resX() / resY()) * _height;
}

p3d::camera::~camera() {}

ray camera::primaryRay(int x, int y) {
	ray ray;
	math::vec3 df = math::normalize(_at - _eye);
	float pixel_x = _width * (x / _resX - 0.5);
	float pixel_y = _height * (y / _resY - 0.5);

	ray.o(*_eye);
	ray.d(math::normalize(-df * _ze + pixel_x * _xe + pixel_y * _ye));
	return ray;
}

math::vec3 *p3d::camera::eye() const {
	return _eye;
}

math::vec3 *p3d::camera::at() const {
	return _at;
}

math::vec3 *p3d::camera::up() const {
	return _up;
}

math::vec3 *p3d::camera::xe() const {
	return _xe;
}

math::vec3 *p3d::camera::ye() const {
	return _ye;
}

math::vec3 *p3d::camera::ze() const {
	return _ze;
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

int p3d::camera::resX() const {
	return _resX;
}

int p3d::camera::resY() const {
	return _resY;
}
