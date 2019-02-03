#ifndef LAYERBAR_H
#define LAYERBAR_H

#include "imguicomponent.h"

using namespace std;

class LayerBar : public ImGUIComponent
{

public:

	~LayerBar();

	LayerBar(TextureAtlas *textureAtlas, string name);

	void run(bool* opened);

	void addIcons();

	void layerProperties(bool *opened);
};

#endif