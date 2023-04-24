#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "EnemyState.h"


class Enemy {
public:
	~Enemy();
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Move(const Vector3& move);
	void ChangeState(BaseEnemyState* newState);

	//getter
	WorldTransform GetWorldTransfrom() { return worldTransform_; }

	enum class Phase {
		Approach,
		Leave,
	};

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデルデータ
	Model* model_;
	//テクスチャハンドル
	uint32_t textureHandle_;
	//状態遷移
	Phase phase_ = Phase::Approach;
	//状態遷移
	BaseEnemyState* state_;
};
