#pragma once
#define _USE_MATH_DEFINES

#define DEGTORAD M_PI / 180.0

#include <math.h>


namespace Math {
	template<typename T>
	T Clamp(T num, T min, T max) {
		if (num < min) return min;
		if (num > max) return max;
		return num;
	}

}

struct Vector {
	Vector() : x(0), y(0), z(0) {}
	Vector(double x, double y, double z) : x(x), y(y), z(z) {}

	Vector operator+(const Vector& rhs) {
		return Vector(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	Vector operator-(const Vector& rhs) {
		return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	Vector operator*(const double& scalar) {
		return Vector(x * scalar, y * scalar, z * scalar);
	}

	Vector operator*(const Vector& rhs) {
		return Vector(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	Vector operator-() {
		return Vector(-x, -y, -z);
	}

	static double DotProduct(const Vector& lhs, const Vector& rhs) {
		return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
	}

	static Vector CrossProduct(const Vector& lhs, const Vector& rhs) {
		return Vector((lhs.y * rhs.z) - (lhs.z * rhs.y), (lhs.z * rhs.x) - (lhs.x * rhs.z), (lhs.x * rhs.y) - (lhs.y * rhs.x));
	}

	static Vector rotateAroundAxis(Vector vec, double theta, Vector axis) {
		Vector result;
		result = (vec * cos(theta)) + (CrossProduct(axis, vec) * sin(theta)) + (axis * DotProduct(axis, vec) * (1 - cos(theta)));
		return result;
	}

	static double Length(Vector vec) {
		return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
	}

	static Vector Normalize(Vector vec) {
		double vecLength = Length(vec);
		if (vecLength == 0.0) {
			//TODO: decide if this is a good idea
			return Vector(1.0, 0.0, 0.0);
		}
		double invLength = 1.0 / vecLength;
		return Vector(vec.x * invLength, vec.y * invLength, vec.z * invLength);
	}

	// Class member variables
	double x, y, z;

};

struct Camera {
	Vector pos;
	Vector dir;
};

struct Colour {
	Colour() : r(0.0), g(0.0), b(0.0), a(1.0) {}
	Colour(double r, double g, double b) : r(r), g(g), b(b), a(1.0) {}
	Colour(double r, double g, double b, double a) : r(r), g(g), b(b), a(a) {}

	Colour operator+(const Colour& rhs) {
		Colour finalColour = Colour(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
		// TODO: simd?
		finalColour.r = Math::Clamp(finalColour.r, 0.0, 1.0);
		finalColour.g = Math::Clamp(finalColour.g, 0.0, 1.0);
		finalColour.b = Math::Clamp(finalColour.b, 0.0, 1.0);
		finalColour.a = Math::Clamp(finalColour.a, 0.0, 1.0);

		return finalColour;
	}

	Colour& operator+=(const Colour& rhs) {
		*this = *this + rhs;
		return *this;
	}

	double r, g, b, a;
};



