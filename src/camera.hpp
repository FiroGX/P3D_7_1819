#pragma once
#include "math/vec3.hpp"

namespace p3d {
	class camera {
		math::vec3 *_eye;
		math::vec3 *_at;
		math::vec3 *_up;
		float _near;
		float _far;
		float _fov;
		float _width;
		float _height;

	public:
		camera(float, float, float,
			float, float, float,
			float, float, float,
			float, float, float, float, float);
		~camera();

		math::vec3 *eye() const;
		math::vec3 *at() const;
		math::vec3 *up() const;
		float near() const;
		float far() const;
		float fov() const;
		float width() const;
		float height() const;
	};
}