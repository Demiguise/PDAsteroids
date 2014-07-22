#pragma once
#include "renderableobject.h"

class IEvent;

enum AsteroidSize
{
	Large = 2,
	Medium = 1,
	Small = 0
};

class Asteroid :
	public CRenderableObject
{
public:
	Asteroid(ModelData newMesh, EnVector3 initPos, EnVector3 initRot);
	Asteroid(ModelData newMesh, EnVector3 initPos);
	Asteroid(ModelData newMesh);
	~Asteroid();

	void Update(float dt);
	bool OnEvent(Event::IEvent* e);
	void OnActivated(EnVector3 initPosition, EnVector3 initDirection, AsteroidSize initSize);
	void SetRigidBody(RigidBody* newRB);
	void OnDeath();
	bool active;
	AsteroidSize size;

private:
	void Init();
	void AddListeners();
	void RemoveListener(std::string eventType);
	ModelData ScaleRBModel(ModelData rbModel, const float& scale);
};

