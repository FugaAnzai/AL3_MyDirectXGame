#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Enemy {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void Move();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデルデータ
	Model* model_;
	//テクスチャハンドル
	uint32_t textureHandle_;

};
