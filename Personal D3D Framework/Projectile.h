#pragma once
#include "renderableobject.h"
class Projectile :
	public CRenderableObject
{
public:
	Projectile(ModelData newMesh, EnVector3 initPos, EnVector3 initRot);
	Projectile(ModelData newMesh, EnVector3 initPos);
	Projectile(ModelData newMesh);
	~Projectile();

	void Update();
	bool OnEvent(Event::IEvent* e);

private:
	void Init();
	void AddListeners();
	void RemoveListener(std::string eventType);
	UINT uID;
};

