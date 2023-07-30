#define _USE_MATH_DEFINES
#include "Boss.h"
#include <cassert>
#include "ImGuiManager.h"
#include "CollsionConfig.h"
#include "DebugText.h"
#include "MathUtils.h"
#include "EnemyBullet.h"
#include "GameScene.h"
#include <numbers>
#include "PrimitiveDrawer.h"

void Boss::Initialize(Model* model,Model* armModel,Model* bubbleModel) {
	
	// モデルがなければ中止
	assert(model);
	// モデル代入
	model_ = model;
	bubbleModel_ = bubbleModel;
	// ワールド変換データの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3{0.0f, 0.0f, 300.0f};
	worldTransform_.rotation_ = Vector3{0.0f, 17.3f, 0.0f};
	worldTransform_.scale_ = Vector3{0.5f, 0.5f, 0.5f};
	worldTransform_.UpdateMatrix();

	leftArm_ = new BossArm();
	rightArm_ = new BossArm();

	leftArm_->Initialize(armModel, BossArm::ArmDirection::Left);
	rightArm_->Initialize(armModel, BossArm::ArmDirection::Right);
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(~kCollisionAttributeEnemy);
	SetRadius(kRadius);
	
	// stateの初期値をnew
	state_ = new LeftArmPunch();
	// BossStateにBossのポインタを渡す
	state_->SetBoss(this);
	// BossStateの初期化
	state_->Initialize();

}

void Boss::Update() {

	// 状態遷移
	state_->Update();

	for (auto& timedCall : timedCalls_) {
		timedCall->Update();
	}

	timedCalls_.remove_if([](auto& timedCall) {
		if (timedCall->IsFinished()) {
			return true;
		}
		return false;
	});

	/*ImGui::Begin("Boss");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.1f, -1000, 1000);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.1f, -100, 100);
	ImGui::Text("%d", hitPoint);
	ImGui::End();*/

	worldTransform_.UpdateMatrix();

}

void Boss::Draw(const ViewProjection& viewProjection) {
	
	Vector3 center = GetWorldPosition();
	float radius = GetRadius();

	Sphere sphere = {center,radius};

	model_->Draw(worldTransform_, viewProjection);
	leftArm_->Draw(viewProjection);
	rightArm_->Draw(viewProjection);

}

void Boss::OnCollision() {

	hitPoint--;
	DebugText::GetInstance()->ConsolePrintf("Hit\n");

	

}

void Boss::Fire() {
	
	assert(player_);

	const float kBulletSpeed = 8.0f;
	Vector3 direction = player_->GetWorldPosition() - GetWorldPosition();
	Vector3 velocity = kBulletSpeed * Normalize(direction);

	std::unique_ptr<EnemyBullet> newBullet(new EnemyBullet());
	newBullet->Initialize(bubbleModel_, GetWorldPosition(), velocity, player_);
	
	gamescene_->AddEnemyBullet(std::move(newBullet));

}

void Boss::FireAndReload() {

	Fire();

	// 時限発動

	std::function<void(void)> callback = std::bind(&Boss::FireAndReload, this);
	std::unique_ptr<TimedCall> timedCall(new TimedCall(callback, fireTimer_));
	timedCalls_.push_back(std::move(timedCall));
}

void Boss::FireReset() { timedCalls_.clear(); }

void Boss::Barrage() {

	const uint32_t kSubdivision = 12;
	const float kLonEvery = (360 / kSubdivision) * ((float)std::numbers::pi / 180);
	const float kLatEvery = (360 / kSubdivision) * ((float)std::numbers::pi / 180);

	/*const float theta = (float)std::numbers::pi / kSubdivision;
	const float phi = (2 * (float)std::numbers::pi) / kSubdivision;*/

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -(float)std::numbers::pi / 2.0f + kLatEvery * latIndex;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;
			Vector3 direction;
			direction = (Vector3{
			    std::cos(lat) * std::cos(lon), std::sin(lat), std::cos(lat) * std::sin(lon)});
			
			/*Matrix4x4 rotateYMat = MakeRotateYMatrix(-50 * ((float)std::numbers::pi / 180));

			direction = direction * rotateYMat;*/

			const float kBulletSpeed = 2.0f;

			Vector3 velocity = kBulletSpeed * Normalize(direction);

			std::unique_ptr<EnemyBullet> newBullet(new EnemyBullet());
			newBullet->Initialize(bubbleModel_, GetWorldPosition(), velocity, player_);

			gamescene_->AddEnemyBullet(std::move(newBullet));

		}
	}

}

void Boss::LeftPunch() {

	Vector3 targetPos = player_->GetWorldPosition();
	Vector3 fromPos = GetWorldPosition();

}

void Boss::ChangeState(BaseBossState* newState) {

	// 現在のstateをdelete
	delete state_;
	// 遷移先の状態を代入
	state_ = newState;
	// 敵クラスの実体を代入
	state_->SetBoss(this);
	// 初期化関数を呼び出し
	state_->Initialize();
	
}
