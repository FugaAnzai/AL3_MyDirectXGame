#include "Player.h"
#include "cassert"
#include "ImGuiManager.h"
#include "CollsionConfig.h"

Player::~Player() { 
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform3DReticle_.Initialize();
	worldTransform_.translation_ = Vector3{0, 0, 30};
	uint32_t textureReticle = TextureManager::Load("2DReticle.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, Vector2{}, Vector4{1,1,1,1}, Vector2{0.5f, 0.5f});
	input_->GetInstance();

	// 衝突属性と衝突マスクの設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(~kCollisionAttributePlayer);

}

void Player::Update(const ViewProjection& viewProjection) { 

	Move();
	Rotate();
	Attack();

	//弾のリストを毎フレーム更新
	for (auto &bullet : bullets_) {
		bullet->Update();		
	}

	//弾の死亡フラグが立っていたらリストから削除
	bullets_.remove_if([](auto &bullet) {
		if (bullet->GetIsDead()) {
			return true;
		}
		return false;
	});

	//デバッグ用座標表示
	ImGui::Begin("Player");
	float inputTranslation[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z};
	ImGui::InputFloat3("translation", inputTranslation);
	ImGui::End();

	const float kDistancePlayerTo3DReticle = 50.0f;
	Vector3 offset = {0, 0, 1.0f};
	offset = TransformNormal(offset,worldTransform_.matWorld_);
	offset = kDistancePlayerTo3DReticle * Normalize(offset);
	worldTransform3DReticle_.translation_ = GetWorldPosition() + offset;
	worldTransform3DReticle_.UpdateMatrix();

	Vector3 reticleWorldPosition = {

	    worldTransform3DReticle_.matWorld_.m[3][0],
	    worldTransform3DReticle_.matWorld_.m[3][1],
	    worldTransform3DReticle_.matWorld_.m[3][2]

	};

	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matViewProjectionViewport = Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));

	reticleWorldPosition = Transform(reticleWorldPosition, matViewProjectionViewport);

	sprite2DReticle_->SetPosition(Vector2{reticleWorldPosition.x, reticleWorldPosition.y});

	// 行列の更新
	worldTransform_.UpdateMatrix();

}

void Player::Draw(const ViewProjection& viewProjection) {

	for (auto &bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	model_->Draw(worldTransform_, viewProjection,textureHandle_);
	model_->Draw(worldTransform3DReticle_, viewProjection);
}

void Player::DrawUI() {

	sprite2DReticle_->Draw();

}

void Player::Move() {
	Vector3 move = Vector3(0, 0, 0);

	const float kPlayerSpeed = 0.2f;
	const float kMoveLimitX = 32;
	const float kMoveLimitY = 18;

	//入力方向に移動
	if (input_->GetInstance()->PushKey(DIK_LEFT)) {
		move.x -= kPlayerSpeed;
	}

	if (input_->GetInstance()->PushKey(DIK_RIGHT)) {
		move.x += kPlayerSpeed;
	}

	if (input_->GetInstance()->PushKey(DIK_UP)) {
		move.y += kPlayerSpeed;
	}

	if (input_->GetInstance()->PushKey(DIK_DOWN)) {
		move.y -= kPlayerSpeed;
	}

	//移動量加算
	worldTransform_.translation_ += move;

	//画面外押し出し
	worldTransform_.translation_.x =
	    std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y =
	    std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);
}

void Player::Rotate() {

	const float kRotateSpeed = 0.02f;

	//旋回処理
	if (input_->GetInstance()->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotateSpeed; 
	}

	if (input_->GetInstance()->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotateSpeed;
	}

}

void Player::Attack() {

	if (input_->GetInstance()->TriggerKey(DIK_SPACE)) {

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		Vector3 reticleWorldPosition = {

			reticleWorldPosition.x = worldTransform3DReticle_.matWorld_.m[3][0],
		    reticleWorldPosition.y = worldTransform3DReticle_.matWorld_.m[3][1],
		    reticleWorldPosition.z = worldTransform3DReticle_.matWorld_.m[3][2]

		};
		velocity = reticleWorldPosition - GetWorldPosition();
		velocity = kBulletSpeed * Normalize(velocity);

		//弾の生成
		std::unique_ptr<PlayerBullet> newBullet(new PlayerBullet());
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		//弾を登録
		bullets_.push_back(std::move(newBullet));
	}

}

void Player::OnCollision() {


}

