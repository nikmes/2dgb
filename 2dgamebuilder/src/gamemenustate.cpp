/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file gamemenustate.cpp
*
*/

#include <stdio.h>
#include "gameplaystate.h"
#include "gamemenustate.h"
#include <glib2d/glib2d.h>
#include "globals.h"

CGameMenuState CGameMenuState::m_GameMenuState;

void CGameMenuState::init(Application *app)
{
	m_app = app;

	TRACE(T_INF, "GAMEMENU state initialized");

	m_app->m_currentState = GAMEMENU;

	ServiceLocator::getStateService()->setState(INTRO);

	currentScreen = ServiceLocator::getTextureService()->GetTexture("media/2dgbLogo_psp.png");

	TRACE(T_INF, "Init function for GameMenu finished");

	this->timepassed = .0f;
}

void CGameMenuState::cleanup()
{
	ServiceLocator::getTextureService()->ReleaseTexture("media/2dgbLogo_psp.png");
}

void CGameMenuState::pause()
{
}

void CGameMenuState::resume()
{
}

void CGameMenuState::handleEvents()
{
}

void CGameMenuState::update(float tpf)
{
	double ellapsed = ServiceLocator::getHelpService()->GetDeltaTime();

	this->timepassed += ellapsed;							// update time passed since we started the introstate
}

void CGameMenuState::draw()
{
	int width, height;

	ServiceLocator::getRenderService()->getWindowSize(&width, &height);

	g2dBeginRects(currentScreen);

	g2dSetCoordMode(G2D_CENTER);

	g2dSetCoordXY(width / 2, height / 2);

	g2dAdd();

	g2dEnd();

}