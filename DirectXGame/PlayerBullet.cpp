#include "PlayerBullet.h"
#include <assert.h>
#include "MathUtils.h"
#include "CollsionConfig.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) { 
	assert(model);
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.jpg");
	//発生位置のセット
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	//衝突属性と衝突マスクの設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(~kCollisionAttributePlayer);
	//初期速度のセット
	velocity_ = velocity;
}

void PlayerBullet::Update() {

	//速度加算
	worldTransform_.translation_ += velocity_;

	//デスタイマーが0になったら死亡フラグを立てる
	if (--deathTimer_ <= 0) {
		isDead_ = true;	
	}

	//行列更新
	worldTransform_.UpdateMatrix();

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection,textureHandle_);
}

void PlayerBullet::OnCollision() {

	isDead_ = true;
}
