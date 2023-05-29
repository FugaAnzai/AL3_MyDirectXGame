#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Skydome {
public:
	void Initalize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_;

};
