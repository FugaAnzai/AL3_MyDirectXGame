#include "Enemy.h"
#include "TextureManager.h"
#include <assert.h>
#include "ImGuiManager.h"
#include "MathUtils.h"
#include "Player.h"
#include "CollsionConfig.h"
#include "GameScene.h"

Enemy::~Enemy() { 
	delete state_;}

void Enemy::Initialize(Model* model) { 

	//モデルがなければ中止
	assert(model);
	//モデル代入
	model_ = model;
	//ワールド変換データの初期化
	worldTransform_.Initialize();
	//テクスチャロード
	textureHandle_ = TextureManager::Load("eyes.jpg");
	worldTransform_.UpdateMatrix();
	//stateの初期値をnew
	state_ = new EnemyStateApproach();
	//EnemyStateにEnemyのポインタを渡す
	state_->SetEnemy(this);
	//EnemyStateの初期化
	state_->Initialize();
	//衝突属性と衝突マスクの設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(~kCollisionAttributeEnemy);
}

void Enemy::Update() {

	//状態遷移
	state_->Update();

	for (auto& timedCall : timedCalls_) {
		timedCall->Update();
	}

	timedCalls_.remove_if([](auto& timedCall) {
		if (timedCall->IsFinished()) {
			return true;
		}
		return false;
	});

	ImGui::Begin("Enemy");
	float inputTranslation[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z};
	ImGui::InputFloat3("translation", inputTranslation);
	ImGui::End();

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) { 

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Move(const Vector3& move) { 
	worldTransform_.translation_ += move; 
}

void Enemy::Fire() {

	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 0.6f;
	Vector3 direction = player_->GetWorldPosition() - GetWorldPosition();
	Vector3 velocity = kBulletSpeed * Normalize(direction);

	// 弾の生成
	std::unique_ptr<EnemyBullet> newBullet(new EnemyBullet());
	newBullet->Initialize(model_, GetWorldPosition(), velocity, player_);

	// 弾を登録
	gamescene_->AddEnemyBullet(std::move(newBullet));

}

void Enemy::FireAndReload() {

	Fire();

	//時限発動

	std::function<void(void)> callback = std::bind(&Enemy::FireAndReload, this);
	std::unique_ptr<TimedCall> timedCall(new TimedCall(callback, fireTimer_));
	timedCalls_.push_back(std::move(timedCall));

}

void Enemy::FireReset() {

	timedCalls_.clear();

}

void Enemy::ChangeState(BaseEnemyState* newState) 
{ 
	//現在のstateをdelete
	delete state_;
	//遷移先の状態を代入
	state_ = newState;
	//敵クラスの実体を代入
	state_->SetEnemy(this);
	//初期化関数を呼び出し
	state_->Initialize();

}

void Enemy::OnCollision() {

	isDead_ = true;

}
