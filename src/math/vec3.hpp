#pragma once

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
	inline vec3 operator*(float, const vec3 &);

	// Dot product
	inline float dot(const vec3 &, const vec3 &);

	// Cross product
	inline vec3 cross(const vec3 &, const vec3 &);

	// Normalization
	inline vec3 normalize(const vec3 &);
}