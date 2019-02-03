#ifndef GRIDPROPERTIESWINDOW_H
#define GRIDPROPERTIESWINDOW_H

#include "imguicomponent.h"
#include "textureatlas.h"

class GridPropertiesWindow : public ImGUIComponent
{

private:

public:

	GridPropertiesWindow(TextureAtlas *textureAtlas, string name);

	~GridPropertiesWindow();

	void run(bool* opened);

	void addIcons();

};

#endif