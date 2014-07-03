#include "Renderer.h"
#include "RenderableObject.h"

//RendererEventReceiver Class
RendererEventReceiver::RendererEventReceiver(Renderer* parentRenderer)
{
	parent = parentRenderer;
}

RendererEventReceiver::RendererEventReceiver() {}
RendererEventReceiver::~RendererEventReceiver() {}

bool RendererEventReceiver::Receive(Event::IEvent* e)
{
	return parent->OnEvent(e);
}

//Renderer Class
Renderer::Renderer()
{
	receiver = new RendererEventReceiver(this);
	IEventManager::GetInstance()->AddListener("RenderableObject Created", receiver);
	IEventManager::GetInstance()->AddListener("RenderableObject Destroyed", receiver);
	IEventManager::GetInstance()->AddListener("Asteroid Activated", receiver);
}


Renderer::~Renderer()
{
	delete receiver;
}
