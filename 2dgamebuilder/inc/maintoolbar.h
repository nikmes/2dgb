#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include "imguicomponent.h"

using namespace std;

class MainToolBar : public ImGUIComponent
{

public:

	~MainToolBar();
		
	MainToolBar(TextureAtlas *textureAtlas, string name);

	void run(bool* opened);

	void addIcons();
};

#endif
