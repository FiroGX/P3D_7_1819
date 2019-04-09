#include "area_light.hpp"

p3d::area_light::area_light(math::vec3 position, math::vec3 a, math::vec3 b, math::vec3 color) : light(position, color), _vector_a(a), _vector_b(b) {
	_is_area = true;
};

p3d::area_light::~area_light() {}

//getters
math::vec3 p3d::area_light::vector_a() const {
	return _vector_a;
}
math::vec3 p3d::area_light::vector_b() const {
	return _vector_b;
}

//setters
void p3d::area_light::vector_a(math::vec3 a) {
	_vector_a = a;
}
void p3d::area_light::vector_b(math::vec3 b) {
	_vector_b = b;
}

//random light point for soft shadow calculation
math::vec3 p3d::area_light::pos() {
  //corner point
  math::vec3 c = p3d::light::pos();
  //random epsilons for vectors a and b
  float a_epsilon = static_cast<float> (std::rand()) / static_cast<float>(RAND_MAX);
  float b_epsilon = static_cast<float> (std::rand()) / static_cast<float>(RAND_MAX);
  //random position calculation
  math::vec3 random_position = c + (a_epsilon * _vector_a) + (b_epsilon * _vector_b);

  return random_position;
}
