#pragma once

class UIElement;
class ID3D11Device;
class ID3D11DeviceContext;
class IFW1FontWrapper;
class IFW1Factory;
class TextElement;
class Player;

class UI
{
public:
	UI(ID3D11Device* pDevice);
	~UI();
	void Update(Player* mPlayer);
	void Render(ID3D11DeviceContext* pContext);

private:
	IFW1FontWrapper* pFontWrapper;
	IFW1Factory* pFW1Factory;
	//I just... I just want them to be here like this.
	//I have no real reasoning, this could be done so much better.
	//I just  want them to be here... like this.
	TextElement* scoreElement;
	TextElement* lifeElement;
	TextElement* levelElement;
};

