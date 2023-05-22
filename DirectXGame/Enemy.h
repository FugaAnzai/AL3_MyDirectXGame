#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "EnemyState.h"
#include "EnemyBullet.h"
#include <list>
#include "TimedCall.h"
#include "MathUtils.h"
#include "Collider.h"

class Player;

class Enemy : public Collider {
public:
	~Enemy();
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Move(const Vector3& move);
	void Fire();
	void FireAndReload();
	void FireReset();
	void ChangeState(BaseEnemyState* newState);
	void OnCollision() override;

	//getter,setter
	WorldTransform GetWorldTransfrom() { return worldTransform_; }
	uint32_t GetFireTimer() { return fireTimer_; }
	Vector3 GetWorldPosition() override {
		Vector3 result;
		result.x = worldTransform_.matWorld_.m[3][0];
		result.y = worldTransform_.matWorld_.m[3][1];
		result.z = worldTransform_.matWorld_.m[3][2];
		return result;
	}
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	void SetFireTimer(uint32_t fireTimer) { fireTimer_ = fireTimer; }
	void SetPlayer(Player* player) { player_ = player; }

	//定数　
	static const int kFireInterval = 60;
	static const int kRadius = 1;

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデルデータ
	Model* model_;
	//テクスチャハンドル
	uint32_t textureHandle_;
	//状態遷移
	BaseEnemyState* state_; 
	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//弾のタイマー
	uint32_t fireTimer_ = 0;
	//時限発動のリスト
	std::list<std::unique_ptr<TimedCall>> timedCalls_;
	//プレイヤーのデータ
	Player* player_ = nullptr;
};
