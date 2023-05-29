#include "Skydome.h"
#include <cassert>

void Skydome::Initalize(Model* model) {
	assert(model);

	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = Vector3{100, 100, 100};
	worldTransform_.UpdateMatrix();
}

void Skydome::Update() {
	
	
}

void Skydome::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection);

}
