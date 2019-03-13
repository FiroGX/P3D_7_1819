#pragma once
#include "material.hpp"
#include "math/vec3.hpp"

namespace p3d {
	class hit {
		math::vec3 _point;
		math::vec3 _normal;
		material _mat;
		bool _collided;
		float _distance;

	public:
		hit(math::vec3, math::vec3, material, float, bool);
		hit();
		~hit();

		math::vec3 point() const;
		math::vec3 normal() const;
		material mat() const;
		bool collided() const;
		float distance() const;

		void distance(float);
	};
}