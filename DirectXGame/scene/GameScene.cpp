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

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	texureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	viewprojection_.Initialize();
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth,WinApp::kWindowHeight);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	player_ = new Player();
	player_->Initialize(model_,texureHandle_);
	enemy_ = new Enemy();
	enemy_->SetPlayer(player_);
	enemy_->Initialize(model_);

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
		viewprojection_.UpdateMatrix();
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

	player_->Draw(debugCamera_->GetViewProjection());

	if (enemy_ != nullptr) {
		enemy_->Draw(debugCamera_->GetViewProjection());
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

	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

	std::list<Collider*> colliders_;
	colliders_.push_back(player_);
	colliders_.push_back(enemy_);

	for (auto& eBullet : enemyBullets) {
		colliders_.push_back(eBullet.get());
	}

	for (auto& pBullet : playerBullets) {
		colliders_.push_back(pBullet.get());
	}

	std::list<Collider*>::iterator itrA = colliders_.begin();

	for (; itrA != colliders_.end(); ++itrA) {
		Collider* objectA = *itrA;

		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* objectB = *itrB;

			if (!(objectA->GetCollisionAttribute() & objectB->GetCollisionMask())||
			    !(objectB->GetCollisionAttribute() & objectA->GetCollisionMask()))
			{
				continue;
			}

			CheckCollisionPair(objectA, objectB);

		}

	}

}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	
	Vector3 objectA = colliderA->GetWorldPosition();
	Vector3 objectB = colliderB->GetWorldPosition();
	float length = Length(Subtract(objectB, objectA));

	if (length < colliderA->GetRadius() + colliderB->GetRadius()) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}

}
