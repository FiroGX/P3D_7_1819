#pragma once
#include "material.hpp"
#include "math/vec3.hpp"

namespace p3d {
	class hit {
		math::vec3 *_point;
		math::vec3 *_normal;
		material *_mat;
		bool _collided;

	public:
		hit(math::vec3 *, math::vec3 *, material *, bool);
		hit();
		~hit();

		math::vec3 *point() const;
		math::vec3 *normal() const;
		material *mat() const;
		bool collided() const;
	};
}