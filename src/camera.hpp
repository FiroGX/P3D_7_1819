#pragma once
#include "math/vec3.hpp"
#include "ray.hpp"

namespace p3d {
	class camera {
		math::vec3 _eye;
		math::vec3 _at;
		math::vec3 _up;
		math::vec3 _xe;
		math::vec3 _ye;
		math::vec3 _ze;
		float	   _hither;
		float 	   _fov;
		float 	   _width;
		float 	   _height;
		float	   _df;
		int		   _resX;
		int		   _resY;

	public:
		camera(math::vec3, math::vec3, math::vec3, float, float, int, int);
		camera();
		~camera();

		ray primaryRay(float, float);
		math::vec3 eye() const;
		math::vec3 at() const;
		math::vec3 up() const;
		math::vec3 xe() const;
		math::vec3 ye() const;
		math::vec3 ze() const;
		float hither() const;
		float fov() const;
		float width() const;
		float height() const;
		float df() const;
		int resX() const;
		int resY() const;
	};
}
