#ifndef STATEPROVIDER_H
#define STATEPROVIDER_H
#include "definitions.h"

class Application;

class StateProvider
{
private:

	State m_state;
	SubState m_subState;
	MoveType m_moveType;
	DrawType m_drawType;
	RollerOperation m_rollerOperation;

public:
	
	Application *m_app;

	StateProvider(Application *app);

	~StateProvider();

	void setState(State state);

	State getState();

	void setSubState(SubState subState);

	SubState getSubState();

	void setMoveType(MoveType moveType);

	MoveType getMoveType();

	DrawType getDrawType();

	void setDrawType(DrawType drawType);

	RollerOperation getRollerOperation();

	void setRollerOperation(RollerOperation rollerOperation);
};

#endif
