#pragma once
#include "Common.h"
#include "Events.h"

//Singleton Design Pattern
class EventReceiver
{
public:
	EventReceiver() {};
	~EventReceiver() {};
	virtual bool Receive(Event::IEvent* e) = 0;
};

class IEventManager
{
public:
	static IEventManager* GetInstance();

	~IEventManager();
	void AddListener(std::string eventType, EventReceiver* regEntity);
	void RemoveListener(std::string eventType, EventReceiver* regEntity);
	void RemoveAllListenersFromEnt(EventReceiver* regEntity);
	void QueueEvent(Event::IEvent* newEvent);
	void Update();

private:
	IEventManager();
	static IEventManager* m_pInstance;
	std::queue<Event::IEvent*, std::deque<Event::IEvent*>> mainEventQueue;
	std::map<std::string, std::vector<EventReceiver*>> listenerMap;
};

