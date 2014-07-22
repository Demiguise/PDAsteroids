#pragma once
#include "renderableobject.h"
#include "Timer.h"

class IEvent;

class Projectile :
	public CRenderableObject
{
public:
	Projectile(ModelData newMesh, EnVector3 initPos, EnVector3 initRot);
	Projectile(ModelData newMesh, EnVector3 initPos);
	Projectile(ModelData newMesh);
	~Projectile();

	void Update(float dt);
	bool OnEvent(Event::IEvent* e);
	void OnDeath();
	void OnActivated(EnVector3 position, EnVector3 direction);
	void SetRigidBody(RigidBody* newRB);
	bool active;

private:
	void Init();
	void AddListeners();
	void RemoveListener(std::string eventType);
	Timer* lifeTimer;
	float lifeSpan;
};

