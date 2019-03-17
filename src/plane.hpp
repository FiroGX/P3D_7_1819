#pragma once

#include "scene_obj.hpp"

namespace p3d {
	class plane : public scene_obj {
		math::vec3 _point;
		math::vec3 _normal;

	public:
		plane(math::vec3, math::vec3, math::vec3, material);
		~plane();

		math::vec3 point();
		math::vec3 normal();
		virtual hit calculate_intersection(const ray &) const;
	};
}