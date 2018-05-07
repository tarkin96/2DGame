#include <cmath>
#include <string>
#include <iostream>
#include "vector3f.h"

Vector3f& Vector3f::operator=(const Vector3f& rhs) {
  if ( this != &rhs ) {
	  v[0] = rhs[0];
	  v[1] = rhs[1];
	  v[2] = rhs[2];
  }
  return *this;
}

Vector3f::Vector3f(float x, float y, float z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

float &Vector3f::operator[](int index) {
	return v[index];
}

float Vector3f::operator[](int index) const {
	return v[index];
}

Vector3f Vector3f::operator*(float scale) const {
	return Vector3f(v[0] * scale, v[1] * scale, v[2] * scale);
}

Vector3f Vector3f::operator/(float scale) const {
  if ( scale < 0.001 && scale > -0.001 ) {
    throw std::string("scale too small in /");
  }
	return Vector3f(v[0] / scale, v[1] / scale, v[2] / scale);
}

Vector3f Vector3f::operator+(const Vector3f &other) const {
	return Vector3f(v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2]);
}

Vector3f Vector3f::operator-(const Vector3f &other) const {
	return Vector3f(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2]);
}

Vector3f Vector3f::operator-() const {
	return Vector3f(-v[0], -v[1], -v[2]);
}

const Vector3f &Vector3f::operator*=(float scale) {
	v[0] *= scale;
	v[1] *= scale;
	v[2] *= scale;
	return *this;
}

const Vector3f &Vector3f::operator/=(float scale) {
  if ( scale < 0.001 && scale > -0.001 ) {
    throw std::string("scale too small in /=");
  }
	v[0] /= scale;
	v[1] /= scale;
	v[2] /= scale;
	return *this;
}

const Vector3f &Vector3f::operator+=(const Vector3f &other) {
	v[0] += other.v[0];
	v[1] += other.v[1];
	v[2] += other.v[2];
	return *this;
}

const Vector3f &Vector3f::operator-=(const Vector3f &other) {
	v[0] -= other.v[0];
	v[1] -= other.v[1];
	v[2] -= other.v[2];
	return *this;
}

float Vector3f::magnitude() const {
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float Vector3f::magnitudeSquared() const {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

Vector3f Vector3f::normalize() const {
	float m = sqrt(v[0] * v[0] + v[1] * v[1] + + v[2] * v[2]);
  if ( m < 0.001 && m > -0.001 ) {
    throw std::string("Point too close in Vector3f::normalize");
  }
	return Vector3f(v[0] / m, v[1] / m, v[2] / m);
}

float Vector3f::dot(const Vector3f &other) const {
	return v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2];
}

Vector3f Vector3f::cross(const Vector3f &) const {
	throw std::string("Cross Product not implemented for vector3f!"); 
}

Vector3f operator*(float scale, const Vector3f &v) {
	return v * scale;
}

std::ostream &operator<<(std::ostream &output, const Vector3f &v) {
	output << '(' << v[0] << ", " << v[1]  << ", " << v[2] << ')';
	return output;
}
