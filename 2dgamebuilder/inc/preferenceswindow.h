#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include "imguicomponent.h"

using namespace std;

class PreferencesWindow : public ImGUIComponent
{

private:

	TextureAtlas *m_textureAtlas;

public:

	PreferencesWindow(TextureAtlas *textureAtlas, string name);

	~PreferencesWindow();

	void run(bool* opened);

	void addIcons();

};

#endif
