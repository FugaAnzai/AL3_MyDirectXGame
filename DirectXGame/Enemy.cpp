#include "Enemy.h"
#include "TextureManager.h"
#include <assert.h>
#include "ImGuiManager.h"
#include "Vector3.h"

void Enemy::Initialize(Model* model) { 

	const Vector3 initPosition = Vector3(0, 5, 100);

	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = initPosition;
	textureHandle_ = TextureManager::Load("eyes.jpg");
}

void Enemy::Update() {

	Move();

	ImGui::Begin("Enemy");
	float inputTranslation[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z};
	ImGui::InputFloat3("translation", inputTranslation);
	ImGui::End();

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Move() {

	const float kMoveSpeed = -0.2f;

	worldTransform_.translation_ += Vector3(0, 0, kMoveSpeed);

}