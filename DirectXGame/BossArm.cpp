#include "BossArm.h"
#include <cassert>
#include "CollsionConfig.h"
#include "Player.h"
#include "ImGuiManager.h"

void BossArm::Initialize(Model* model, ArmDirection armDirection) {

	// モデルがなければ中止
	assert(model);
	// モデル代入
	model_ = model;
	//向き代入
	armDirection_ = armDirection;
	// ワールド変換データの初期化
	worldTransform_.Initialize();
	if (armDirection_ == ArmDirection::Right) {
		worldTransform_.translation_ = Vector3{150.0f, 0.0f, 500.0f};
		worldTransform_.rotation_ = Vector3{0.0f, 17.3f, 0.0f};
	}

	if (armDirection_ == ArmDirection::Left) {
		worldTransform_.translation_ = Vector3{-150.0f, 0.0f, 500.0f};
		worldTransform_.rotation_ = Vector3{0.0f, 17.3f, 0.0f};
	}

	worldTransform_.scale_ = Vector3{1.0f, 1.0f, 1.0f};
	
	worldTransform_.UpdateMatrix();

	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(~kCollisionAttributeEnemy);
	SetRadius(kRadius);

}

void BossArm::Update(Vector3 velocity) {

	worldTransform_.translation_ += velocity;

	/*ImGui::Begin("BossArm");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.1f, -1000, 1000);
	ImGui::End();*/

	worldTransform_.UpdateMatrix();

}

void BossArm::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection);

}

void BossArm::OnCollision() {}
