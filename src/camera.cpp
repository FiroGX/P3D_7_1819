#include <cmath>
#include "camera.hpp"

p3d::camera::camera(math::vec3 eye, math::vec3 at, math::vec3 up, float hither, float fov, int resX, int resY) {

	_eye = eye; _at = at;	_up = up;	_hither = hither;
	_fov = math::radian(fov); _resX = resX; _resY = resY;

	_ze = math::normalize(_eye - _at); //vector from at to the eye
	_xe = math::normalize(math::cross(_up, _ze));	//cross between up vector and Ze
	_ye = math::cross(_ze, _xe); //cross between Ze and Xe

	_df = (_at - _eye).magnitude(); //distance from eye to at
	_height = 2.0f * _df * std::tanf(_fov / 2);	//height and width calculations
	_width = (_resX / _resY) * _height;	//based on resolution and fovy
}

p3d::camera::camera() {}

p3d::camera::~camera() {}

/* Primary Ray casting method */
p3d::ray p3d::camera::primaryRay(float x, float y) {
	float pixel_x = _width * (x / _resX - 0.5f); //x component on the pixel
	float pixel_y = _height * (y / _resY - 0.5f); //y component on the pixel

	math::vec3 dir(-_df * _ze + pixel_x * _xe + pixel_y * _ye);

	return ray(_eye,dir);
}

p3d::ray p3d::camera::castRay(float x, float y) { // STILL NOT COMPLETE this is suposed to return a ray from the lens to a point in the xy position of the viewplane
	float pixel_x = _width * (x / _resX - 0.5f); //x component on the pixel
	float pixel_y = _height * (y / _resY - 0.5f); //y component on the pixel

	math::vec3 dir(-_df * _ze + pixel_x * _xe + pixel_y * _ye);

	return ray(_eye, dir);
}

math::vec3 p3d::camera::eye() const {
	return _eye;
}

math::vec3 p3d::camera::at() const {
	return _at;
}

math::vec3 p3d::camera::up() const {
	return _up;
}

math::vec3 p3d::camera::xe() const {
	return _xe;
}

math::vec3 p3d::camera::ye() const {
	return _ye;
}

math::vec3 p3d::camera::ze() const {
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

float p3d::camera::df() const {
	return _df;
}

int p3d::camera::resX() const {
	return _resX;
}

int p3d::camera::resY() const {
	return _resY;
}
