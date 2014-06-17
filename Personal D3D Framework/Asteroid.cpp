#include "Asteroid.h"

Asteroid::Asteroid( ModelData newMesh, EnVector3 initPos, EnVector3 initRot)
{
	position = initPos;
	rotation = initRot;
	mesh = newMesh;
	Init();
}

Asteroid::Asteroid(ModelData newMesh, EnVector3 initPos)
{
	position = initPos;
	rotation = EnVector3::Zero();
	mesh = newMesh;
	Init();
}

Asteroid::Asteroid(ModelData newMesh)
{
	position = EnVector3::Zero();
	rotation = EnVector3::Zero();
	mesh = newMesh;
	Init();
}


Asteroid::~Asteroid()
{
	OnDeath();
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveAllListenersFromEnt(this);
}

void Asteroid::Update()
{

}

bool Asteroid::OnEvent(Event::IEvent* e)
{
	return false;
}

void Asteroid::Init()
{
	SceneManager::GetInstance()->RegisterEntity(this, uID);
	PhysicsManager::GetInstance()->RegisterEntity(this, ColliderType::Planar, 1);
	name = "Asteroid";
	AddListeners();
}

void Asteroid::OnDeath()
{

}

void Asteroid::AddListeners()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->AddListener("Collision", this);
}

void Asteroid::RemoveListener(std::string eventType)
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveListener(eventType, this);
}

