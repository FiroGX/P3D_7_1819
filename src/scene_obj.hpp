#pragma once
#include "material.hpp"

namespace p3d {
	class scene_obj {
		material *mat;

	public:
		scene_obj() = default;
		~scene_obj() = default;
	};
}