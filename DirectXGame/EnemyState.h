#pragma once

class Enemy;

class BaseEnemyState {
public:
	//仮想デストラクタ
	virtual ~BaseEnemyState(){};
	//純粋仮想Update
	virtual void Update() = 0;
	//ポインタ代入
	void SetEnemy(Enemy* enemy);

protected:
	//Enemyポインタ
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


