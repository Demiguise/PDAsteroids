#pragma once
#include "Entity.h"

class CRenderableObject :
	public Entity
{
public:
	CRenderableObject(ModelData newMesh, EnVector3 initPos, EnVector3 initRot);
	CRenderableObject(ModelData newMesh, EnVector3 initPos);
	CRenderableObject(ModelData newMesh);
	CRenderableObject();
	~CRenderableObject();
	
	void Update();
	bool renderable;
	ModelData mesh;

private:
	UINT uID;
};
