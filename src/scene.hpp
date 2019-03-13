#pragma once
#include <vector>
#include "light.hpp"
#include "camera.hpp"
#include "scene_obj.hpp"
#include "sphere.hpp"
#include "math/vec3.hpp"
#include <fstream>

namespace p3d {
	class scene {
		camera *_cam;
		math::vec3 *_b_color;
		std::vector<light*> *_lights;
		std::vector<scene_obj*> *_objs;

	public:
		scene() {
					_objs = new std::vector<p3d::scene_obj*>;
					_lights = new std::vector<light*>;
					_b_color = new math::vec3(0.0f,0.0f,0.0f);
		};
		~scene();

		camera *cam() const;
		math::vec3 *b_color() const;
		std::vector<light*> *lights() const;
		std::vector<scene_obj*> *objs() const;
		bool load_nff(std::string);
	};
}