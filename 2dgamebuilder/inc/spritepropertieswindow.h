#ifndef SPRITEPROPERTIESWINDOW_H
#define SPRITEPROPERTIESWINDOW_H

#include "imguicomponent.h"

using namespace std;

class SpritePropertiesWindow : public ImGUIComponent
{

private:

	TextureAtlas *m_textureAtlas;

public:

	SpritePropertiesWindow(TextureAtlas *textureAtlas, string name);

	~SpritePropertiesWindow();

	void run(bool* opened);

	void addIcons();
};

#endif