#pragma once
#include "math/vec3.hpp"

namespace p3d {
	class light {
		math::vec3 *_pos;
		math::vec3 *_color;

	public:
		light(float, float, float, float, float, float);
		~light();

		math::vec3 *pos() const;
		math::vec3 *color() const;
	};
}