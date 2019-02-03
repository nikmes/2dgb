#include "stateprovider.h"
#include "applicationstate.h"

StateProvider::StateProvider(Application *app)
{
	m_app = app;
}

StateProvider::~StateProvider()
{

}

void StateProvider::setState(State state)
{
	m_state = state;
}

void StateProvider::setSubState(SubState subState)
{
	m_subState = subState;
}

State StateProvider::getState()
{
	return m_state;
}

SubState StateProvider::getSubState()
{
	return m_subState;
}

void StateProvider::setMoveType(MoveType moveType)
{
	m_moveType = moveType;
}

MoveType StateProvider::getMoveType()
{
	return m_moveType;
}

DrawType StateProvider::getDrawType()
{
	return m_drawType;
}

void StateProvider::setDrawType(DrawType drawType)
{
	m_drawType = drawType;
}

RollerOperation StateProvider::getRollerOperation()
{
	return m_rollerOperation;
}

void StateProvider::setRollerOperation(RollerOperation rollerOperation)
{
	m_rollerOperation = rollerOperation;
}