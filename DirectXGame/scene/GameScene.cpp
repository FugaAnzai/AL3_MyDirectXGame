#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <ImGuiManager.h>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
	delete modelSkydome_;
	delete skydome_;
	delete railcamera_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	texureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	viewprojection_.Initialize();
	viewprojection_.farZ = 100;
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth,WinApp::kWindowHeight);
	debugCamera_->SetFarZ(viewprojection_.farZ);
	railcamera_ = new RailCamera();
	railcamera_->Initialize(Vector3{0, 0, 0}, Vector3{0, 0, 0});
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	player_ = new Player();
	player_->Initialize(model_,texureHandle_);
	player_->SetParent(&railcamera_->GetWorldTransform());
	enemy_ = new Enemy();
	enemy_->SetPlayer(player_);
	enemy_->Initialize(model_);
	collisionManager = std::make_unique<CollisionManager>();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initalize(modelSkydome_);
	
}

void GameScene::Update() {
	
	#ifdef _DEBUG
	//デバッグ時にのみデバッグカメラ切り替え
	if (input_->GetInstance()->TriggerKey(DIK_C)) {
		isDebugCameraActive = !isDebugCameraActive;
	}

#endif

	player_->Update();

	

	if (enemy_ != nullptr) {
		enemy_->Update();
	}

	if (isDebugCameraActive) {
		debugCamera_->Update();
		viewprojection_.matView = debugCamera_->GetViewProjection().matView;
		viewprojection_.matProjection = debugCamera_->GetViewProjection().matProjection;
	} else {
		railcamera_->Update();
		viewprojection_.matView = railcamera_->GetViewProjection().matView;
		viewprojection_.matProjection = railcamera_->GetViewProjection().matProjection;
		viewprojection_.TransferMatrix();
	}
	

	CheckAllCollsions();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	skydome_->Draw(viewprojection_);

	player_->Draw(viewprojection_);

	if (enemy_ != nullptr) {
		enemy_->Draw(viewprojection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollsions() {

	collisionManager->ClearList();

	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

	collisionManager->AddCollider(player_);
	collisionManager->AddCollider(enemy_);

	for (auto& eBullet : enemyBullets) {
		collisionManager->AddCollider(eBullet.get());
	}

	for (auto& pBullet : playerBullets) {
		collisionManager->AddCollider(pBullet.get());  
	}

	collisionManager->CheckAllCollsions();

}
