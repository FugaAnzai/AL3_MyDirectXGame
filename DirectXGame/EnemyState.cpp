#include "EnemyState.h"
#include "Enemy.h"

void BaseEnemyState::SetEnemy(Enemy* enemy) { 
	enemy_ = enemy;
}

void EnemyStateApproach::Update() {
	
	const float kApproachSpeed = -0.2f;
	Vector3 move = Vector3(0, 0, kApproachSpeed);
	enemy_->Move(move);

	if (enemy_->GetWorldTransfrom().translation_.z < 0.0f) {
		enemy_->ChangeState(new EnemyStateLeave());
	}

}

void EnemyStateLeave::Update() {
	
	const float kLeaveSpeed = -0.2f;
	Vector3 move = Vector3(kLeaveSpeed, 0, kLeaveSpeed);
	enemy_->Move(move);

}
