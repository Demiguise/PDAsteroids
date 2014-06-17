#pragma once
#include "Entity.h"
#include "IEventManager.h"
#include "GameLog.h"

class Camera :
	public Entity
{
public:
	Camera(EnVector3 initPos, EnVector3 initRot);
	Camera(EnVector3 initPos);
	Camera();
	~Camera();
	void Update();
	void SetPosition(EnVector3 newPos);
	void SetRotation(EnVector3 newRot);
	bool OnEvent(Event::IEvent* e);

	float mouseSensitivity;
	EnVector3 lookAtTarget;
	bool invertY;
	bool debugLockCamera;

private:
	void AddListeners();
	void RemoveListener(std::string eventType);
	void Init();
	UINT uID;
	EnVector2 curMousePosition;
};

