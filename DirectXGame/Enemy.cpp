#include "Enemy.h"
#include "TextureManager.h"
#include <assert.h>
#include "ImGuiManager.h"
#include "MathUtils.h"
#include "Player.h"

Enemy::~Enemy() { 
	delete state_;}

void Enemy::Initialize(Model* model) { 

	//初期位置
	const Vector3 initPosition = Vector3(20, 0, 100);

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
	worldTransform_.UpdateMatrix();
	//stateの初期値をnew
	state_ = new EnemyStateApproach();
	//EnemyStateにEnemyのポインタを渡す
	state_->SetEnemy(this);
	//EnemyStateの初期化
	state_->Initialize();
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

	// 弾のリストを毎フレーム更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}

	// 弾の死亡フラグが立っていたらリストから削除
	bullets_.remove_if([](auto& bullet) {
		if (bullet->GetIsDead()) {
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

	for (auto& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

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
	bullets_.push_back(std::move(newBullet));

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

}
