#include "RailCamera.h"
#include "MathUtils.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& radian) {
	
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = radian;
	worldTransform_.UpdateMatrix();
	viewProjection_.Initialize();

}

void RailCamera::Update() {

	Vector3 moveSpeed = Vector3{0,0,0.0f};

	worldTransform_.translation_ += moveSpeed;

	worldTransform_.UpdateMatrix();

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	/*ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.1f, -100,100);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.1f, -100,100);
	ImGui::End();*/

}
