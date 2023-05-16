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

	Vector3 posA, posB;

	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

	//自キャラと敵の弾の当たり判定
	posA = player_->GetWorldPostion();

	for (auto& bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPostion();

		float lengthAtoB = Length(posB - posA);

		if (lengthAtoB < bullet->kRadius + player_->kRadius) {
			player_->OnCollision();
			bullet->OnCollision();
		}

	}

	posA = enemy_->GetWorldPostion();

	for (auto& bullet : playerBullets) {
		// 自弾の座標
		posB = bullet->GetWorldPostion();

		float lengthAtoB = Length(posB - posA);

		if (lengthAtoB < bullet->kRadius + enemy_->kRadius) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}

	for (auto& pBullet : playerBullets) {
		// 自弾の座標
		posA = pBullet->GetWorldPostion();

		for (auto& eBullet : enemyBullets) {
			// 敵弾の座標
			posB = eBullet->GetWorldPostion();

			float lengthAtoB = Length(posB - posA);

			if (lengthAtoB < pBullet->kRadius + eBullet->kRadius) {
				pBullet->OnCollision();
				eBullet->OnCollision();
			}

		}
	}

}
