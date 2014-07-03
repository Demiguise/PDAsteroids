#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <WinUser.h>
#include "Common.h"

class CInput
{
public:
	static CInput* GetInstance();
	~CInput();
	void Update();
	void OnEvent(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	CInput();
	static CInput* m_pInstance;
	GameKey TranslateKeyPress(WPARAM wParam);
	void AddActiveKey(GameKey newKey);
	void RemoveActiveKey(GameKey oldKey);
	std::vector<GameKey> activeKeys;
};

