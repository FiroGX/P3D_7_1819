#pragma once
#include "math/vec3.hpp"

namespace p3d {
	class material {
		math::vec3 _color;
		float _kd; // diffuse component
		float _ks; // specular component
		float _shine; // phong cosine power
		float _t; // transmitance
		float _ref_index; // index of refraction

	public:
		material(math::vec3, float, float, float, float, float);
		material();
		~material();

		math::vec3 color() const;
		float kd() const;
		float ks() const;
		float shine() const;
		float t() const;
		float ref_index() const;
	};
}