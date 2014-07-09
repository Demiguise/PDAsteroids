#pragma once
#include "Common.h"

class FileManager;
class GameWindow;
class Renderer;
class SceneManager;
class IEventManager;
class PhysicsManager;
class Player;
class Timer;
class Camera;
class UI;

class GameApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();
	int Run();
	void CreateTestObjects();
	void LoadAllModels();

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

