#pragma once
#include "math/vec3.hpp"

namespace p3d {
	class light {
		math::vec3 _pos;
		math::vec3 _color;

	public:
		light(math::vec3, math::vec3);
		~light();

		math::vec3 pos() const;
		math::vec3 color() const;

		void pos(math::vec3);
		void color(math::vec3);
	};
}