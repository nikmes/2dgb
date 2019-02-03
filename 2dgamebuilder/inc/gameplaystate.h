#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H

#include "applicationstate.h"
#include <stack>

class CGamePlayState : public CApplicationState
{
public:
	void init(Application *app);
	void cleanup();

	void pause();
	void resume();

	void handleEvents();
	void update(float tpf);
	void draw();

	static CGamePlayState* Instance()
	{
		return &m_GamePlayState;
	}

protected:
	CGamePlayState() { }

private:

	static CGamePlayState m_GamePlayState;
	double timepassed;
	int cx, cy;

};

#endif