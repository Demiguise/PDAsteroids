#pragma once
#include "renderableobject.h"
#include "Projectile.h"

class Player :
	public CRenderableObject
{
public:
	Player(ModelData newMesh, EnVector3 initPos, EnVector3 initRot);
	Player(ModelData newMesh, EnVector3 initPos);
	Player(ModelData newMesh);
	~Player();
	
	void Update();
	bool OnEvent(Event::IEvent* e);
	
private:
	void Init();
	void AddListeners();
	void RemoveListener(std::string eventType);
	void FireShot();
	UINT uID;
};

