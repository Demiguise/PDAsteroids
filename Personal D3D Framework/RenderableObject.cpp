#include "RenderableObject.h"


CRenderableObject::CRenderableObject(ModelData newMesh, EnVector3 initPos, EnVector3 initRot) 
{
	position = initPos;
	rotation = initRot;
	mesh = newMesh;
}

CRenderableObject::CRenderableObject(ModelData newMesh, EnVector3 initPos) 
{
	position = initPos;
	mesh = newMesh;
}

CRenderableObject::CRenderableObject(ModelData newMesh) 
{
	mesh = newMesh;
}

CRenderableObject::CRenderableObject()
{
	
}

CRenderableObject::~CRenderableObject()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveAllListenersFromEnt(this);
}

void CRenderableObject::Update()
{
	Entity::Update();
}