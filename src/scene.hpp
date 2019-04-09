#pragma once
#include <vector>
#include "light.hpp"
#include "camera.hpp"
#include "scene_obj.hpp"
#include "math/vec3.hpp"

namespace p3d {
	class scene {
		camera _cam;
		math::vec3 _b_color;
		std::vector<light*> _lights;
		std::vector<scene_obj*> _objs;

	public:
		scene();
		~scene();

		camera cam() const;
		math::vec3 b_color() const;
		std::vector<light*> lights() const;
		std::vector<scene_obj*> objs() const;
        hit calculate_hit(const ray &) const;
		bool load_nff(const std::string);
	};
}