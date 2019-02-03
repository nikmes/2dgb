/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file imguiprovider.h
*
*/

#ifndef IMGUIPROVIDER_H
#define IMGUIPROVIDER_H

#include "imgui.h"
#include "loggerwindow.h"
#include "maintoolbar.h"
#include "preferenceswindow.h"
#include "filedialogwindow.h"
#include "spriteselector.h"
#include "renderingstatswindow.h"
#include "gridpropertieswindow.h"
#include "editbar.h"
#include "imguicomponent.h"
#include "layerbar.h"
#include "camerapropertieswindow.h"
#include "layerpropertieswindow.h"
#include "box2dworldwindow.h"
#include "spritepropertieswindow.h"
#include "gamepadswindow.h"
#include "screenshotwindow.h"
#include "colorpickerwindow.h"
#include "gamebar.h"
#include "benchmarkingwindow.h"

using namespace std;

class IMGUIProvider
{
private:
	
	map<string, ImGUIComponent*> imGuiComponents;

	ImFont* my_font0;
	ImFont* my_font1;
	ImFont* my_font2;
	ImFont* my_font3;

public:

	TextureAtlas *textureAtlas;

	ImVector<char*> Items;
	bool ScrollToBottom;

	// Define all windows our editor can render

	GridPropertiesWindow *gridPropertiesWindow;

	FileDialogWindow *fileDialogWindow;
	
	PreferencesWindow *preferencesWindow;

	LoggerWindow *loggerWindow;

	RenderingStatsWindow *renderingStatsWindow;

	CameraPropertiesWindow *cameraPropertiesWindow;

	LayerPropertiesWindow *layerPropertiesWindow;

	ScreenShotWindow *screenShotWindow;

	ColorPickerWindow *colorPickerWindow;

	SpritePropertiesWindow *spritePropertiesWindow;

	BenchmarkingWindow * benchMarkingWindow;

	// Define all toolbars our editor can render

	LayerBar		*layerBar;
	MainToolBar		*mainToolBar;
	SpriteSelector  *spriteSelector;
	EditBar			*editBar;
	GameBar			*gameBar;

	bool showHints;

	IMGUIProvider();
	~IMGUIProvider();

	bool init();
	void draw();
	void Shutdown();
	void NewFrame();
	void InvalidateDeviceObjects();
	bool CreateDeviceObjects();
	void showImage(bool* opened);

	void addImGuiComponent(ImGUIComponent *toolBar);
	void renderImGuiComponents();
	ImGUIComponent* getToolBar(string toolBarName);

};

#endif