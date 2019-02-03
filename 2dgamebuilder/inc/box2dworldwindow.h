#ifndef BOX2DWORLDWINDOW_H
#define BOX2DWORLDWINDOW_H

#include "imguicomponent.h"

using namespace std;

class Box2dWorldWindow : public ImGUIComponent
{

private:

	TextureAtlas *m_textureAtlas;

public:

	Box2dWorldWindow(TextureAtlas *textureAtlas, string name);

	~Box2dWorldWindow();

	void run(bool* opened);

	void addIcons();
};

#endif