#pragma once
#include "Common.h"
#include "IEventManager.h"

class Entity;
struct CollisionData;
enum ColliderType;
class PhysicsManager;

class PhysicsEventReceiver : public EventReceiver
{
public:
	PhysicsEventReceiver(PhysicsManager* parentPhysics);
	PhysicsEventReceiver();
	~PhysicsEventReceiver();
	bool Receive(Event::IEvent* e);

private:
	PhysicsManager* parent;
};

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
	PhysicsManager();
	~PhysicsManager();
	void Update(const float& dt);
	bool OnEvent(Event::IEvent* e);
	void CollisionUpdate(const float& dt);
	bool CastRay(EnVector3 pos, EnVector3 dir, float dist, RayCastHit& raycastOut);
	void RegisterEntity(Entity* entity, ColliderType rbType, float mass, float scale);
	void RemoveEntity(Entity* entity);
	float gravAcceleration;

	PhysicsEventReceiver* receiver;

private:
	ModelData ScaleRBModel(ModelData rbModel, const float& scale);
	void LoadColliders();
	void ResolveCollisions(std::vector<CollisionPair>& possibleCollisions,const float& dt);
	std::vector<CollisionPair> CoarseCollisionDetection(const std::deque<Entity*>& availableCollideables);
	void GenerateContacts(std::vector<CollisionPair>& coarseCollisions);

	std::deque<Entity*> sceneCollideables;
	std::vector<ModelData> colliderModels;
};

