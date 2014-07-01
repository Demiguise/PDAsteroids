#pragma once
#include "Common.h"

class Entity;
enum ColliderType;

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

	class EntityEvent : public IEvent
	{
	public:
		EntityEvent();
		~EntityEvent();
		Entity* entity;
	};

	class PhysicsCreationEvent : public IEvent
	{
	public:
		PhysicsCreationEvent();
		~PhysicsCreationEvent();
		Entity* entity;
		ColliderType rbType;
		float mass;
		float scale;
	};

	class CollisionEvent : public IEvent
	{
	public:
		CollisionEvent();
		~CollisionEvent();
		Entity* entityA;
		Entity* entityB;
	};
}