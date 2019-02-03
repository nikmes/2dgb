#ifndef PHYSICSSTATE_H
#define PHYSICSSTATE_H

#include "applicationstate.h"

class CPhysicsState : public CApplicationState
{
public:
	void init(Application *app);
	void cleanup();

	void pause();
	void resume();

	void handleEvents();
	void update(float tpf);
	void draw();

	static CPhysicsState* Instance()
	{
		return &m_PhysicsState;
	}

	//Particles myParticles;

protected:
	CPhysicsState() { }

private:
	static CPhysicsState m_PhysicsState;

	Sprite *m_colListSelectedSprite;
	ImGuiIO io;
	// Operations 
	void hdlErase();
	void hdlSingleSelect();
	void hdlRollerOperation(Sprite *currSprite);
	float rot = 0.0;
	float cx = 0, cy = 0;
	int mouseClicks = 0;
};

#endif