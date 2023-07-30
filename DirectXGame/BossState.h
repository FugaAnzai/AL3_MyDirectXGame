#pragma once
#include "Vector3.h"

class Boss;

class BaseBossState {
public:
	// 仮想デストラクタ
	virtual ~BaseBossState(){};
	// 純粋仮想Update
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	// ポインタ代入
	void SetBoss(Boss* boss) { boss_ = boss; }

protected:
	// Enemyポインタ
	Boss* boss_ = nullptr;
};

class BubbleToPlayer : public BaseBossState {
private:
	int frame = 0;

public:
	void Initialize() override;
	void Update() override;
};

class BubbleBarrage : public BaseBossState {
public:
	void Initialize() override;
	void Update() override;
};

class LeftArmPunch : public BaseBossState {
protected:
	Vector3 target;
	Vector3 from;
	Vector3 direction;
	bool isReturn = false;

	const int moveSpeed = 2;

public:
	void Initialize() override;
	void Update() override;
};
