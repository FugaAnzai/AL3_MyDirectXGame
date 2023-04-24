#include "Enemy.h"
#include "TextureManager.h"
#include <assert.h>
#include "ImGuiManager.h"
#include "Vector3.h"

void (Enemy::*Enemy::phaseTable[])() = {&Enemy::Approach, &Enemy::Leave};

void Enemy::Initialize(Model* model) { 

	//初期位置
	const Vector3 initPosition = Vector3(0, 5, 100);

	//モデルがなければ中止
	assert(model);
	//モデル代入
	model_ = model;
	//ワールド変換データの初期化
	worldTransform_.Initialize();
	//初期位置代入
	worldTransform_.translation_ = initPosition;
	//画像ロード
	textureHandle_ = TextureManager::Load("eyes.jpg");
}

void Enemy::Update() {

	//関数テーブルでの状態管理
	(this->*phaseTable[static_cast<size_t>(phase_)])();

	//デバッグ用
	ImGui::Begin("Enemy");
	float inputTranslation[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z};
	ImGui::InputFloat3("translation", inputTranslation);
	ImGui::End();

	//座標変換
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Approach() {

	//接近速度
	const float kApproachSpeed = -0.2f;

	//ベクトル加算
	worldTransform_.translation_ += Vector3(0, 0, kApproachSpeed);

	//z座標0まで行ったら状態切り替え
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

}

void Enemy::Leave() {

	//離脱速度
	const float kLeaveSpeed = -0.2f;

	//ベクトル加算
	worldTransform_.translation_ += Vector3(kLeaveSpeed, 0, kLeaveSpeed);

}