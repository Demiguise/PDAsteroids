#pragma once
#include "renderableobject.h"

class Asteroid :
	public CRenderableObject
{
public:
	Asteroid(ModelData newMesh, EnVector3 initPos, EnVector3 initRot);
	Asteroid(ModelData newMesh, EnVector3 initPos);
	Asteroid(ModelData newMesh);
	~Asteroid();

	void Update();
	bool OnEvent(Event::IEvent* e);

private:
	void Init();
	void AddListeners();
	void RemoveListener(std::string eventType);
	void OnDeath();
	UINT uID;
};

