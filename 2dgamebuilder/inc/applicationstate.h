#ifndef APPLICATIONSTATE_H
#define APPLICATIONSTATE_H

#include "application.h"

class CApplicationState
{
public:
	virtual void init(Application* app) = 0;

	virtual void cleanup() = 0;

	virtual void pause() = 0;

	virtual void resume() = 0;

	virtual void handleEvents() = 0;

	virtual void update(float tpf) = 0;

	virtual void draw() = 0;

	void changeState(CApplicationState* state)
	{
		m_app->changeState(state);
	}

protected:

	CApplicationState() { }

	Application *m_app;
};

#endif