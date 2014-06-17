#include "Events.h"

//Base IEvent class
namespace Event
{
	IEvent::IEvent()
	{
		eType = "UnInitialised";
	}


	IEvent::~IEvent()
	{

	}

	//InputEvent class

	InputEvent::InputEvent()
	{

	}

	InputEvent::~InputEvent()
	{

	}
}