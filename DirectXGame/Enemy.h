#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "EnemyState.h"
#include "EnemyBullet.h"
#include <list>
#include "TimedCall.h"

class Enemy {
public:
	~Enemy();
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Move(const Vector3& move);
	void Fire();
	void FireAndReset();
	void ChangeState(BaseEnemyState* newState);

	//getter,setter
	WorldTransform GetWorldTransfrom() { return worldTransform_; }
	uint32_t GetFireTimer() { return fireTimer_; }
	void SetFireTimer(uint32_t fireTimer) { fireTimer_ = fireTimer; }

	//定数　
	static const int kFireInterval = 60;

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
};
