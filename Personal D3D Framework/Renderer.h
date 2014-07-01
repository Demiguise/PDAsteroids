#pragma once
#include "RenderableObject.h"
#include "Common.h"
#include "BufferManager.h"
#include "IEventManager.h"

//This is a abstract base class, I don't want it to ever be called but it provides everything else
//that needs to use a renderer the correct interfaces.

class Renderer;

class RendererEventReceiver : public EventReceiver
{
public:
	RendererEventReceiver(Renderer* parentRenderer);
	RendererEventReceiver();
	~RendererEventReceiver();
	bool Receive(Event::IEvent* e);

private:
	Renderer* parent;
};

class Renderer
{
public:
	Renderer();
	~Renderer();
	virtual bool Init() = 0;

	virtual void UpdateScene(const EnMatrix4x4 &camPos, const EnVector3& target) = 0;
	virtual void DrawScene() = 0;
	virtual void OnResize(UINT newHeight, UINT newWidth) = 0;
	virtual void CreateBuffer(ModelData model, std::string name) = 0;
	virtual bool OnEvent(Event::IEvent* e) = 0;

	std::string rendererName;
	RendererEventReceiver* receiver;
};