#include "D3DBufferManager.h"
#include "RenderableObject.h"

D3DBufferManager::D3DBufferManager(ID3D11Device* mDevice)
{
	md3dDevice = mDevice;
}


D3DBufferManager::~D3DBufferManager()
{

}

void D3DBufferManager::InitNewBuffer(ModelData model, std::string name)
{
	bufferData newBuffers;
	//Create new Vertex Buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.CPUAccessFlags = 0;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.ByteWidth = sizeof(Vertex) * model.vData.size();
	vbd.StructureByteStride = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vbData = {0};
	vbData.pSysMem = &model.vData[0];
	md3dDevice->CreateBuffer(&vbd, &vbData, &newBuffers.vertexBuffer);

	//Create new Index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.CPUAccessFlags = 0;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.ByteWidth = sizeof(UINT) * model.iData.size();
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA ibData = {0};
	ibData.pSysMem = &model.iData[0];
	md3dDevice->CreateBuffer(&ibd, &ibData, &newBuffers.indexBuffer);
	
	newBuffers.iCount = model.iData.size();
	newBuffers.semanticName = name;
	newBuffers.topologyType = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
	bufferCache.push_back(newBuffers);
}

bool D3DBufferManager::LinkEntities(CRenderableObject* entity, std::string semanticModelName)
{
	for (UINT i = 0 ; i < bufferCache.size() ; ++i)
	{
		if (bufferCache[i].semanticName == semanticModelName)
		{
			RemoveEntity(entity); //This is in case we have duplicates. Which we will. Always...
			bufferMap.insert(std::pair<CRenderableObject*, UINT>(entity, i));
			return true;
		}
	}
	return false;
}

bool D3DBufferManager::RemoveEntity(CRenderableObject* entity)
{
	std::map<CRenderableObject*, UINT>::iterator bufferMapIT;
	for (bufferMapIT = bufferMap.begin() ; bufferMapIT != bufferMap.end() ; ++bufferMapIT)
	{
		if (entity->uID == bufferMapIT->first->uID)
		{
			bufferMap.erase(bufferMapIT);
			return true;
		}
	}
	return false;
}


std::vector<packedBufferData> D3DBufferManager::GrabSceneBuffers()
{
	std::vector<packedBufferData> sceneData;
	std::map<CRenderableObject*, UINT>::iterator bdIT;
	for (bdIT = bufferMap.begin() ; bdIT != bufferMap.end() ; ++bdIT)
	{
		if(bdIT->first->renderable)
		{
			packedBufferData newData;
			newData.owningEntity = bdIT->first;
			newData.bufferContents = bufferCache[bdIT->second];
			sceneData.push_back(newData);
		}
	}
	return sceneData;
}