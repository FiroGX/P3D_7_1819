
#pragma once
#include "scene_obj.hpp"
#include "material.hpp"
#include "hit.hpp"
#include "ray.hpp"
#include "math/vec3.hpp"

namespace p3d {
	class sphere : public scene_obj{

		math::vec3 *_center;
		float _radius;

	public:
		sphere(math::vec3*, float);
		~sphere();
		math::vec3 *center();
		float radius();
		virtual hit calculate_intersection(ray &);
	};
}


