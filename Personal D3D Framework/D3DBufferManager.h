#pragma once
#include "Buffermanager.h"
#include "D3DCommon.h"

class D3DBufferManager :
	public BufferManager
{
public:
	D3DBufferManager(ID3D11Device* mDevice);
	~D3DBufferManager();
	void InitNewBuffer (ModelData model, std::string name);
	bool LinkEntities (CRenderableObject* entity, std::string semanticModelName);
	bool RemoveEntity (CRenderableObject* entity);
	
	std::vector<bufferData> bufferCache;
	std::vector<packedBufferData> GrabSceneBuffers ();
	std::map<CRenderableObject*, UINT> bufferMap;

private:
	ID3D11Device* md3dDevice;
};

