#ifndef EDITBAR_H
#define EDITBAR_H

#include "imguicomponent.h"

using namespace std;

class EditBar : public ImGUIComponent
{
private:

	vector<textureAtlasItem> rollerIcons;
	vector<textureAtlasItem> alignIcons;

public:

	~EditBar();

	EditBar(TextureAtlas *textureAtlas, string name);

	void run(bool* opened);

	void addIcons();

};

#endif