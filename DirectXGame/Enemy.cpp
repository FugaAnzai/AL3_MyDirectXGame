#include "Enemy.h"
#include "TextureManager.h"
#include <assert.h>
#include "ImGuiManager.h"
#include "Vector3.h"

Enemy::~Enemy() { delete state_; }

void Enemy::Initialize(Model* model) { 

	//初期位置
	const Vector3 initPosition = Vector3(0, 5, 100);

	//モデルがなければ中止
	assert(model);
	//モデル代入
	model_ = model;
	//ワールド変換データの初期化
	worldTransform_.Initialize();
	//初期位置の代入
	worldTransform_.translation_ = initPosition;
	//テクスチャロード
	textureHandle_ = TextureManager::Load("eyes.jpg");
	//stateの初期値をnew
	state_ = new EnemyStateApproach();
	//EnemyStateにEnemyのポインタを渡す
	state_->SetEnemy(this);
}

void Enemy::Update() {

	//状態遷移
	state_->Update();

	ImGui::Begin("Enemy");
	float inputTranslation[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z};
	ImGui::InputFloat3("translation", inputTranslation);
	ImGui::End();

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Move(const Vector3& move) { worldTransform_.translation_ += move; }

void Enemy::ChangeState(BaseEnemyState* newState) 
{ 
	delete state_;
	state_ = newState;
	state_->SetEnemy(this);

}