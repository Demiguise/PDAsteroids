#pragma once
#include "Entity.h"
#include "GameLog.h"

class Entity;

class SceneManager
{
public:
	static SceneManager* GetInstance();
	~SceneManager();

	void RegisterEntity(Entity* entity, UINT& idRef);
	void RemoveEntity(Entity* entity);
	UINT GenerateUID();
	void SetActiveCamera(Entity* newCam);
	void UpdateEntities();

	Entity* activeCamera;
	bool sceneChangeThisFrame;
	
private:
	SceneManager();
	static SceneManager* m_pInstance;

	ModelData InitModelData(LPSTR meshFileName);
	std::deque<Entity*> availableEntities;
	UINT prevSceneEntityCount;
	std::map<LPCSTR, ModelData> MeshReference;
};

