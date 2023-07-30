#pragma once
#include "Collider.h"
#include "Model.h"
#include "ViewProjection.h"

class BossArm : public Collider {
public:

	enum ArmDirection {
		Left,
		Right,
	};

	void Initialize(Model* model,ArmDirection armDirection);
	void Update(Vector3 velocity);
	void Draw(const ViewProjection& viewProjection);
	void OnCollision() override;

	Vector3 GetWorldPosition() override {
		Vector3 result;
		result.x = worldTransform_.matWorld_.m[3][0];
		result.y = worldTransform_.matWorld_.m[3][1];
		result.z = worldTransform_.matWorld_.m[3][2];
		return result;
	}

	WorldTransform GetWorldTransform() { return worldTransform_;}

	const float kRadius = 20;

	private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデルデータ
	Model* model_;
	// テクスチャハンドル
	uint32_t textureHandle_;
	//向き
	ArmDirection armDirection_;
	//攻撃中か
	bool isAttack;
	//プレイヤーの座標保存
	Vector3 playerTmp;

};
