#pragma once
#include "Common.h"
#include "IEventManager.h"

class RigidBody;
class IEvent;
class Entity;

class EntityEventReceiver : public EventReceiver
{
public:
	EntityEventReceiver(Entity* parentEntity);
	EntityEventReceiver();
	~EntityEventReceiver();
	bool Receive(Event::IEvent* e);

private:
	Entity* parent;
};

class Entity
{
public:
	Entity(EnVector3 initPos, EnVector3 initRot);
	Entity(EnVector3 initPos);
	Entity();
	~Entity();
	virtual void Update();
	virtual bool OnEvent(Event::IEvent* e);
	virtual void OnDeath();
	void AddForce(EnVector3 direction, float power);
	bool TestAABBIntersection(BoundingBox& incomingAABB);
	EnVector3 GetLocalAxis(const int& index);
	UINT GetUID();
	virtual void SetRigidBody(RigidBody* newRB);

	std::string name;
	EnVector3 position;
	EnVector3 rotation;
	EnMatrix4x4 localToWorld;
	Quaternion quaternion;
	EnVector3 forceAccum;
	EnVector3 velocity;
	RigidBody* rigidBody;
	EntityEventReceiver* receiver;
	BoundingBox AABB;
	UINT uID;

protected:
	void AddListeners();
	void RemoveListener(std::string eventType);
	void UpdateLocalToWorldMatrix();
	void UpdateQuaternion();
	void Init();
};
