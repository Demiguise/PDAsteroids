#pragma once
#include "Common.h"
#include <D3D11.h>
#include <D3DX11.h>
#include <xnamath.h>
#include <d3dx11effect.h>
#include <DxErr.h>

class Entity;

struct bufferData
{
	std::string semanticName;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY topologyType;
	UINT iCount;
};

struct packedBufferData
{
	Entity* owningEntity;
	bufferData bufferContents;
};

//Helpful Macros (Shameless 'Stolen' from Frank D Luna)

#define ReleaseCOM(x) { if(x) { x->Release(); x = 0; } }

//Need to have the '\' at the end of the line to say it's a
//line macro. Since they are usually just a single line.
#define HR(x)												\
{															\
	HRESULT hr = x;											\
	if (FAILED(hr))											\
	{														\
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true);	\
	}														\
}
