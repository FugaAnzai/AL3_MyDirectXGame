#include "CollisionManager.h"

void CollisionManager::AddCollider(Collider* collider) {
	
	colliders_.push_back(collider);

}

void CollisionManager::ClearList() {

	colliders_.clear();

}

void CollisionManager::CheckAllCollsions() {

	std::list<Collider*>::iterator itrA = colliders_.begin();

	for (; itrA != colliders_.end(); ++itrA) {
		Collider* objectA = *itrA;

		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* objectB = *itrB;

			if (!(objectA->GetCollisionAttribute() & objectB->GetCollisionMask()) ||
			    !(objectB->GetCollisionAttribute() & objectA->GetCollisionMask())) {
				continue;
			}

			CheckCollisionPair(objectA, objectB);
		}
	}

}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	Vector3 objectA = colliderA->GetWorldPosition();
	Vector3 objectB = colliderB->GetWorldPosition();
	float length = Length(Subtract(objectB, objectA));

	if (length < colliderA->GetRadius() + colliderB->GetRadius()) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}

}
