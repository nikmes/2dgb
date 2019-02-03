#ifndef RENDERINGSTATSWINDOW_H
#define RENDERINGSTATSWINDOW_H

#include "imguicomponent.h"

using namespace std;

class RenderingStatsWindow : public ImGUIComponent
{

private:

	TextureAtlas *m_textureAtlas;

public:

	RenderingStatsWindow(TextureAtlas *textureAtlas, string name);

	~RenderingStatsWindow();

	void run(bool* opened);

	void addIcons();

};

#endif