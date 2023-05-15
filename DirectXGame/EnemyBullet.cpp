#include "EnemyBullet.h"
#include <assert.h>
#include "MathUtils.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
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
	worldTransform_.scale_.z = 3.0f;
	//回転のセット
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	Vector3 velocityXZ = Vector3{velocity_.x, 0, velocity_.z};
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, Length(velocityXZ));
}

void EnemyBullet::Update() {

	// 速度加算
	worldTransform_.translation_ += velocity_;

	// デスタイマーが0になったら死亡フラグを立てる
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 行列更新
	worldTransform_.UpdateMatrix();

}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
