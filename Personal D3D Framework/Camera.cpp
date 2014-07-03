#include "Camera.h"
#include "Colliders.h"
#include "Events.h"
#include "IEventManager.h"
#include "GameLog.h"

Camera::Camera(EnVector3 initPos, EnVector3 initRot)
{
	position = initPos;
	rotation = initRot;
	Init();
}

Camera::Camera(EnVector3 initPos) 
{
	position = initPos;
	rotation = EnVector3(0.0f, 0.0f, 0.0f);
	Init();
}

Camera::Camera() 
	: Entity ()
{
	position = EnVector3(0.0f, 0.0f, 0.0f);
	rotation = EnVector3(0.0f, 0.0f, 0.0f);
	Init();
}

Camera::~Camera()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveAllListenersFromEnt(receiver);
}

void Camera::Init()
{
	rigidBody = 0;
	mouseSensitivity = 0.15f;
	invertY = false;
	debugLockCamera = true;
	curMousePosition = EnVector2(400.0f, 300.0f);
	//AddListeners();
}


void Camera::AddListeners()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->AddListener("UserMouseMove", receiver);
	eventMan->AddListener("UserKeyPress", receiver);
	eventMan->AddListener("UserKeyRelease", receiver);
}

void Camera::RemoveListener(std::string eventType)
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveListener(eventType, receiver);
}

void Camera::Update()
{
	lookAtTarget = Util::ScalarProduct3D(GetLocalAxis(2), -2.0f) + position;
	UpdateQuaternion();
	UpdateLocalToWorldMatrix();
	//Update our AABB to our current co-ordinates.
	if (rigidBody != 0)
	{
		rigidBody->ReCalculateAABB(AABB);
	}
}


void Camera::SetPosition(EnVector3 newPos)
{
	position = newPos;
}

void Camera::SetRotation(EnVector3 newRot)
{
	rotation = newRot;
}

bool Camera::OnEvent(Event::IEvent* e)
{
	if (e->eType == "UserKeysActive")
	{
		std::vector<GameKey> keyPresses = *static_cast<Event::InputEvent*>(e)->keyEvents;
		for (UINT i = 0 ; i < keyPresses.size() ; i++)
		{
			switch (keyPresses[i])
			{
			case GameKey::A:
				SetPosition(position + Util::ScalarProduct3D(GetLocalAxis(0), 0.1f));
				break;
			case GameKey::D:
				SetPosition(position + Util::ScalarProduct3D(GetLocalAxis(0), -0.1f));
				break;
			case GameKey::E:
				SetRotation(rotation + EnVector3(0.0f, 0.0f, -5.0f));
				break;
			case GameKey::Q:
				SetRotation(rotation + EnVector3(0.0f, 0.0f, 5.0f));
				break;
			case GameKey::S:
				SetPosition(position + Util::ScalarProduct3D(GetLocalAxis(2), 0.1f));
				break;
			case GameKey::W:
				SetPosition(position + Util::ScalarProduct3D(GetLocalAxis(2), -0.1f));
				break;
			case GameKey::X:
				SetPosition(position + Util::ScalarProduct3D(GetLocalAxis(1), -0.1f));
				break;
			case GameKey::SPACE:
				SetPosition(position + Util::ScalarProduct3D(GetLocalAxis(1), 0.1f));
				break;
			}
		}
		return true;
	}
	if (e->eType == "UserMouseMove" && debugLockCamera != true)
	{
		Event::InputEvent* mouseMove = static_cast<Event::InputEvent*>(e);
		EnVector2 localMove = mouseMove->mouseMovement;
		//Change in the X position of the mouse means a rotation in the Y-axis as it's our up direction. This may be slightly confusing.
		float deltaX = (curMousePosition.x - mouseMove->mouseMovement.x)* mouseSensitivity;
		float deltaY = (curMousePosition.y - mouseMove->mouseMovement.y) * ((invertY)?-1:1) * mouseSensitivity;
		EnVector3 newRotation = Util::ScalarProduct3D(GetLocalAxis(0), deltaY) + Util::ScalarProduct3D(GetLocalAxis(1), -deltaX);
		SetRotation(rotation + newRotation);
		
		curMousePosition = localMove;
	}

	return false;
}