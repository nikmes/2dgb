#ifndef CAMERAPROPERTIESWINDOW_H
#define CAMERAPROPERTIESWINDOW_H

#include "imguicomponent.h"

using namespace std;

class CameraPropertiesWindow : public ImGUIComponent
{

private:

	TextureAtlas *m_textureAtlas;

public:

	CameraPropertiesWindow(TextureAtlas *textureAtlas, string name);

	~CameraPropertiesWindow();

	void run(bool* opened);

	void addIcons();
};

#endif