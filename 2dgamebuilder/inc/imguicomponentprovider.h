#ifndef IMGUICOMPONENT_H
#define IMGUICOMPONENT_H

#include <map>
#include "imguicomponent.h"

using namespace std;

class ImguiComponentProvider
{
private:

	map<string, ImGUIComponent*> imguiComponents;

public:

	ImguiComponentProvider();

	~ImguiComponentProvider();

	void addImguiComponent(ImGUIComponent *imguiComponent);

	void render();

	ImGUIComponent* getImguiComponent(string imguiComponentName);

	bool colorPicker4(float* col, bool show_alpha); // Color picker component to select color from pallette

	void runPNGSelect(bool* opened);
};

#endif
