#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <ImGuiManager.h>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete player_;
	delete debugCamera_;
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
	LoadEnemyPopData();
	player_ = new Player();
	player_->Initialize(model_,texureHandle_);
	player_->SetParent(&railcamera_->GetWorldTransform());
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

	UpdateEnemyPopCommands();

	player_->Update();

	for (auto& enemy : enemies_) {
		enemy->Update();
	}

	enemies_.remove_if([](auto& enemy) {
		if (enemy->GetIsDead()) {
			return true;
		}
		return false;
	});

	// 弾のリストを毎フレーム更新
	for (auto& bullet : enemyBullets_) {
		bullet->Update();
	}

	// 弾の死亡フラグが立っていたらリストから削除
	enemyBullets_.remove_if([](auto& bullet) {
		if (bullet->GetIsDead()) {
			return true;
		}
		return false;
	});

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

	for (auto& enemy : enemies_) {
		enemy->Draw(viewprojection_);
	}

	for (auto& bullet : enemyBullets_) {
		bullet->Draw(viewprojection_);
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
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemyBullets_;
	const std::list<std::unique_ptr<Enemy>>& enemies = enemies_;

	collisionManager->AddCollider(player_);

	for (auto& eBullet : enemyBullets) {
		collisionManager->AddCollider(eBullet.get());
	}

	for (auto& pBullet : playerBullets) {
		collisionManager->AddCollider(pBullet.get());  
	}

	for (auto& enemy : enemies) {
		collisionManager->AddCollider(enemy.get());
	}

	collisionManager->CheckAllCollsions();

}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet) {

	enemyBullets_.push_back(std::move(enemyBullet));

}

void GameScene::AddEnemy(const Vector3& position) {
	
	std::unique_ptr<Enemy> enemy(new Enemy());
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
	enemy->Initialize(model_);
	enemy->SetTranslate(position);

	enemies_.push_back(std::move(enemy));

}

void GameScene::LoadEnemyPopData() {

	std::ifstream file;
	file.open( "Resources/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();

}

void GameScene::UpdateEnemyPopCommands() {

	if (isWaitEnemyPop) {
		waitEnemyPopTimer--;

		if (waitEnemyPopTimer <= 0) {
			isWaitEnemyPop = false;
		}
		return;
	}

	std::string line;

	while (std::getline(enemyPopCommands, line)) {
		
		std::istringstream line_stream(line);

		std::string word;
		
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0) {
			
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			std::getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			AddEnemy(Vector3{x, y, z});

		} else if (word.find("WAIT") == 0) {
			
			std::getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWaitEnemyPop = true;
			waitEnemyPopTimer = waitTime;

		}

	}

}
