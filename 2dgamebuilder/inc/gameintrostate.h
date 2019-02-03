#ifndef GAMEINTROSTATE_H
#define GAMEINTROSTATE_H

#include "applicationstate.h"
#include <stack>

class CGameIntroState : public CApplicationState
{
public:
	void init(Application *app);
	void cleanup();

	void pause();
	void resume();

	void handleEvents();
	void update(float tpf);
	void draw();

	static CGameIntroState* Instance()
	{
		return &m_GameIntroState;
	}

protected:
	CGameIntroState() { }

private:

	g2dTexture *currentScreen;			// the current texture that we are rendering
	stack <g2dTexture*> screens;		// a stack holding all textures we will render
	static CGameIntroState m_GameIntroState;
	float color;
	float fader;
	double timepassed;
};

#endif