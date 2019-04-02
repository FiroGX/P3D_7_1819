#pragma once
#include <cstdlib>
#include "math/vec3.hpp"
#include "light.hpp"

namespace p3d {
	class area_light : public light{
    math::vec3 _vector_a;
    math::vec3 _vector_b;

	public:
    area_light(math::vec3, math::vec3, math::vec3, math::vec3);
		~area_light();

    math::vec3 vector_a() const;
    math::vec3 vector_b() const;

    void vector_a(math::vec3);
    void vector_b(math::vec3);

    math::vec3 pos();
	};
}
