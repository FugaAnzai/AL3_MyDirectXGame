#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "TextureManager.h"

class Player;

class EnemyBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity, Player* player);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void OnCollision();

	bool GetIsDead() const { return isDead_; }
	Vector3 GetWorldPostion() {
		Vector3 result;
		result.x = worldTransform_.matWorld_.m[3][0];
		result.y = worldTransform_.matWorld_.m[3][1];
		result.z = worldTransform_.matWorld_.m[3][2];
		return result;
	}

	void SetPlayer(Player* player) { player_ = player; }

	static const int kRadius = 1;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_;
	// テクスチャハンドル
	uint32_t textureHandle_;
	// 弾の速度
	Vector3 velocity_;
	// 寿命
	static const int32_t kLifeTime = 60 * 5;
	// フレームタイマー
	int32_t deathTimer_ = kLifeTime;
	// 死亡フラグ
	bool isDead_ = false;
	// プレイヤー
	Player* player_ = nullptr;

};
