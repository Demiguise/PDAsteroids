#pragma once
#include "Common.h"
#include "Colliders.h"
#include "IEventManager.h"
#include "GameLog.h"
#include "SceneManager.h"
#include "PhysicsManager.h"

class IEventManager;
class RigidBody;
class SceneManager;
class PhysicsManager;

class Entity
{
public:
	Entity(EnVector3 initPos, EnVector3 initRot);
	Entity(EnVector3 initPos);
	Entity();
	~Entity();
	virtual void Update();
	virtual bool OnEvent(Event::IEvent* e);
	void AddForce(EnVector3 direction, float power);
	bool TestAABBIntersection(BoundingBox& incomingAABB);
	EnVector3 GetLocalAxis(const int& index);
	UINT GetUID();

	std::string name;
	EnVector3 position;
	EnVector3 rotation;
	EnMatrix4x4 localToWorld;
	Quaternion quaternion;
	EnVector3 forceAccum;
	EnVector3 velocity;
	RigidBody* rigidBody;
	BoundingBox AABB;

protected:
	void AddListeners();
	void RemoveListener(std::string eventType);
	void UpdateLocalToWorldMatrix();
	void UpdateQuaternion();
	void Init();
	
private:
	UINT uID;
};
