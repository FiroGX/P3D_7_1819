#pragma once
#include <cmath>
#define _USE_MATH_DEFINES

namespace math {
	class vec3 {
		float _x;
		float _y;
		float _z;

	public:
		vec3();
		vec3(float, float, float);

		float x() const;
		float y() const;
		float z() const;

		// Vector negation
		vec3 operator-() const;

		// Vector addition
		vec3 operator-(const vec3 &) const;
		vec3 &operator-=(const vec3 &);

		// Vector subtraction
		vec3 operator+(const vec3 &) const;
		vec3 &operator+=(const vec3 &);

		// Vector multiplication
		vec3 operator*(const vec3&);

		// Scalar multiplication
		vec3 operator*(float) const;
		vec3 &operator*=(float);

		// Scalar division
		vec3 operator/(float) const;
		vec3 &operator/=(float);

		// Magnitude
		float magnitude_sqrd() const;
		float magnitude() const;
	};

	// Reversed scalar multiplication
	inline vec3 operator*(float s, const vec3 &v) {
		return v * s;
	}

	// Dot product
	inline float dot(const vec3 &v1, const vec3 &v2) {
		return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
	}

	// Cross product
	inline vec3 cross(const vec3 &v1, const vec3 &v2) {
		return vec3((v1.y() * v2.z()) - (v1.z() * v2.y()),
			(v1.z() * v2.x()) - (v1.x() * v2.z()),
			(v1.x() * v2.y()) - (v1.y() * v2.x()));
	}

	// Normalization
	inline vec3 normalize(const vec3 &v) {
		return v / v.magnitude();
	}

	// Degrees to Radians conversion
	inline float radian(const float degrees) {
		return degrees * M_PI / 180;
	}
}
