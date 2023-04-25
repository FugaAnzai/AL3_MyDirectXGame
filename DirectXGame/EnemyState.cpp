#include "EnemyState.h"
#include "Enemy.h"

void BaseEnemyState::SetEnemy(Enemy* enemy) { 
	//Enemyポインタの代入
	enemy_ = enemy;
}

void EnemyStateApproach::Initialize() {

	//カウントダウン初期化
	enemy_->SetFireTimer(enemy_->kFireInterval);

}

void EnemyStateApproach::Update() {
	
	//接近速度
	const float kApproachSpeed = -0.2f;
	//移動ベクトル
	Vector3 move = Vector3(0, 0, kApproachSpeed);
	//移動処理
	enemy_->Move(move);
	//発射タイマーカウントダウン
	uint32_t timer = enemy_->GetFireTimer();
	timer--;
	enemy_->SetFireTimer(timer);
	if (enemy_->GetFireTimer() == 0) {
		//弾発射
		enemy_->Fire();
		//カウントダウン初期化
		enemy_->SetFireTimer(enemy_->kFireInterval);
	}


	//z座標が0までいったらstateをLeaveに
	if (enemy_->GetWorldTransfrom().translation_.z < 0.0f) {
		enemy_->ChangeState(new EnemyStateLeave());
	}

}

void EnemyStateLeave::Initialize() {
}

void EnemyStateLeave::Update() {
	
	//接近速度
	const float kLeaveSpeed = -0.2f;
	//移動ベクトル
	Vector3 move = Vector3(kLeaveSpeed, 0, kLeaveSpeed);
	//移動処理
	enemy_->Move(move);

}
