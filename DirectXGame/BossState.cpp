#include "BossState.h"
#include "Boss.h"
#include "MathUtils.h"

void BubbleToPlayer::Initialize() {

	boss_->SetFireTimer(boss_->kFireInterval);
	boss_->FireAndReload();

}

void BubbleToPlayer::Update() {

	frame++;

	if (frame == 360) {
		
		boss_->FireReset();
		boss_->ChangeState(new BubbleBarrage());

	}
}

void BubbleBarrage::Initialize() {

	boss_->Barrage();
	boss_->ChangeState(new LeftArmPunch());

}

void BubbleBarrage::Update() {}

void LeftArmPunch::Initialize() {

	target = boss_->GetPlayer()->GetWorldPosition();
	from = boss_->GetLeftArm()->GetWorldPosition();
	direction = target - from;
	direction = Normalize(direction);

}

void LeftArmPunch::Update() {

	Vector3 velocity = (float)moveSpeed * direction;

	if (boss_->GetLeftArm()->GetWorldPosition().z <= 40 && !isReturn) {
	
		isReturn = true;
		direction = from - target;
		direction = Normalize(direction);
		velocity = (float)moveSpeed * direction;
	}

	boss_->GetLeftArm()->Update(velocity);

	if (isReturn) {

		if (boss_->GetLeftArm()->GetWorldPosition().z >= 500) {
		
			boss_->ChangeState(new BubbleToPlayer());

		}

	}

	

}
