#include "Enemy.h"
#include "TextureManager.h"
#include <assert.h>
#include "ImGuiManager.h"
#include "Vector3.h"

Enemy::~Enemy() { delete state_; }

void Enemy::Initialize(Model* model) { 

	//�����ʒu
	const Vector3 initPosition = Vector3(0, 5, 100);

	//���f�����Ȃ���Β��~
	assert(model);
	//���f�����
	model_ = model;
	//���[���h�ϊ��f�[�^�̏�����
	worldTransform_.Initialize();
	//�����ʒu�̑��
	worldTransform_.translation_ = initPosition;
	//�e�N�X�`�����[�h
	textureHandle_ = TextureManager::Load("eyes.jpg");
	//state�̏����l��new
	state_ = new EnemyStateApproach();
	//EnemyState��Enemy�̃|�C���^��n��
	state_->SetEnemy(this);
}

void Enemy::Update() {

	//��ԑJ��
	state_->Update();

	ImGui::Begin("Enemy");
	float inputTranslation[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z};
	ImGui::InputFloat3("translation", inputTranslation);
	ImGui::End();

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Move(const Vector3& move) { worldTransform_.translation_ += move; }

void Enemy::ChangeState(BaseEnemyState* newState) 
{ 
	delete state_;
	state_ = newState;
	state_->SetEnemy(this);

}