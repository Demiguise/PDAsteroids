#pragma once
#include "Common.h"
#include "FW1FontWrapper.h"

class IFW1FontWrapper;
class ID3D11DeviceContext;

//Basic UIElement
class UIElement
{
public:
	UIElement();
	~UIElement();
	virtual void Update();
	virtual void Render(ID3D11DeviceContext* pContext);
	bool active;
};

class TextElement : public UIElement
{
public:
	TextElement(EnVector2 origin, float initFontSize, IFW1FontWrapper* fontWrapper);
	~TextElement();
	void Update();
	void Render(ID3D11DeviceContext* pContext);
	IFW1FontWrapper* pFontWrapper;
	EnVector2 originPoint;
	char contents[20];
	float fontSize;
	
};

