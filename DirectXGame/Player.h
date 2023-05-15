#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Vector3.h"
#include "PlayerBullet.h"
#include <list>
#include "MathUtils.h"

class Player {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model,uint32_t textureHandle);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);
	/// <summary>
	/// 移動
	/// </summary>
	void Move();
	/// <summary>
	/// 旋回
	/// </summary>
	void Rotate();
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	//getter
	Vector3 GetWorldPostion() { 
		Vector3 result;
		result.x = worldTransform_.matWorld_.m[3][0];
		result.y = worldTransform_.matWorld_.m[3][1];
		result.z = worldTransform_.matWorld_.m[3][2];
		return result;
	}

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//入力
	Input* input_ = nullptr;
	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};


