#ifndef BENCHMARKSTATE_H
#define BENCHMARKSTATE_H

#include "applicationstate.h"

class CBenchMarkState : public CApplicationState
{
public:
	void init(Application *app);
	
	void cleanup();

	void pause();
	
	void resume();

	void handleEvents();
	
	void update(float tpf);
	
	void draw();

	static CBenchMarkState* Instance()
	{
		return &m_BenchMarkState;
	}

protected:
	CBenchMarkState() { }

private:
	static  CBenchMarkState m_BenchMarkState;
	ImGuiIO io;
	float	rot = 0.0;
	bool	rotate = false;
	float	rotSpeed = 0.0;
	float	scl = 1.0;
	bool	scale = false;
	int		maxSprites = 100;
	int		renderMethod = 1;
	

};

#endif