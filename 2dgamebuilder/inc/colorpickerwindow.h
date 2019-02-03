#ifndef COLORPICKERWINDOW_H
#define COLORPICKERWINDOW_H

#include "imguicomponent.h"

using namespace std;

class ColorPickerWindow : public ImGUIComponent
{

private:

	TextureAtlas *m_textureAtlas;

public:

	ColorPickerWindow(TextureAtlas *textureAtlas, string name);

	~ColorPickerWindow();

	void run(bool* opened);

	void addIcons();
	
	bool ColorPicker3(float col[3]);

	bool ColorPicker4(float *col, bool show_alpha);

};

#endif
