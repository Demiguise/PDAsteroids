#include "Input.h"
#include "GameLog.h"
#include "Events.h"
#include "IEventManager.h"

CInput* CInput::m_pInstance = 0;

CInput* CInput::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CInput();
	}
	return m_pInstance;
}

CInput::CInput()
{
	GameLog::GetInstance()->Log(DebugChannel::Input, DebugLevel::None, "[InputManager] Testing Input Channel! %i", 3);
	activeKeys.reserve(10);
}


CInput::~CInput()
{
}

void CInput::Update()
{
	IEventManager* eventManager = IEventManager::GetInstance();
	Event::InputEvent* inputEvent = new Event::InputEvent();
	if (activeKeys.size() != 0)
	{
		inputEvent->eType = "UserKeysActive";
		inputEvent->keyEvents = &activeKeys;
		eventManager->QueueEvent(inputEvent);
		//GameLog::GetInstance()->Log(DebugChannel::Input, DebugLevel::Normal, "[InputManager] %d currently active keys.", activeKeys.size());
	}
}

void CInput::OnEvent(UINT msg, WPARAM wParam, LPARAM lParam)
{
	IEventManager* eventManager = IEventManager::GetInstance();
	Event::InputEvent* inputEvent = new Event::InputEvent();
	switch (msg)
	{
	case WM_MOUSEMOVE:
		inputEvent->eType = "UserMouseMove";
		inputEvent->mouseMovement = EnVector2(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_KEYDOWN:
		AddActiveKey(TranslateKeyPress(wParam));
		break;
	case WM_KEYUP:
		RemoveActiveKey(TranslateKeyPress(wParam));
		break;
	}
	eventManager->QueueEvent(inputEvent);
}

void CInput::AddActiveKey(GameKey newKey)
{
	for (GameKey& k : activeKeys)
	{
		if (k == newKey) { return; }
	}
	activeKeys.push_back(newKey);
}

void CInput::RemoveActiveKey(GameKey oldKey)
{
	UINT count = 0;
	for (GameKey& k : activeKeys)
	{
		if (k == oldKey) 
		{ 
			activeKeys.erase(activeKeys.begin() + count);
			return;
		}
		count++;
	}
}

GameKey CInput::TranslateKeyPress(WPARAM wParam)
{
	switch(wParam)
	{
	case 0x41:
		return GameKey::A;
	case 0x44:
		return GameKey::D;
	case 0x45:
		return GameKey::E;
	case 0x51:
		return GameKey::Q;
	case 0x53:
		return GameKey::S;
	case 0x57:
		return GameKey::W;
	case 0x58:
		return GameKey::X;
	case 0x20:
		return GameKey::SPACE;
	}
}