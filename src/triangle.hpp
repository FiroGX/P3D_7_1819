#pragma once

#include "scene_obj.hpp"

namespace p3d {
	class triangle : public scene_obj {

		math::vec3 _a, _b, _c;

	public:
		triangle(math::vec3, math::vec3, math::vec3, material);
	    ~triangle();

		const math::vec3 &a();
		const math::vec3 &b();
		const math::vec3 &c();

		virtual hit calculate_intersection(const ray &) const;
        virtual b_box box() const;
        virtual std::string dump() const;
	};
}