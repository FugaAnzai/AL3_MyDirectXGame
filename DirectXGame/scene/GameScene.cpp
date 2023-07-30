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
	TextureManager::Load("2DReticle.png");
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
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	player_->Initialize(modelPlayer_,model_,texureHandle_);
	player_->SetParent(&railcamera_->GetWorldTransform());
	collisionManager = std::make_unique<CollisionManager>();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	modelBossBody_ = Model::CreateFromOBJ("bossBody", true);
	modelBossArm_ = Model::CreateFromOBJ("bossArm", true);
	boss_ = new Boss();
	boss_->SetPlayer(player_);
	boss_->SetGameScene(this);
	boss_->Initialize(modelBossBody_,modelBossArm_,model_);
	skydome_ = new Skydome();
	skydome_->Initalize(modelSkydome_);
	isEnd = false;
	
}

void GameScene::Update() {

	#ifdef _DEBUG
	//デバッグ時にのみデバッグカメラ切り替え
	if (input_->GetInstance()->TriggerKey(DIK_C)) {
		isDebugCameraActive = !isDebugCameraActive;
	}

#endif

	if (isDebugCameraActive) {
		debugCamera_->Update();
		viewprojection_.matView = debugCamera_->GetViewProjection().matView;
		viewprojection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewprojection_.TransferMatrix();
	} else {
		railcamera_->Update();
		viewprojection_.matView = railcamera_->GetViewProjection().matView;
		viewprojection_.matProjection = railcamera_->GetViewProjection().matProjection;
		viewprojection_.TransferMatrix();
	}

	player_->Update(viewprojection_);

	boss_->Update();

	if (boss_->GetHitPoint() <= 0 || player_->GetHitPoint() <= 0) {
		isEnd = true;
	}

	if (isEnd && input_->TriggerKey(DIK_R)) {
		
		Initialize();

	}

	/*enemies_.remove_if([](auto& enemy) {
		if (enemy->GetIsDead()) {
			return true;
		}
		return false;
	});*/

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

	if (!isEnd) {
		boss_->Draw(viewprojection_);
	}

	
	if (!isEnd) {
		player_->Draw(viewprojection_);
	}
	

	for (auto& bullet : enemyBullets_) {
		bullet->Draw(viewprojection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

	/*DrawSphere(Sphere{boss_->GetWorldPosition(), boss_->GetRadius()}, viewprojection_);
	DrawSphere(
	    Sphere{boss_->GetLeftArm()->GetWorldPosition(), boss_->GetLeftArm()->GetRadius()},
	    viewprojection_);
	DrawSphere(
	    Sphere{boss_->GetRightArm()->GetWorldPosition(), boss_->GetRightArm()->GetRadius()},
	    viewprojection_);*/

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollsions() {

	collisionManager->ClearList();

	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemyBullets_;

	collisionManager->AddCollider(player_);
	collisionManager->AddCollider(boss_);
	collisionManager->AddCollider(boss_->GetRightArm());
	collisionManager->AddCollider(boss_->GetLeftArm());

	for (auto& eBullet : enemyBullets) {
		collisionManager->AddCollider(eBullet.get());
	}

	for (auto& pBullet : playerBullets) {
		collisionManager->AddCollider(pBullet.get());  
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
