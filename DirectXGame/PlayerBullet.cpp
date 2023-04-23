#include "PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) { 
	assert(model);
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.jpg");
	//発生位置のセット
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	//初期速度のセット
	velocity_ = velocity;
}

void PlayerBullet::Update() {

	worldTransform_.translation_ += velocity_;

	if (--deathTimer_ <= 0) {
		isDead_ = true;	
	}

	worldTransform_.UpdateMatrix();

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection,textureHandle_);
}