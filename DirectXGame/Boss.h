#pragma once
#include "Collider.h"
#include "Model.h"
#include "ViewProjection.h"
#include "BossArm.h"
#include "Player.h" 
#include "TimedCall.h"
#include "BossState.h"

class GameScene;

class Boss : public Collider {
public:
	void Initialize(Model* model, Model* armModel, Model* bubbleModel);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	void OnCollision() override;
	void Fire();
	void FireAndReload();
	void FireReset();
	void Barrage();
	void LeftPunch();
	void ChangeState(BaseBossState* newState);

	Vector3 GetWorldPosition() override {
		Vector3 result;
		result.x = worldTransform_.matWorld_.m[3][0];
		result.y = worldTransform_.matWorld_.m[3][1];
		result.z = worldTransform_.matWorld_.m[3][2];
		return result;
	}

	BossArm* GetRightArm() { return rightArm_; }
	BossArm* GetLeftArm() { return leftArm_; }
	Player* GetPlayer() { return player_; }

	int GetHitPoint() { return hitPoint; }

	void SetFireTimer(uint32_t fireTimer) { fireTimer_ = fireTimer; }
	void SetPlayer(Player* player) { player_ = player; }
	void SetGameScene(GameScene* gamescene) { gamescene_ = gamescene; }

	const int kFireInterval = 10;
	const float kRadius = 10;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデルデータ
	Model* model_;
	//　弾のモデルデータ
	Model* bubbleModel_;
	// テクスチャハンドル
	uint32_t textureHandle_;
	//state
	BaseBossState* state_ = nullptr;
	// 弾のタイマー
	uint32_t fireTimer_ = 0;
	// 時限発動のリスト
	std::list<std::unique_ptr<TimedCall>> timedCalls_;
	//右腕
	BossArm* rightArm_ = nullptr;
	//左腕
	BossArm* leftArm_ = nullptr;
	//プレイヤーポインタ
	Player* player_ = nullptr;
	//ゲームシーンのポインタ
	GameScene* gamescene_ = nullptr;
	int hitPoint = 100;

};
