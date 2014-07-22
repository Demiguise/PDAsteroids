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
class AudioPlayer;

class GameApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();
	int Run();
	void LoadAllModels();

private:
	FileManager* mFileManager;
	GameWindow* mGameWindow;
	Renderer* mRenderer;
	SceneManager* mSceneManager;
	IEventManager* mEventManager;
	PhysicsManager* mPhysicsManager;
	Camera* mCamera;
	Player* m_pPlayer;
	AudioPlayer* m_pAudio;
	bool gameRunning;
};

