#pragma once
#include "Matrix4x4.h"

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

public:
	Vector3();

	Vector3(float x, float y, float z);

	Vector3 Add(Vector3 v1, Vector3 v2);

	Vector3 Subtract(Vector3 v1, Vector3 v2);

	Vector3 Multiply(float scalar, Vector3 v);

	float Dot(Vector3 v1, Vector3 v2);

	float Length(Vector3 v);

	Vector3 Normalize(Vector3 v);

	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	Vector3 operator+() const;

	Vector3 operator-() const;

	Vector3 operator+(const Vector3& other) const;

	Vector3 operator-(const Vector3& other) const;

	Vector3 operator*(float s) const;

	Vector3 operator*(const Matrix4x4& matrix) const;

	Vector3 operator/(float s) const;

	Vector3& operator+=(const Vector3& other);

	Vector3& operator-=(const Vector3& other);

	Vector3& operator*=(float s);

	Vector3& operator/=(float s);

};

inline Vector3 operator*(float s, const Vector3& v);