#pragma once
#include "math/vec3.hpp"

namespace p3d {
	class light {
		protected:
			math::vec3 _pos;
			math::vec3 _color;
			bool _is_area = false;

	public:
		light(math::vec3, math::vec3);
		~light();

		virtual math::vec3 pos() const;
		math::vec3 color() const;
		bool is_area() const;

		virtual void pos(math::vec3);
		void color(math::vec3);

	};
}
