#include "Player.h"
#include "cassert"
#include "ImGuiManager.h"

Player::~Player() { 

	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}

}

void Player::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	input_->GetInstance();

}

void Player::Update() { 

	Move();
	Rotate();
	Attack();

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();		
	}

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//デバッグ用座標表示
	ImGui::Begin("Player");
	float inputTranslation[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z};
	ImGui::InputFloat3("translation", inputTranslation);
	ImGui::End();

	//行列の更新
	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	model_->Draw(worldTransform_, viewProjection,textureHandle_);

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
		velocity = Vector3::TransformNormal(velocity, worldTransform_.matWorld_);

		//弾の生成
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		//弾を登録
		bullets_.push_back(newBullet);
	}

}

