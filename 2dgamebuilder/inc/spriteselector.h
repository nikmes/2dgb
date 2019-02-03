/**
* \date    14 / 02 / 2015
* \author  Nicholas Messaritis
*
* \file    spriteselector.h
*
* Implementation of the main tool bar window
*
*/

#ifndef SPRITESELECTOR_H
#define SPRITESELECTOR_H

#include "imguicomponent.h"
#include "spritebatch.h"

using namespace std;

class SpriteSelector : public ImGUIComponent
{
	SpriteBatch *m_spriteBatch;

	Sprite m_selectedSprite;

public:
	
	~SpriteSelector();

	SpriteSelector(TextureAtlas *textureAtlas, string name);

	SpriteSelector();

	void run(bool* opened);

	void addIcons();

	void setSpriteBatch(SpriteBatch *spriteBatch);

	SpriteBatch *getSpriteBatch();

	Sprite getCurrentSprite();

};

#endif
