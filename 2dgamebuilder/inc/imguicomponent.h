/*
*  IMGUIComponent Base Class
*/

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <vector>
#include "imgui.h"
#include "textureatlas.h"

class ImGUIComponent
{

protected:

	TextureAtlas *m_textureAtlas;

	vector<textureAtlasItem> icons;

	ImTextureID texId;

	g2dTexture* tex;

	string m_name;

public:

	bool isVisible;

	~ImGUIComponent();

	ImGUIComponent();

	ImGUIComponent(TextureAtlas *textureAtlas, string name);

	void show() 
	{
		isVisible = true;
	}

	void hide()
	{
		isVisible = false;
	}

	string getName();
	
	void switchVisibility();

	virtual void run(bool* opened) { }

	virtual void addIcons() { } 

};

#endif
