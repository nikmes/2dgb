#ifndef LAYERPROPERTIESWINDOW_H
#define LAYERPROPERTIESWINDOW_H

#include "imguicomponent.h"

using namespace std;

class LayerPropertiesWindow : public ImGUIComponent
{

private:

	TextureAtlas *m_textureAtlas;

public:

	LayerPropertiesWindow(TextureAtlas *textureAtlas, string name);

	~LayerPropertiesWindow();

	void run(bool* opened);

	void addIcons();
};

#endif