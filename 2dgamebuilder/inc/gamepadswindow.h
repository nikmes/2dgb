#ifndef GAMEPADSWINDOW_H
#define GAMEPADSWINDOW_H

#include "imguicomponent.h"

using namespace std;

class GamePadsWindow : public ImGUIComponent
{

private:

	TextureAtlas *m_textureAtlas;

public:

	GamePadsWindow(TextureAtlas *textureAtlas, string name);

	~GamePadsWindow();

	void run(bool* opened);

	void addIcons();

};

#endif
