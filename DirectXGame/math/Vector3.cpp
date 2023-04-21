#include "Vector3.h"
#include <math.h>
#include <assert.h>

Vector3::Vector3() {
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 Vector3::Add(Vector3 v1, Vector3 v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

Vector3 Vector3::Subtract(Vector3 v1, Vector3 v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

Vector3 Vector3::Multiply(float scalar, Vector3 v) {
	Vector3 result;
	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;
	return result;
}

float Vector3::Dot(Vector3 v1,Vector3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Vector3::Length(Vector3 v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Vector3::Normalize(Vector3 v) {
	Vector3 result;
	float length = Length(v);
	result.x = v.x / length;
	result.y = v.y / length;
	result.z = v.z / length;
	return result;
}

Vector3 Vector3::Transform(const Vector3& vector, const Matrix4x4& matrix) {
	
	Vector3 result;

	result = vector * matrix;
	
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1 * matrix.m[3][3];
	assert(w != 0.0f);
	result *= 1.0f / w;
	
	return result;
}


Vector3 Vector3::operator+() const { return *this; }

Vector3 Vector3::operator-() const { return { -x, -y,-z }; }

Vector3 Vector3::operator+(const Vector3& other) const {
	return { x + other.x, y + other.y, z + other.z };
}

Vector3 Vector3::operator-(const Vector3& other) const {
	return { x - other.x, y - other.y, z - other.z };
}

Vector3 Vector3::operator*(float s) const {
	return { x * s, y * s, z * s };
}

Vector3 Vector3::operator*(const Matrix4x4& matrix) const {
	Vector3 result;

	result.x = x * matrix.m[0][0] + y * matrix.m[1][0] + z * matrix.m[2][0] + 1 * matrix.m[3][0];
	result.y = x * matrix.m[0][1] + y * matrix.m[1][1] + z * matrix.m[2][1] + 1 * matrix.m[3][1];
	result.z = x * matrix.m[0][2] + y * matrix.m[1][2] + z * matrix.m[2][2] + 1 * matrix.m[3][2];

	return result;
}

Vector3 Vector3::operator/(float s) const {
	return { x / s, y / s, z / s };
}

Vector3& Vector3::operator +=(const Vector3& other) {
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

Vector3& Vector3::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;

	return *this;

}

Vector3& Vector3::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;

	return *this;

}

inline Vector3 operator*(float s, const Vector3& v) {
	return { s * v.x, s * v.y, s * v.z };
}