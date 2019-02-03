#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "applicationstate.h"
#include <stack>

class CIntroState : public CApplicationState
{
public:
	void init(Application *app);
	void cleanup();

	void pause();
	void resume();

	void handleEvents();
	void update(float tpf);
	void draw();

	static CIntroState* Instance()
	{
		return &m_IntroState;
	}

protected:
	CIntroState() { }

private:

	g2dTexture *currentScreen;			// the current texture that we are rendering
	stack <g2dTexture*> screens;		// a stack holding all textures we will render
	static CIntroState m_IntroState;
	float color;
	float fader;
	float timepassed;
	RenderProvider *rp;
};

#endif