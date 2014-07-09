#include "UIElements.h"


UIElement::UIElement()
{
}


UIElement::~UIElement()
{
}

void UIElement::Update()
{
}

void UIElement::Render(ID3D11DeviceContext* pContext)
{
}


TextElement::TextElement(EnVector2 origin, float initFontSize, IFW1FontWrapper* fontWrapper):
	originPoint(origin),
	fontSize(initFontSize),
	pFontWrapper(fontWrapper)

{
}


TextElement::~TextElement()
{

}

void TextElement::Update()
{
	// turn char* into wchar_t*
	//mbstowcs_s(
}

void TextElement::Render(ID3D11DeviceContext* pContext)
{
	wchar_t wContent[20];
	mbstowcs(wContent, contents, 20);
	pFontWrapper->DrawString(
		pContext,
		wContent,
		fontSize,
		originPoint.x,
		originPoint.y,
		0xffffffff,
		0);
}