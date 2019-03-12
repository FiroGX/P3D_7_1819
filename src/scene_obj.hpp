#pragma once
#include "material.hpp"
#include "hit.hpp"
#include "ray.hpp"

namespace p3d {
	class scene_obj {
		material *_mat;

	public:
		scene_obj(material *mat) { _mat = mat; }
		~scene_obj() = default;

		virtual hit calculate_intersection(ray &) = 0;
	};
}