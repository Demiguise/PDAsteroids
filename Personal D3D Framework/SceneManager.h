#pragma once
#include "IEventManager.h"

class SceneManager;
class IEvent;
class Entity;
class Asteroid;

#define MAX_ASTEROID_COUNT 40
#define MAX_X_COORD 22
#define MAX_Y_COORD 22

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
	void UpdateEntities(float dt);
	bool OnEvent(Event::IEvent* e);
	void InitAsteroids();
	void InitLevel();

	Entity* activeCamera;
	SceneEventReceiver* receiver;
	bool sceneChangeThisFrame;
	std::vector<Asteroid*> asteroidPool;

private:
	void DealWithAsteroidDeath(Asteroid* deadAsteroid);
	bool CheckForAsteroids();
	void CheckObjectOnScreen(Entity* ent);

	std::deque<Entity*> availableEntities;
	UINT prevSceneEntityCount;
	std::map<LPCSTR, ModelData> MeshReference;
};

