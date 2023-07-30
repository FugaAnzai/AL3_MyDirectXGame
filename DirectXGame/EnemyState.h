#pragma once

class Enemy;

class BaseEnemyState {
public:
	//仮想デストラクタ
	virtual ~BaseEnemyState(){};
	//純粋仮想Update
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	//ポインタ代入
	void SetEnemy(Enemy* enemy);

protected:
	//Enemyポインタ
	Enemy* enemy_ = nullptr;
};

class EnemyStateApproach : public BaseEnemyState {
public:
	void Initialize() override;
	void Update() override;
};

class EnemyStateLeave : public BaseEnemyState {
public:
	void Initialize() override;
	void Update() override;
};


