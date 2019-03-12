#pragma once
#include "math/vec3.hpp"

namespace p3d {
	class ray {
		math::vec3 _o;
		math::vec3 _d;
	public:
		ray();
		ray(math::vec3, math::vec3);
		~ray();

		math::vec3 o() const;
		math::vec3 d() const;

		void o(const math::vec3);
		void d(const math::vec3);
	};
}