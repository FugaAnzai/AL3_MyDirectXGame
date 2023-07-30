#include "MathUtils.h"
#include <assert.h>
#include <cmath>
#include "PrimitiveDrawer.h"
#include <numbers>

Matrix4x4 Inverse(const Matrix4x4& m) {
	float A = (m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]) +
	          (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]) +
	          (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]) -
	          (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]) -
	          (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]) -
	          (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]) -
	          (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]) -
	          (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]) -
	          (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]) +
	          (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]) +
	          (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]) +
	          (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]) +
	          (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]) +
	          (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]) +
	          (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]) -
	          (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]) -
	          (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]) -
	          (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]) -
	          (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]) -
	          (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]) -
	          (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]) +
	          (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]) +
	          (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]) +
	          (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

	Matrix4x4 result = MakeIdentity4x4();

	result.m[0][0] = 1 / A *
	                 ((m.m[1][1] * m.m[2][2] * m.m[3][3]) + (m.m[1][2] * m.m[2][3] * m.m[3][1]) +
	                  (m.m[1][3] * m.m[2][1] * m.m[3][2]) - (m.m[1][3] * m.m[2][2] * m.m[3][1]) -
	                  (m.m[1][2] * m.m[2][1] * m.m[3][3]) - (m.m[1][1] * m.m[2][3] * m.m[3][2]));

	result.m[0][1] = 1 / A *
	                 (-(m.m[0][1] * m.m[2][2] * m.m[3][3]) - (m.m[0][2] * m.m[2][3] * m.m[3][1]) -
	                  (m.m[0][3] * m.m[2][1] * m.m[3][2]) + (m.m[0][3] * m.m[2][2] * m.m[3][1]) +
	                  (m.m[0][2] * m.m[2][1] * m.m[3][3]) + (m.m[0][1] * m.m[2][3] * m.m[3][2]));

	result.m[0][2] = 1 / A *
	                 ((m.m[0][1] * m.m[1][2] * m.m[3][3]) + (m.m[0][2] * m.m[1][3] * m.m[3][1]) +
	                  (m.m[0][3] * m.m[1][1] * m.m[3][2]) - (m.m[0][3] * m.m[1][2] * m.m[3][1]) -
	                  (m.m[0][2] * m.m[1][1] * m.m[3][3]) - (m.m[0][1] * m.m[1][3] * m.m[3][2]));

	result.m[0][3] = 1 / A *
	                 (-(m.m[0][1] * m.m[1][2] * m.m[2][3]) - (m.m[0][2] * m.m[1][3] * m.m[2][1]) -
	                  (m.m[0][3] * m.m[1][1] * m.m[2][2]) + (m.m[0][3] * m.m[1][2] * m.m[2][1]) +
	                  (m.m[0][2] * m.m[1][1] * m.m[2][3]) + (m.m[0][1] * m.m[1][3] * m.m[2][2]));

	result.m[1][0] = 1 / A *
	                 (-(m.m[1][0] * m.m[2][2] * m.m[3][3]) - (m.m[1][2] * m.m[2][3] * m.m[3][0]) -
	                  (m.m[1][3] * m.m[2][0] * m.m[3][2]) + (m.m[1][3] * m.m[2][2] * m.m[3][0]) +
	                  (m.m[1][2] * m.m[2][0] * m.m[3][3]) + (m.m[1][0] * m.m[2][3] * m.m[3][2]));

	result.m[1][1] = 1 / A *
	                 ((m.m[0][0] * m.m[2][2] * m.m[3][3]) + (m.m[0][2] * m.m[2][3] * m.m[3][0]) +
	                  (m.m[0][3] * m.m[2][0] * m.m[3][2]) - (m.m[0][3] * m.m[2][2] * m.m[3][0]) -
	                  (m.m[0][2] * m.m[2][0] * m.m[3][3]) - (m.m[0][0] * m.m[2][3] * m.m[3][2]));

	result.m[1][2] = 1 / A *
	                 (-(m.m[0][0] * m.m[1][2] * m.m[3][3]) - (m.m[0][2] * m.m[1][3] * m.m[3][0]) -
	                  (m.m[0][3] * m.m[1][0] * m.m[3][2]) + (m.m[0][3] * m.m[1][2] * m.m[3][0]) +
	                  (m.m[0][2] * m.m[1][0] * m.m[3][3]) + (m.m[0][0] * m.m[1][3] * m.m[3][2]));

	result.m[1][3] = 1 / A *
	                 ((m.m[0][0] * m.m[1][2] * m.m[2][3]) + (m.m[0][2] * m.m[1][3] * m.m[2][0]) +
	                  (m.m[0][3] * m.m[1][0] * m.m[2][2]) - (m.m[0][3] * m.m[1][2] * m.m[2][0]) -
	                  (m.m[0][2] * m.m[1][0] * m.m[2][3]) - (m.m[0][0] * m.m[1][3] * m.m[2][2]));

	result.m[2][0] = 1 / A *
	                 ((m.m[1][0] * m.m[2][1] * m.m[3][3]) + (m.m[1][1] * m.m[2][3] * m.m[3][0]) +
	                  (m.m[1][3] * m.m[2][0] * m.m[3][1]) - (m.m[1][3] * m.m[2][1] * m.m[3][0]) -
	                  (m.m[1][1] * m.m[2][0] * m.m[3][3]) - (m.m[1][0] * m.m[2][3] * m.m[3][1]));

	result.m[2][1] = 1 / A *
	                 (-(m.m[0][0] * m.m[2][1] * m.m[3][3]) - (m.m[0][1] * m.m[2][3] * m.m[3][0]) -
	                  (m.m[0][3] * m.m[2][0] * m.m[3][1]) + (m.m[0][3] * m.m[2][1] * m.m[3][0]) +
	                  (m.m[0][1] * m.m[2][0] * m.m[3][3]) + (m.m[0][0] * m.m[2][3] * m.m[3][1]));

	result.m[2][2] = 1 / A *
	                 ((m.m[0][0] * m.m[1][1] * m.m[3][3]) + (m.m[0][1] * m.m[1][3] * m.m[3][0]) +
	                  (m.m[0][3] * m.m[1][0] * m.m[3][1]) - (m.m[0][3] * m.m[1][1] * m.m[3][0]) -
	                  (m.m[0][1] * m.m[1][0] * m.m[3][3]) - (m.m[0][0] * m.m[1][3] * m.m[3][1]));

	result.m[2][3] = 1 / A *
	                 (-(m.m[0][0] * m.m[1][1] * m.m[2][3]) - (m.m[0][1] * m.m[1][3] * m.m[2][0]) -
	                  (m.m[0][3] * m.m[1][0] * m.m[2][1]) + (m.m[0][3] * m.m[1][1] * m.m[2][0]) +
	                  (m.m[0][1] * m.m[1][0] * m.m[2][3]) + (m.m[0][0] * m.m[1][3] * m.m[2][1]));

	result.m[3][0] = 1 / A *
	                 (-(m.m[1][0] * m.m[2][1] * m.m[3][2]) - (m.m[1][1] * m.m[2][2] * m.m[3][0]) -
	                  (m.m[1][2] * m.m[2][0] * m.m[3][1]) + (m.m[1][2] * m.m[2][1] * m.m[3][0]) +
	                  (m.m[1][1] * m.m[2][0] * m.m[3][2]) + (m.m[1][0] * m.m[2][2] * m.m[3][1]));

	result.m[3][1] = 1 / A *
	                 ((m.m[0][0] * m.m[2][1] * m.m[3][2]) + (m.m[0][1] * m.m[2][2] * m.m[3][0]) +
	                  (m.m[0][2] * m.m[2][0] * m.m[3][1]) - (m.m[0][2] * m.m[2][1] * m.m[3][0]) -
	                  (m.m[0][1] * m.m[2][0] * m.m[3][2]) - (m.m[0][0] * m.m[2][2] * m.m[3][1]));

	result.m[3][2] = 1 / A *
	                 (-(m.m[0][0] * m.m[1][1] * m.m[3][2]) - (m.m[0][1] * m.m[1][2] * m.m[3][0]) -
	                  (m.m[0][2] * m.m[1][0] * m.m[3][1]) + (m.m[0][2] * m.m[1][1] * m.m[3][0]) +
	                  (m.m[0][1] * m.m[1][0] * m.m[3][2]) + (m.m[0][0] * m.m[1][2] * m.m[3][1]));

	result.m[3][3] = 1 / A *
	                 ((m.m[0][0] * m.m[1][1] * m.m[2][2]) + (m.m[0][1] * m.m[1][2] * m.m[2][0]) +
	                  (m.m[0][2] * m.m[1][0] * m.m[2][1]) - (m.m[0][2] * m.m[1][1] * m.m[2][0]) -
	                  (m.m[0][1] * m.m[1][0] * m.m[2][2]) - (m.m[0][0] * m.m[1][2] * m.m[2][1]));

	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result = MakeIdentity4x4();
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	result = Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
	return result;
}

