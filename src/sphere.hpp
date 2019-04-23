#pragma once

#include "scene_obj.hpp"
#include "hit.hpp"
#include "ray.hpp"

namespace p3d {
	class sphere : public scene_obj{

		math::vec3 _center;
		float _radius;

	public:
		sphere(math::vec3, float, material);
		~sphere();

		const math::vec3 &center();
		float radius();

		virtual hit calculate_intersection(const ray &) const;
        virtual b_box box() const;
        virtual std::string dump() const;
	};
}