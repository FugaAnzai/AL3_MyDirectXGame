#include "Player.h"
#include "cassert"
#include "ImGuiManager.h"
#include "CollsionConfig.h"
#include "ViewProjection.h"

Player::~Player() { 
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model,Model* bModel, uint32_t textureHandle) { 
	assert(model);

	model_ = model;
	bmodel_ = bModel;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform3DReticle_.Initialize();
	worldTransform3DReticle_.parent_ = &worldTransform_;
	worldTransform_.translation_ = Vector3{0, 0, 30};
	worldTransform_.scale_ = Vector3{0.3f, 0.3f, 0.3f};
	uint32_t textureReticle = TextureManager::Load("2DReticle.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, Vector2{640,360}, Vector4{1,1,1,1}, Vector2{0.5f, 0.5f});
	input_->GetInstance();
	worldTransform_.UpdateMatrix();
	// 衝突属性と衝突マスクの設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(~kCollisionAttributePlayer);

}

void Player::Update(const ViewProjection& viewProjection) { 

	Move();
	Rotate();
	Attack();
	ReticleMove(viewProjection);

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
	/*ImGui::Begin("Player");
	float inputTranslation[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z};
	ImGui::InputFloat3("translation", inputTranslation);
	ImGui::End();*/

	// 行列の更新
	worldTransform_.UpdateMatrix();

}

void Player::Draw(const ViewProjection& viewProjection) {

	for (auto &bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	model_->Draw(worldTransform_, viewProjection,textureHandle_);
	//model_->Draw(worldTransform3DReticle_, viewProjection);
}

void Player::DrawUI() {

	sprite2DReticle_->Draw();

}

void Player::Move() {

	XINPUT_STATE joystate;

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

	if (Input::GetInstance()->GetJoystickState(0, joystate)) {
		move.x += (float)joystate.Gamepad.sThumbLX / SHRT_MAX * kPlayerSpeed;
		move.y += (float)joystate.Gamepad.sThumbLY / SHRT_MAX * kPlayerSpeed;
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

	XINPUT_STATE joystate;

	if (!Input::GetInstance()->GetJoystickState(0, joystate)) {
		return;
	}

	if (input_->GetInstance()->TriggerKey(DIK_SPACE) || (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {

		if (!isShot) {
			
			isShot = true;

			// 弾の速度
			const float kBulletSpeed = 5.0f;
			Vector3 velocity(0, 0, kBulletSpeed);
			Vector3 reticleWorldPosition = {

			    reticleWorldPosition.x = worldTransform3DReticle_.matWorld_.m[3][0],
			    reticleWorldPosition.y = worldTransform3DReticle_.matWorld_.m[3][1],
			    reticleWorldPosition.z = worldTransform3DReticle_.matWorld_.m[3][2]

			};
			velocity = reticleWorldPosition - GetWorldPosition();
			velocity = kBulletSpeed * Normalize(velocity);

			// 弾の生成
			std::unique_ptr<PlayerBullet> newBullet(new PlayerBullet());
			newBullet->Initialize(bmodel_, GetWorldPosition(), velocity);

			// 弾を登録
			bullets_.push_back(std::move(newBullet));

		}
		
	}

	if (isShot) {
		
		shotCount++;

		if (shotCount > 8) {
			isShot = false;
			shotCount = 0;
		}

	}

}

void Player::ReticleMove(const ViewProjection& viewProjection) {

	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	sprite2DReticle_->SetPosition(Vector2{(float)spritePosition.x, (float)spritePosition.y});

	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0.0f, 1.0f);
	Matrix4x4 matVPV =
	    Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	Vector3 screenPlayerPos = Transform(GetWorldPosition(), matVPV);

	sprite2DReticle_->SetPosition( Vector2{(float)screenPlayerPos.x, (float)screenPlayerPos.y + 100});

	spritePosition = sprite2DReticle_->GetPosition();

	Vector3 posNear = Vector3{(float)spritePosition.x, (float)spritePosition.y, 0};
	Vector3 posFar = Vector3{(float)spritePosition.x, (float)spritePosition.y, 1};

	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	Vector3 reticleDirection = posFar - posNear;
	reticleDirection = Normalize(reticleDirection);
	const float kDistanceTestObject = 100.0f;
	worldTransform3DReticle_.translation_ = posNear + (kDistanceTestObject * reticleDirection);

	worldTransform3DReticle_.UpdateMatrix();

}



void Player::OnCollision() {


}