void DrawSphere(const Sphere& sphere,const ViewProjection& viewProjection)
{
	const uint32_t kSubdivision = 10;
	const float kLonEvery = (360 / kSubdivision) * ((float)std::numbers::pi / 180);
	const float kLatEvery = (360 / kSubdivision) * ((float)std::numbers::pi / 180);

	const float theta = (float)std::numbers::pi / kSubdivision;
	const float phi = (2 * (float)std::numbers::pi) / kSubdivision;

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -(float)std::numbers::pi / 2.0f + kLatEvery * latIndex;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;
			Vector3 a, b, c;
			a = sphere.center + (sphere.radius * Vector3{ std::cos(lat) * std::cos(lon),std::sin(lat),std::cos(lat) * std::sin(lon) });
			b = sphere.center + (sphere.radius * Vector3{ std::cos(lat + theta) * std::cos(lon),std::sin(lat + theta),std::cos(lat + theta) * std::sin(lon) });
			c = sphere.center + (sphere.radius * Vector3{ std::cos(lat) * std::cos(lon + phi),std::sin(lat),std::cos(lat) * std::sin(lon + phi) });

			PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection);
			PrimitiveDrawer::GetInstance()->DrawLine3d(a, b, Vector4{0, 1.0f, 0, 1.0f});
			PrimitiveDrawer::GetInstance()->DrawLine3d(a, c, Vector4{0, 1.0f, 0, 1.0f});

		}



	}


}