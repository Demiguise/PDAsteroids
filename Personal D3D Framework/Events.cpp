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

	//EntityEvent class

	EntityEvent::EntityEvent()
	{

	}

	EntityEvent::~EntityEvent()
	{

	}

	//PhysicsCreationEvent class

	PhysicsCreationEvent::PhysicsCreationEvent()
	{

	}

	PhysicsCreationEvent::~PhysicsCreationEvent()
	{

	}

	//CollisionEvent class

	CollisionEvent::CollisionEvent()
	{

	}

	CollisionEvent::~CollisionEvent()
	{

	}
}