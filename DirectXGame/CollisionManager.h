#pragma once
#include "Collider.h"
#include <list>
#include "Player.h"
#include "Enemy.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "CollsionConfig.h"

class CollisionManager {
public:

	void AddCollider(Collider* collider);
	void ClearList();
	void CheckAllCollsions();
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

private:
	
	std::list<Collider*> colliders_;

};
