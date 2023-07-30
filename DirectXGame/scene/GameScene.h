#pragma once
#include "Enemy.h"
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <sstream>
#include "Boss.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void CheckAllCollsions();
	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet);
	void AddEnemy(const Vector3& position);
	void LoadEnemyPopData();
	void UpdateEnemyPopCommands();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t texureHandle_ = 0;
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelBossBody_ = nullptr;
	Model* modelBossArm_ = nullptr;
	Model* modelPlayer_ = nullptr;
	ViewProjection viewprojection_;
	Player* player_ = nullptr;
	Boss* boss_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive = false;
	std::list<std::unique_ptr<Enemy>> enemies_;
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;
	std::unique_ptr<CollisionManager> collisionManager;
	Skydome* skydome_ = nullptr;
	RailCamera* railcamera_ = nullptr;
	std::stringstream enemyPopCommands;
	bool isWaitEnemyPop = false;
	int32_t waitEnemyPopTimer = 0;
	bool isEnd = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	

};

