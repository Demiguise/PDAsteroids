#pragma once
#include "Colliders.h"
#include "Entity.h"
#include "Common.h"

class Entity;
struct CollisionData;
enum ColliderType;

struct RayCastHit
{
	RayCastHit()
		: entityHit(0), collisionPos(EnVector3::Zero()), normal(EnVector3::Zero()) {}
	RayCastHit(Entity* e, EnVector3 p, EnVector3 n)
		: entityHit(e), collisionPos(p), normal(n) {}
	Entity* entityHit;
	EnVector3 collisionPos;
	EnVector3 normal;
};

struct CollisionPair
{
	CollisionPair() {}
	CollisionPair(Entity* first, Entity* second)
		: a(first), b(second), data(0) {}
	~CollisionPair() { if (data != 0) { delete data; } }
	Entity* a;
	Entity* b;
	CollisionData* data;
};


class PhysicsManager
{
public:
	static PhysicsManager* GetInstance();
	~PhysicsManager();
	void Update(const float& dt);
	void CollisionUpdate(const float& dt);
	bool CastRay(EnVector3 pos, EnVector3 dir, float dist, RayCastHit& raycastOut);
	void RegisterEntity(Entity* entity, ColliderType rbType, float mass);
	void RemoveEntity(Entity* entity);
	float gravAcceleration;

private:
	PhysicsManager();
	void LoadColliders();
	static PhysicsManager* m_pInstance;
	void ResolveCollisions(std::vector<CollisionPair>& possibleCollisions,const float& dt);
	std::vector<CollisionPair> CoarseCollisionDetection(const std::deque<Entity*>& availableCollideables);
	void GenerateContacts(std::vector<CollisionPair>& coarseCollisions);

	std::deque<Entity*> sceneCollideables;
	std::vector<ModelData> colliderModels;
};

