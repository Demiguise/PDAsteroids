#pragma once
#include "GameWindow.h"
#include "D3DRenderer.h"
#include "SceneManager.h"
#include "FileManager.h"
#include "IEventManager.h"
#include "PhysicsManager.h"

class GameWindow;
class Renderer;
class SceneManager;
class FileManager;
class IEventManager;
class PhysicsManager;

class Services
{
public:
	static Services* GetInstance();
	~Services();
private:
	Services();
	static Services* m_pInstance;

	FileManager* mFileManager;
	GameWindow* mGameWindow;
	Renderer* mRenderer;
	SceneManager* mSceneManager;
	IEventManager* mEventManager;
	PhysicsManager* mPhysicsManager;
};

