#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "TextureManager.h"

class PlayerBullet {
public:
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	
	bool GetIsDead() const { return isDead_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_;
	//テクスチャハンドル
	uint32_t textureHandle_;
	//弾の速度
	Vector3 velocity_;
	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//フレームタイマー
	int32_t deathTimer_ = kLifeTime;
	//死亡フラグ
	bool isDead_ = false;

};
