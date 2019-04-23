#pragma once
#include "material.hpp"
#include "b_box.hpp"
#include "hit.hpp"
#include "ray.hpp"
#include <iostream>
#include <string>

namespace p3d {
	class scene_obj {
	protected:
		material _mat;

	public:
		scene_obj(material mat) { _mat = mat; }
		~scene_obj() = default;

		virtual hit calculate_intersection(const ray &) const = 0;
        virtual b_box box() const = 0;
        virtual std::string dump() const = 0;
	};
}