#ifndef GAMEBAR_H
#define GAMEBAR_H

#include "imguicomponent.h"

using namespace std;

class GameBar : public ImGUIComponent
{
private:

	vector<textureAtlasItem> rollerIcons;

public:

	~GameBar();

	GameBar(TextureAtlas *textureAtlas, string name);

	void run(bool* opened);

	void addIcons();
	
	void gameProperties(bool *opened);

	void gameResources(bool *opened);

	// GUI Dialogues
	
	bool newGameDialogue();

};


#endif