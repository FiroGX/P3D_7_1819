#include "scene.hpp"

p3d::camera *p3d::scene::cam() const {
	return _cam;
}

math::vec3 *p3d::scene::b_color() const {
	return _b_color;
}

std::vector<p3d::light*> *p3d::scene::lights() const {
	return _lights;
}

std::vector<p3d::scene_obj*> *p3d::scene::objs() const {
	return _objs;
}
