#pragma once
#include <cstdint>
#include "Vector3.h"

class Collider {
public:
	
	virtual void OnCollision() = 0;

	virtual Vector3 GetWorldPosition() = 0;
	float GetRadius() { return radius_; }
	void SetRadius(float radius) { radius_ = radius; }

	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	uint32_t GetCollisionMask() { return collisionMask_; }
	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

private:
	float radius_ = 2;
	uint32_t collisionAttribute_ = 0xffffffff;
	uint32_t collisionMask_ = 0xffffffff;

};
