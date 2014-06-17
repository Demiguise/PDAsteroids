#include "Projectile.h"


Projectile::Projectile(ModelData newMesh, EnVector3 initPos, EnVector3 initRot)
{
	position = initPos;
	rotation = initRot;
	mesh = newMesh;
	Init();
}

Projectile::Projectile(ModelData newMesh, EnVector3 initPos)
{
	position = initPos;
	rotation = EnVector3::Zero();
	mesh = newMesh;
	Init();
}

Projectile::Projectile( ModelData newMesh)
{
	position = EnVector3::Zero();
	rotation = EnVector3::Zero();
	mesh = newMesh;
	Init();
}


Projectile::~Projectile()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveAllListenersFromEnt(this);
}

void Projectile::Update()
{

}

bool Projectile::OnEvent(Event::IEvent* e)
{
	return false;
}

void Projectile::Init()
{
	SceneManager::GetInstance()->RegisterEntity(this, uID);
	PhysicsManager::GetInstance()->RegisterEntity(this, ColliderType::Planar, 1);
	name = "Projectile";
	AddListeners();
}

void Projectile::AddListeners()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->AddListener("Collision", this);
}

void Projectile::RemoveListener(std::string eventType)
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveListener(eventType, this);
}