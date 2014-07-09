#include "UI.h"
#include "UIElements.h"
#include "FW1FontWrapper.h"
#include "Player.h"

UI::UI(ID3D11Device* pDevice)
{
	HRESULT hr = FW1CreateFactory(FW1_VERSION, &pFW1Factory);
	hr = pFW1Factory->CreateFontWrapper(pDevice, L"Arial", &pFontWrapper);
	scoreElement = new TextElement(EnVector2(0.0f,0.0f), 15.0f, pFontWrapper);
	lifeElement = new TextElement(EnVector2(749.0f, 0.0f), 15.0f, pFontWrapper);
	levelElement = new TextElement(EnVector2(350.f, 0.0f), 15.0f, pFontWrapper);
}


UI::~UI()
{
	pFontWrapper->Release();
	pFW1Factory->Release();
}

void UI::Render(ID3D11DeviceContext* pContext)
{
	scoreElement->Render(pContext);
	lifeElement->Render(pContext);
	levelElement->Render(pContext);
}

void UI::Update(Player* mPlayer)
{
	_snprintf(scoreElement->contents, 20, "Score: %d", mPlayer->score);
	_snprintf(lifeElement->contents, 20, "Lives: %d", mPlayer->lives);
	_snprintf(levelElement->contents, 20, "Level (%d)", mPlayer->curLevel);
}