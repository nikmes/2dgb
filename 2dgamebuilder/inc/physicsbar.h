#ifndef PHYSICSBAR_H
#define PHYSICSBAR_H

#include "imguicomponent.h"

using namespace std;

class PhysicsBar : public ImGUIComponent
{

private:

	TextureAtlas *m_textureAtlas;

	vector<textureAtlasItem> icons;

public:

	~PhysicsBar();

	PhysicsBar(TextureAtlas *textureAtlas, string name);

	void run(bool* opened);

	void addIcons();
};

#endif