#include "RenderableObject.h"


CRenderableObject::CRenderableObject(ModelData newMesh, EnVector3 initPos, EnVector3 initRot) 
{
	position = initPos;
	rotation = initRot;
	mesh = newMesh;
	Init();
}

CRenderableObject::CRenderableObject(ModelData newMesh, EnVector3 initPos) 
{
	position = initPos;
	mesh = newMesh;
	Init();
}

CRenderableObject::CRenderableObject(ModelData newMesh) 
{
	mesh = newMesh;
	Init();
}

CRenderableObject::CRenderableObject()
{
	Init();
}

CRenderableObject::~CRenderableObject()
{
	Event::EntityEvent* e = new Event::EntityEvent();
	e->eType = "RenderableObject Destroyed";
	e->entity = this;
	IEventManager::GetInstance()->QueueEvent(e);
}

void CRenderableObject::Init()
{
	Event::EntityEvent* e = new Event::EntityEvent();
	e->eType = "RenderableObject Created";
	e->entity = this;
	IEventManager::GetInstance()->QueueEvent(e);
}

void CRenderableObject::Update()
{
	Entity::Update();
}