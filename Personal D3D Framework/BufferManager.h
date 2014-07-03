#pragma once
#include "Common.h"
class CRenderableObject;

class BufferManager
{
public:
	BufferManager();
	~BufferManager();
	virtual void InitNewBuffer(ModelData model, std::string name) = 0;
	virtual bool LinkEntities (CRenderableObject* entity, std::string semanticModelName) = 0;
	virtual bool RemoveEntity (CRenderableObject* entity) = 0;
};

