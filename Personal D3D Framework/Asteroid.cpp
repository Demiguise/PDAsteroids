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
	eventMan->RemoveAllListenersFromEnt(receiver);
}

void Asteroid::Update()
{
	Entity::Update();
}

void Asteroid::OnActivated(EnVector3 initPosition, EnVector3 initDirection, AsteroidSize initSize)
{
	Event::EntityEvent* e = new Event::EntityEvent();
	e->eType = "Asteroid Activated";
	e->entity = this;
	IEventManager::GetInstance()->QueueEvent(e);

	
	active = true;
	renderable = true;
	rigidBody->isAwake = true;
	position = initPosition;
	AddForce(initDirection, rand() % 250 + 1000);
	size = initSize;
}

void Asteroid::OnDeath()
{
	Event::EntityEvent* e = new Event::EntityEvent();
	e->eType = "Asteroid Deactivated";
	e->entity = this;
	IEventManager::GetInstance()->QueueEvent(e);

	renderable = false;
	rigidBody->isAwake = false;
	velocity = EnVector3::Zero();
}

bool Asteroid::OnEvent(Event::IEvent* e)
{
	if (e->eType == "Collision Event")
	{
		Event::CollisionEvent* collisionE = static_cast<Event::CollisionEvent*>(e);
		if ((	collisionE->entityA->uID == this->uID &&
				collisionE->entityB->name == "Projectile") || (
				collisionE->entityA->name == "Projectile" &&
				collisionE->entityB->uID == this->uID))
		{
			OnDeath();
			return false;
		}
	}
	return false;
	return false;
}

void Asteroid::Init()
{
	name = "Asteroid";

	Event::PhysicsCreationEvent* e = new Event::PhysicsCreationEvent();
	e->eType = "PhysicsObj Created";
	e->entity = this;
	e->mass = 1.0f;
	e->scale = 1.0f;
	e->rbType = ColliderType::Sphere;
	IEventManager::GetInstance()->QueueEvent(e);

	size = AsteroidSize::Large;
	active = false;
	renderable = false;
	AddListeners();
}

void Asteroid::SetRigidBody(RigidBody* newRB)
{
	rigidBody = newRB;
	rigidBody->isAwake = false;
}

void Asteroid::AddListeners()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->AddListener("Collision Event", receiver);
}

void Asteroid::RemoveListener(std::string eventType)
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveListener(eventType, receiver);
}

