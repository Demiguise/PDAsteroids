#pragma once
#include "Common.h"
#include "Camera.h"
#include "Renderer.h"
#include "D3DRenderer.h"
#include "GameWindow.h"
#include "SceneManager.h"
#include "IEventManager.h"
#include "PhysicsManager.h"
#include "Player.h" 
#include "GameLog.h"
#include "Timer.h"
#include <Awesomium\WebCore.h>

class GameWindow;

class GameApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();
	int Run();
	void CreateTestObjects();
	void LoadAllModels();
	void GhettoSpawnAsteroids();

	FileManager* mFileManager;
	GameWindow* mGameWindow;
	Renderer* mRenderer;
	SceneManager* mSceneManager;
	IEventManager* mEventManager;
	PhysicsManager* mPhysicsManager;
	Camera* mCamera;
	Timer* mTimer;
	Timer* physicsTimer;
	Player* m_pPlayer;
};

