#include "GameApp.h"

#define mClientWidth 800
#define mClientHeight 600

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR cmdLine, int showCmd)
{
	GameApp mGApp(hInstance);
	return mGApp.Run();
}

GameApp::GameApp(HINSTANCE hInstance)
{
	mFileManager = FileManager::GetInstance();
	mEventManager = IEventManager::GetInstance();
	mGameWindow = new GameWindow(this, hInstance, mClientHeight, mClientWidth);
	mRenderer = new D3DRenderer(hInstance, mGameWindow->GetMainWHandle(),
											mClientHeight, mClientWidth);
	mSceneManager = SceneManager::GetInstance();
	//Setup Camera with Position and Rotation
	mCamera = new Camera(	EnVector3(0.0f, 0.0f, 50.0f),
							EnVector3(0.0f, 0.0f, 0.0f));
	mCamera->name = "Camera 1";
	mSceneManager->SetActiveCamera(mCamera);

	mPhysicsManager = PhysicsManager::GetInstance();

	mTimer = new Timer();
	physicsTimer = new Timer();
	CreateTestObjects();
}

void GameApp::CreateTestObjects()
{
	m_pPlayer = new Player(mFileManager->LoadModelData("Models/Ship.line", FileType::LineFile),
													EnVector3(0.0f, 0.0f, 0.0f));
	mRenderer->CreateBuffer(m_pPlayer);
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
	while(msg.message != WM_QUIT)
	{
		mTimer->resetTimer();
		if (PeekMessage(&msg, 0, 0,0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (physicsTimer->elapsedTime() > 0.005f)
			{
				mPhysicsManager->Update(0.005f);
				physicsTimer->resetTimer();
			}
			
			mEventManager->Update();
			mSceneManager->UpdateEntities();
			Camera* activeCamera = static_cast<Camera*>(mSceneManager->activeCamera);
			mRenderer->UpdateScene(activeCamera->localToWorld, activeCamera->lookAtTarget);
			mRenderer->DrawScene();
		}
	}
	return (int)msg.wParam;
}