#include "SceneManager.h"
#include "GameLog.h"
#include "Entity.h"
#include "Events.h"
#include "IEventManager.h"
#include "Asteroid.h"
#include "FileManager.h"
#include "ScopedTimer.h"

//SceneEventReceiver Class
SceneEventReceiver::SceneEventReceiver(SceneManager* parentScene)
{
	parent = parentScene;
}

SceneEventReceiver::SceneEventReceiver() {}
SceneEventReceiver::~SceneEventReceiver() {}

bool SceneEventReceiver::Receive(Event::IEvent* e)
{
	return parent->OnEvent(e);
}

//SceneManager Class
SceneManager::SceneManager()
{
	asteroidPool.reserve(MAX_ASTEROID_COUNT);
	InitAsteroids();
	receiver = new SceneEventReceiver(this);
	IEventManager::GetInstance()->AddListener("Entity Created", receiver);
	IEventManager::GetInstance()->AddListener("Entity Destroyed", receiver);
	IEventManager::GetInstance()->AddListener("Asteroid Deactivated", receiver);
	GameLog::GetInstance()->Log(DebugChannel::Main, DebugLevel::Normal, "[SceneManager] Initialisation Complete.");
}

SceneManager::~SceneManager()
{
	IEventManager::GetInstance()->RemoveAllListenersFromEnt(receiver);
	delete receiver;
}

void SceneManager::InitAsteroids()
{
	for(UINT i = 0 ; i < MAX_ASTEROID_COUNT ; ++i)
	{
		asteroidPool.push_back(new Asteroid(FileManager::GetInstance()->LoadModelData("Models/Asteroid1L.line", FileType::LineFile)));
	}
}

void SceneManager::RegisterEntity(Entity* entity, UINT& idRef)
{
	idRef = GenerateUID();
	availableEntities.push_back(entity);
}

void SceneManager::RemoveEntity(Entity* entity)
{
	for (UINT i = 0 ; i < availableEntities.size() ; ++i)
	{
		if (availableEntities[i] == entity)
		{
			availableEntities.erase(availableEntities.begin() + i);
		}
	}
}

void SceneManager::UpdateEntities(float dt)
{
	for (UINT i = 0 ; i < availableEntities.size() ; ++i)
	{
		CheckObjectOnScreen(availableEntities[i]);
		availableEntities[i]->Update(dt);
	}
}

void SceneManager::InitLevel()
{
	for (Asteroid* a : asteroidPool)
	{
		a->SetActiveStatus(false);
	}
	float maxAsteroidPos = 20.0f;
	float minAsteroidPos = 3.0f;
	for (UINT i = 0 ; i < 10 ; ++i)
	{
		float startingX = Util::RandomFloat(-maxAsteroidPos, maxAsteroidPos);
		while (startingX < minAsteroidPos && startingX > -minAsteroidPos) { startingX = Util::RandomFloat(-maxAsteroidPos, maxAsteroidPos); }
		float startingY = Util::RandomFloat(-maxAsteroidPos, maxAsteroidPos);
		while (startingY < minAsteroidPos && startingY > -minAsteroidPos) { startingY = Util::RandomFloat(-maxAsteroidPos, maxAsteroidPos); }
		EnVector3 startPos = EnVector3(startingX, startingY, 0.0f);
		EnVector3 startVel = EnVector3(Util::RandomFloat(-5.0f, 5.0f), Util::RandomFloat(-5.0f, 5.0f), 0.0f);
		asteroidPool[i]->OnActivated(startPos, startVel, AsteroidSize::Large);
		asteroidPool[i]->mesh = FileManager::GetInstance()->LoadModelData("Models/Asteroid1L.line", FileType::LineFile);
	}
}

void SceneManager::CheckObjectOnScreen(Entity* ent)
{
	if (ent->position.x > MAX_X_COORD || ent->position.x < -MAX_X_COORD)
	{
		ent->position.x *= -1;
	}
	if (ent->position.y > MAX_Y_COORD || ent->position.y < -MAX_Y_COORD)
	{
		ent->position.y *= -1;
	}
	return;
}

bool SceneManager::OnEvent(Event::IEvent* e)
{
	if (e->eType == "Entity Created")
	{
		Entity* entity = static_cast<Event::EntityEvent*>(e)->entity;
		RegisterEntity(entity, entity->uID);
		return false;
	}
	else if (e->eType == "Entity Destroyed")
	{
		Entity* destroyedEnt = static_cast<Event::EntityEvent*>(e)->entity;
		RemoveEntity(destroyedEnt);
		return false;
	}
	else if (e->eType == "Asteroid Deactivated")
	{
		Asteroid* deactivatedAsteroid = static_cast<Asteroid*>(static_cast<Event::EntityEvent*>(e)->entity);
		DealWithAsteroidDeath(deactivatedAsteroid);
		return false;
	}
	return false;
}

void SceneManager::DealWithAsteroidDeath(Asteroid* deadAsteroid)
{
	UINT asteroidCount = 2;
	for (Asteroid* availableAsteroid : asteroidPool)
	{
		if (!availableAsteroid->active)
		{
			float x = Util::RandomFloat(-1.0f, 1.0f);
			float y = Util::RandomFloat(-1.0f, 1.0f);
			switch(deadAsteroid->size)
			{
			case AsteroidSize::Large:
				availableAsteroid->mesh = FileManager::GetInstance()->LoadModelData("Models/Asteroid1M.line", FileType::LineFile);
				availableAsteroid->OnActivated(deadAsteroid->position, EnVector3(x, y, 0.0f), AsteroidSize::Medium);
				break;
			case AsteroidSize::Medium:
				availableAsteroid->mesh = FileManager::GetInstance()->LoadModelData("Models/Asteroid1S.line", FileType::LineFile);
				availableAsteroid->OnActivated(deadAsteroid->position, EnVector3(x, y, 0.0f), AsteroidSize::Small);
				break;
			case AsteroidSize::Small:
				deadAsteroid->active = false;
				break;
			}
			--asteroidCount;
		}
		if (asteroidCount == 0) { break; }
	}
	deadAsteroid->active = false;
	if (!CheckForAsteroids())
	{
		Event::IEvent* e = new Event::IEvent;
		e->eType = "Level Complete";
		IEventManager::GetInstance()->QueueEvent(e);
		InitLevel();
	}
}

bool SceneManager::CheckForAsteroids()
{
	for (Asteroid* asteroid : asteroidPool)
	{
		if(asteroid->active) { return true; }
	}
	return false;
}

UINT SceneManager::GenerateUID()
{
	UINT newID = 0;
	for (UINT i = 0 ; i < availableEntities.size() ; ++i)
	{
		if (availableEntities[i]->GetUID() >= newID)
		{
			newID = availableEntities[i]->GetUID() + 1;
		}
	}
	return newID;
}

void SceneManager::SetActiveCamera(Entity* newCam)
{
	activeCamera = newCam;
}