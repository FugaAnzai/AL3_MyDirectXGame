#include "EnemyBullet.h"
#include <assert.h>
#include "MathUtils.h"
#include "Player.h"
#include "ImGuiManager.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity,Player* player) {
	assert(model);
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red.jpg");
	// 発生位置のセット
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// 初期速度のセット
	velocity_ = velocity;
	// スケールのセット
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 1.5f;
	//プレイヤーセット
	SetPlayer(player);
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Update() {
	assert(player_);

	Vector3 playerPos = player_->GetWorldPostion();
	Vector3 bulletPos = GetWorldPostion();

	//弾からプレイヤーまで
	Vector3 toPlayer = player_->GetWorldPostion() - GetWorldPostion();

	//assert(Length(toPlayer) > 1.0e-5f);

	//球面線形補間
	velocity_ = Slerp(Normalize(velocity_), Normalize(toPlayer), 0.1f);

	// 速度加算
	worldTransform_.translation_ += velocity_;

	// 回転のセット
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	Vector3 velocityXZ = Vector3{velocity_.x, 0, velocity_.z};
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, Length(velocityXZ));

	// デスタイマーが0になったら死亡フラグを立てる
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	ImGui::Begin("Bullet");
	float inputTranslation[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::InputFloat3("translation", inputTranslation);
	ImGui::End();

	// 行列更新
	worldTransform_.UpdateMatrix();

}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
