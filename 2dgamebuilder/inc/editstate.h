#ifndef EDITSTATE_H
#define EDITSTATE_H

#include "applicationstate.h"
//#include "particles.h"

class CEditState : public CApplicationState
{
public:
	void init(Application *app);
	void cleanup();

	void pause();
	void resume();

	void handleEvents();
	void update(float tpf);
	void draw();

	static CEditState* Instance()
	{
		return &m_EditState;
	}

	//Particles myParticles;

protected:
	CEditState() { }

private:
	static CEditState m_EditState;
	
	Sprite *m_colListSelectedSprite;
	ImGuiIO io;
	// Operations 
	void hdlErase();
	void hdlSingleSelect();
	void hdlRollerOperation(Sprite *currSprite);
	void hdlBox2dOperations();
	void hdlSquareDrawOperation();
	float rot=0.0;
	float cx = 0, cy = 0;
	int mouseClicks = 0;
	double omX, omY;
	point a, b;
	float x1, x2, y1, y2;
};

#endif