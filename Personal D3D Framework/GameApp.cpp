#include "GameApp.h"
#include "FileManager.h"
#include "GameWindow.h"
#include "Renderer.h"
#include "D3DRenderer.h"
#include "SceneManager.h"
#include "IEventManager.h"
#include "PhysicsManager.h"
#include "Player.h" 
#include "Asteroid.h"
#include "Timer.h"
#include "Camera.h"
#include "GameLog.h"
#include "Input.h"
#include "UI.h"
#include "AudioPlayer.h"

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

	m_pAudio = new AudioPlayer(mGameWindow->GetMainWHandle());
	mEventManager = IEventManager::GetInstance();
	LoadAllModels();
	gameRunning = false;
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

GameApp::~GameApp()
{
	delete mCamera;
	mSceneManager->RemoveEntity(mCamera);
} 

int GameApp::Run()
{
	MSG msg = {0};
	Timer fpsTimer, frameTimer, physicsTimer, renderTimer;
	renderTimer.startTimer();
	physicsTimer.startTimer();
	fpsTimer.startTimer();
	UINT fpsCounter = 0;
	//Ghetto game start.
	mEventManager->Update();
	mSceneManager->UpdateEntities();
	mSceneManager->InitLevel();
	mRenderer->GetUI()->ActivateMainMenu();
	m_pPlayer->SetActiveStatus(false);
	while(msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0,0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			CInput::GetInstance()->Update();

			if (CInput::GetInstance()->Contains(GameKey::ENTER) && gameRunning == false) //Check for game start
			{
				GameLog::GetInstance()->Log(DebugChannel::Main, DebugLevel::Normal, "[Game] Game starting.");
				m_pAudio->PlaySoundFile("Sounds/bikehorn.wav");
				mSceneManager->InitLevel();
				gameRunning = true;
				m_pPlayer->SetActiveStatus(true);
				mRenderer->GetUI()->ActivateMainUI();
			}

			if ((m_pPlayer->lives <= 0 && gameRunning == true) ||
				(CInput::GetInstance()->Contains(GameKey::ESC) && gameRunning == true)) //Check for game finished or Escape
			{
				GameLog::GetInstance()->Log(DebugChannel::Main, DebugLevel::Normal, "[Game] Game ended, returning to main menu.");
				gameRunning = false;
				m_pPlayer->SetActiveStatus(false);
				mRenderer->GetUI()->ActivateMainMenu();
			}

			mEventManager->Update();
			mSceneManager->UpdateEntities();

			//Physics Update
			if (physicsTimer.elapsedTime() > PHYSICS_STEP)
			{
				mPhysicsManager->Update(PHYSICS_STEP);
				physicsTimer.resetTimer();
			}

			//Render scene
			if (renderTimer.elapsedTime() > 0.0016f)
			{
				frameTimer.resetTimer();
				Camera* activeCamera = static_cast<Camera*>(mSceneManager->activeCamera);
				mRenderer->UpdateScene(activeCamera->localToWorld, activeCamera->lookAtTarget);
				mRenderer->GetUI()->Update(m_pPlayer);
				mRenderer->DrawScene();
				frameTimer.stopTimer();
				if (fpsTimer.elapsedTime() < 1.0f){ ++fpsCounter; }
				else {
					mGameWindow->Update(fpsCounter, frameTimer.elapsedTime());
					fpsTimer.resetTimer();
					fpsCounter = 0;
				}
			}
		}
	}
	return (int)msg.wParam;
}