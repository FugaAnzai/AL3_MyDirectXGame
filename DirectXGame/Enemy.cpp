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

	switch (phase_) {
	case Enemy::Phase::Approach:
		Approach();
		break;
	case Enemy::Phase::Leave:
		Leave();
		break;
	}

	ImGui::Begin("Enemy");
	float inputTranslation[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z};
	ImGui::InputFloat3("translation", inputTranslation);
	ImGui::End();

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Approach() {

	const float kApproachSpeed = -0.2f;

	worldTransform_.translation_ += Vector3(0, 0, kApproachSpeed);

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

}

void Enemy::Leave() {

	const float kLeaveSpeed = -0.2f;

	worldTransform_.translation_ += Vector3(kLeaveSpeed, 0, kLeaveSpeed);

}