#include "sphere.hpp"

public namespace p3d {

	sphere::sphere(math::vec3 * center, float radius) : scene_obj()
	{
		_center = center;
		_radius = radius;
		mat = nullptr;
	}

	sphere::~sphere()
	{
		//TODO
	}

	math::vec3 * sphere::center()
	{
		return _center;
	}

	float sphere::radius()
	{
		return _radius;
	}

	hit sphere::calculate_intersection(ray &ray)
	{
		return hit();
	}

}
