/*made this class myself, inspired by Vector2f class*/
//used for objects that contain 3 values, such as rgb colors

#ifndef VECTOR3F__H
#define VECTOR3F__H

#include <iostream>

class Vector3f {
public:
  explicit Vector3f(float x = 0, float y = 0, float z = 0);

  float &operator[](int index);
  float operator[](int index) const;

  bool operator==(const Vector3f &other) const {
    return (v[0]==other.v[0] && v[1] == other.v[1]);
  }

  bool operator!=(const Vector3f &other) const {
    return (v[0]!=other.v[0] or v[1] != other.v[1]);
  }
		
  Vector3f operator*(float scale) const;
  Vector3f operator/(float scale) const;

  Vector3f operator+(const Vector3f &other) const;
  Vector3f operator-(const Vector3f &other) const;
  Vector3f operator-() const;
		
  const Vector3f &operator*=(float scale);
  const Vector3f &operator/=(float scale);
  const Vector3f &operator+=(const Vector3f &other);
  const Vector3f &operator-=(const Vector3f &other);
		
  float magnitude() const;
  float magnitudeSquared() const;
  Vector3f normalize() const;
  float dot(const Vector3f &other) const;
  Vector3f cross(const Vector3f &other) const;
  Vector3f& operator=(const Vector3f&);
private:
  float v[3];
};

Vector3f operator*(float scale, const Vector3f &v);
std::ostream &operator<<(std::ostream &output, const Vector3f &v);

#endif
