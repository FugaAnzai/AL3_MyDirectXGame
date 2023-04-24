#pragma once

class Enemy;

class BaseEnemyState {
public:
	virtual ~BaseEnemyState(){};
	virtual void Update() = 0;
	void SetEnemy(Enemy* enemy);

protected:
	Enemy* enemy_;
};

class EnemyStateApproach : public BaseEnemyState {
public:
	void Update() override;
};

class EnemyStateLeave : public BaseEnemyState {
public:
	void Update() override;
};


