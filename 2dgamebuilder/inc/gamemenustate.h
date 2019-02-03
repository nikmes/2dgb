#ifndef GAMEMENUSTATE_H
#define GAMEMENUSTATE_H

#include "applicationstate.h"
#include <stack>

class CGameMenuState : public CApplicationState
{
public:
	void init(Application *app);
	void cleanup();

	void pause();
	void resume();

	void handleEvents();
	void update(float tpf);
	void draw();

	static CGameMenuState* Instance()
	{
		return &m_GameMenuState;
	}

protected:
	CGameMenuState() { }

private:

	g2dTexture *currentScreen;			// the current texture that we are rendering
	static CGameMenuState m_GameMenuState;
	double timepassed;
};

#endif