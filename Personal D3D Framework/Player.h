#pragma once
#include "renderableobject.h"

#define MAX_PLAYER_PROJECTILES 20
#define THRUSTVELOCITY 80
#define ROTATIONSPEED 5.5f

class Projectile;

class Player :
	public CRenderableObject
{
public:
	Player(ModelData newMesh, EnVector3 initPos, EnVector3 initRot);
	Player(ModelData newMesh, EnVector3 initPos);
	Player(ModelData newMesh);
	~Player();
	
	void Update(float dt);
	bool OnEvent(Event::IEvent* e);
	UINT lives;
	UINT score;
	UINT curLevel;
	void SetActiveStatus(bool status);

private:
	void Init();
	void AddListeners();
	void RemoveListener(std::string eventType);
	void FireShot();

	std::vector<Projectile*> projectilePool;
	EnVector3 projectileHelper;
	float maxInvulTime;
	float curInvulTime;
	float maxReloadTime;
	float curReloadTime;
	bool active;
};

