#include "UI.h"
#include "UIElements.h"
#include "FW1FontWrapper.h"
#include "Player.h"

UI::UI(ID3D11Device* pDevice)
{
	HRESULT hr = FW1CreateFactory(FW1_VERSION, &pFW1Factory);
	hr = pFW1Factory->CreateFontWrapper(pDevice, L"Arial", &pUIFontWrapper);
	hr = pFW1Factory->CreateFontWrapper(pDevice, L"Georgia", &pTitleFontWrapper);
	//Main Menu
	mainTitle = new TextElement(EnVector2((mClientWidth / 2) - 130, mClientHeight / 3), 65.0f, pTitleFontWrapper);
	char titleContents[20] = {"Asteroids"};
	memcpy(&mainTitle->contents, &titleContents, 20);
	entryMessage = new TextElement(EnVector2((mClientWidth / 2) - 65, (mClientHeight / 2) - 30), 25.0f, pUIFontWrapper);
	char entryContents[20] = {"Press Enter"};
	memcpy(&entryMessage->contents, &entryContents, 20);
	//Main UI
	scoreElement = new TextElement(EnVector2(0.0f,0.0f), 15.0f, pUIFontWrapper);
	lifeElement = new TextElement(EnVector2(749.0f, 0.0f), 15.0f, pUIFontWrapper);
	levelElement = new TextElement(EnVector2(350.f, 0.0f), 15.0f, pUIFontWrapper);
}


UI::~UI()
{
	pUIFontWrapper->Release();
	pTitleFontWrapper->Release();
	pFW1Factory->Release();
}

void UI::Render(ID3D11DeviceContext* pContext)
{
	if (gameRunning)
	{
		scoreElement->Render(pContext);
		lifeElement->Render(pContext);
		levelElement->Render(pContext);
	}
	else
	{
		mainTitle->Render(pContext);
		entryMessage->Render(pContext);
	}
}

void UI::Update(Player* mPlayer)
{
	if (gameRunning)
	{
		if (curScore != mPlayer->score) { 
			curScore = mPlayer->score; 
			_snprintf(scoreElement->contents, 20, "Score: %d", mPlayer->score); 
		}
		if (curLives != mPlayer->lives) { 
			curLives = mPlayer->lives; 
			_snprintf(lifeElement->contents, 20, "Lives: %d", mPlayer->lives); 
		}
		if (curLevel != mPlayer->curLevel) { 
			curLevel = mPlayer->curLevel; 
			_snprintf(levelElement->contents, 20, "Level (%d)", mPlayer->curLevel); 
		}
	}
}

void UI::ActivateMainMenu()
{
	gameRunning = false;
}

void UI::ActivateMainUI()
{
	gameRunning = true;
}