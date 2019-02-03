#ifndef SCREENSHOTWINDOW_H
#define SCREENSHOTWINDOW_H

#include "imguicomponent.h"

using namespace std;

class ScreenShotWindow : public ImGUIComponent
{

private:

	TextureAtlas *m_textureAtlas;

public:

	ScreenShotWindow(TextureAtlas *textureAtlas, string name);

	~ScreenShotWindow();

	void run(bool* opened);

	void addIcons();
};

#endif#pragma once
