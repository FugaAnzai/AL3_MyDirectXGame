#include "Player.h"
#include "cassert"
#include "ImGuiManager.h"
#include "CollsionConfig.h"

Player::~Player() { 

}

void Player::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	input_->GetInstance();

	// 衝突属性と衝突マスクの設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(~kCollisionAttributePlayer);

}

void Player::Update() { 

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

	//行列の更新
	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {

	for (auto &bullet : bullets_) {
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
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾の生成
		std::unique_ptr<PlayerBullet> newBullet(new PlayerBullet());
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		//弾を登録
		bullets_.push_back(std::move(newBullet));
	}

}

void Player::OnCollision() {


}

