#include "SceneManager.h"

SceneManager* SceneManager::m_pInstance = 0;

SceneManager* SceneManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new SceneManager();
	}
	return m_pInstance;
}


SceneManager::SceneManager()
{
	GameLog::GetInstance()->Log(DebugChannel::Main, DebugLevel::Normal, "[SceneManager] Initialisation Complete.");
}

SceneManager::~SceneManager()
{

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

void SceneManager::UpdateEntities()
{
	for (UINT i = 0 ; i < availableEntities.size() ; ++i)
	{
		availableEntities[i]->Update();
	}
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