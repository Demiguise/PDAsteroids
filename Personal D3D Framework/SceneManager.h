#pragma once
#include "Entity.h"
#include "Asteroid.h"
#include "GameLog.h"
#include "IEventManager.h"


#define MAX_ASTEROID_COUNT 40

class Entity;

class SceneEventReceiver : public EventReceiver
{
public:
	SceneEventReceiver(SceneManager* parentScene);
	SceneEventReceiver();
	~SceneEventReceiver();
	bool Receive(Event::IEvent* e);

private:
	SceneManager* parent;
};

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void RegisterEntity(Entity* entity, UINT& idRef);
	void RemoveEntity(Entity* entity);
	UINT GenerateUID();
	void SetActiveCamera(Entity* newCam);
	void UpdateEntities();
	bool OnEvent(Event::IEvent* e);
	void InitAsteroids();

	Entity* activeCamera;
	SceneEventReceiver* receiver;
	bool sceneChangeThisFrame;
	std::vector<Asteroid*> asteroidPool;

private:
	void DealWithAsteroidDeath(Asteroid* deadAsteroid);

	std::deque<Entity*> availableEntities;
	UINT prevSceneEntityCount;
	std::map<LPCSTR, ModelData> MeshReference;
};

