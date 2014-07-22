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
	void ActivateMainMenu();
	void ActivateMainUI();


private:
	IFW1FontWrapper* pUIFontWrapper;
	IFW1FontWrapper* pTitleFontWrapper;
	IFW1Factory* pFW1Factory;
	//These are also terrible and should be done through other means.
	//Duplication of variables is never a good thing.
	unsigned int curScore;
	unsigned int curLives;
	unsigned int curLevel;

	//I just... I just want them to be here like this.
	//I have no real reasoning, this could be done so much better.
	//I just  want them to be here... like this.
	//Main Menu
	TextElement* mainTitle;
	TextElement* entryMessage;
	//Main Game UI
	TextElement* scoreElement;
	TextElement* lifeElement;
	TextElement* levelElement;
	bool gameRunning;
};

