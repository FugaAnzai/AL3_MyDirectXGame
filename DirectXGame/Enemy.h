#pragma once
#include "WorldTransform.h"
#include "Model.h"



class Enemy {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Approach();
	void Leave();

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
	//メンバ関数ポインタ
	static void (Enemy::*phaseTable[])();

};
