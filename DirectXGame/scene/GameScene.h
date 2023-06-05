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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t texureHandle_ = 0;
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	ViewProjection viewprojection_;
	Player* player_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive = false;
	Enemy* enemy_ = nullptr;
	std::unique_ptr<CollisionManager> collisionManager;
	Skydome* skydome_ = nullptr;
	RailCamera* railcamera_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	

};

