#include "GameApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR cmdLine, int showCmd)
{
	GameApp mGApp(hInstance);
	return mGApp.Run();
}

GameApp::GameApp(HINSTANCE hInstance)
{
	mFileManager = FileManager::GetInstance();
	mGameWindow = new GameWindow(this, hInstance, mClientHeight, mClientWidth);
	mRenderer = new D3DRenderer(hInstance, mGameWindow->GetMainWHandle(),
											mClientHeight, mClientWidth);
	mSceneManager = new SceneManager();
	//Setup Camera with Position and Rotation
	mCamera = new Camera(	EnVector3(0.0f, 0.0f, 50.0f),
							EnVector3(0.0f, 0.0f, 0.0f));
	mCamera->name = "Camera 1";
	mSceneManager->SetActiveCamera(mCamera);
	mPhysicsManager = new PhysicsManager();

	mEventManager = IEventManager::GetInstance();
	mTimer = new Timer();
	physicsTimer = new Timer();
	LoadAllModels();
	

	CreateTestObjects();
}

void GameApp::CreateTestObjects()
{
	m_pPlayer = new Player(mFileManager->LoadModelData("Models/Ship.line", FileType::LineFile),
													EnVector3(0.0f, 0.0f, 0.0f));
}

void GameApp::LoadAllModels()
{
	//All of them.
	mRenderer->CreateBuffer(mFileManager->LoadModelData("Models/Ship.line", FileType::LineFile),
							"Models/Ship.line");
	mRenderer->CreateBuffer(mFileManager->LoadModelData("Models/Projectile.line", FileType::LineFile),
							"Models/Projectile.line");
	mRenderer->CreateBuffer(mFileManager->LoadModelData("Models/Asteroid1L.line", FileType::LineFile),
							"Models/Asteroid1L.line");
	mRenderer->CreateBuffer(mFileManager->LoadModelData("Models/Asteroid1M.line", FileType::LineFile),
							"Models/Asteroid1M.line");
	mRenderer->CreateBuffer(mFileManager->LoadModelData("Models/Asteroid1S.line", FileType::LineFile),
							"Models/Asteroid1S.line");
}

void GameApp::GhettoSpawnAsteroids()
{
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
		mSceneManager->asteroidPool[i]->OnActivated(startPos, startVel, AsteroidSize::Large);
	}
}

GameApp::~GameApp()
{
	delete mCamera;
	mSceneManager->RemoveEntity(mCamera);
}

int GameApp::Run()
{
	MSG msg = {0};
	mTimer->startTimer();
	physicsTimer->startTimer();

	//Ghetto game start.
	mEventManager->Update();
	mSceneManager->UpdateEntities();
	GhettoSpawnAsteroids();

	while(msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0,0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if(mTimer->elapsedTime() > 0.016f)
		{
			CInput::GetInstance()->Update();
			mEventManager->Update();
			mSceneManager->UpdateEntities();
			if (physicsTimer->elapsedTime() > PHYSICS_STEP)
			{
				mPhysicsManager->Update(PHYSICS_STEP);
				physicsTimer->resetTimer();
			}
			Camera* activeCamera = static_cast<Camera*>(mSceneManager->activeCamera);
			mRenderer->UpdateScene(activeCamera->localToWorld, activeCamera->lookAtTarget);
			mRenderer->DrawScene();
			mTimer->resetTimer();
		}
	}
	return (int)msg.wParam;
}