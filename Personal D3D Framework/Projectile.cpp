#include "Projectile.h"
#include "Colliders.h"
#include "GameLog.h"
#include "Events.h"
#include "IEventManager.h"


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
	delete lifeTimer;
}

void Projectile::Update()
{
	Entity::Update();
	if(active)
	{
		if (lifeTimer->elapsedTime() >= lifeSpan)
		{
			OnDeath();
		}
	}
}

void Projectile::OnDeath()
{
	active = false;
	renderable = false;
	rigidBody->isAwake = false;
	velocity = EnVector3::Zero();
	lifeTimer->stopTimer();
}

void Projectile::OnActivated(EnVector3 initPosition, EnVector3 initDirection)
{
	active = true;
	renderable = true;
	rigidBody->isAwake = true;
	position = initPosition;
	AddForce(initDirection, 8000);
	lifeTimer->resetTimer();
	lifeTimer->startTimer();
}

bool Projectile::OnEvent(Event::IEvent* e)
{
	if (e->eType == "Collision Event")
	{
		Event::CollisionEvent* collisionE = static_cast<Event::CollisionEvent*>(e);
		if (collisionE->entityA->uID == this->uID ||
			collisionE->entityB->uID == this->uID)
		{
			OnDeath();
			return false;
		}
	}
	return false;
}

void Projectile::Init()
{
	Event::PhysicsCreationEvent* e = new Event::PhysicsCreationEvent();
	e->eType = "PhysicsObj Created";
	e->entity = this;
	e->mass = 1.0f;
	e->scale = 0.2f;
	e->rbType = ColliderType::Sphere;
	IEventManager::GetInstance()->QueueEvent(e);

	active = false;
	renderable = false;
	AddListeners();
	lifeSpan = 5.0f;
	lifeTimer = new Timer();
}

void Projectile::SetRigidBody(RigidBody* newRB)
{
	rigidBody = newRB;
	rigidBody->isAwake = false;
}

void Projectile::AddListeners()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->AddListener("Collision Event", receiver);
}

void Projectile::RemoveListener(std::string eventType)
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveListener(eventType, receiver);
}