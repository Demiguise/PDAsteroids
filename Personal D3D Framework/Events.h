#pragma once
#include "Common.h"

namespace Event 
{
	class IEvent
	{
	public:
		IEvent();
		~IEvent();
		std::string eType;
	};

	class InputEvent : public IEvent
	{
	public:
		InputEvent();
		~InputEvent();
		EnVector2 mouseMovement;
		std::vector<GameKey>* keyEvents;
	};
}