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

CGamePlayState CGamePlayState::m_GamePlayState;

void CGamePlayState::init(Application *app)
{
	m_app = app;

	m_app->m_currentState = GAMEPLAY;

	ServiceLocator::getStateService()->setState(INTRO);

	this->timepassed = .0f;
	cx = 512*100;
	cy = 512*100;
}

void CGamePlayState::cleanup()
{

}

void CGamePlayState::pause()
{
}

void CGamePlayState::resume()
{
}

void CGamePlayState::handleEvents()
{
#ifdef PLATFORM_PSP

	float speed = 6;

	SceCtrlData *pad = ServiceLocator::getInputService()->getEvents();

	if (pad->Buttons & PSP_CTRL_LEFT)
	{
		camX -= speed;
		cx -= 1.0;
	}

	if (pad->Buttons & PSP_CTRL_RIGHT) 
	{
		camX += speed;
		cx += 1.0;
	}
	
	if (pad->Buttons & PSP_CTRL_UP)
	{
		camY -= speed;
		//cy -= 1.0;
	}
	
	if (pad->Buttons & PSP_CTRL_DOWN)
	{
		camY += speed;
		//cy += 1.0;
	}

	if (pad->Buttons & PSP_CTRL_LTRIGGER)
	{
		ServiceLocator::getCameraService()->rot_target += 0.1;
	}
	
	if (pad->Buttons & PSP_CTRL_RTRIGGER)
	{
		ServiceLocator::getCameraService()->rot_target -= 0.1;
	}

	if (pad->Buttons & PSP_CTRL_SQUARE)
	{
		ServiceLocator::getCameraService()->zoom_target -= 0.1;
	}

	if (pad->Buttons & PSP_CTRL_CIRCLE)
	{
		ServiceLocator::getCameraService()->zoom_target += 0.1;
	}

	if (pad->Buttons & PSP_CTRL_SELECT)
	{
		ServiceLocator::getCameraService()->zoom_target = 1.0;
		ServiceLocator::getCameraService()->rot_target = 0;
	}



#endif
}

void CGamePlayState::update(float tpf)
{
	//double ellapsed = ServiceLocator::getHelpService()->GetDeltaTime();

	//this->timepassed += ellapsed;	
}

void CGamePlayState::draw()
{
	int width, height;
	float prevZoom;
	m_app->renderProvider->getWindowSize(&width, &height);

	if (m_app->gameProvider->gameIsCreated())
	{
		string background = ServiceLocator::getGameService()->m_levelStaticBackground;

		m_app->gameProvider->getCurrentLevel()->draw(); // draw all layers of the level

		if (ServiceLocator::getConfigurationService()->pEditor_drawdebuglines)
		{
			//ServiceLocator::getLevelService()->getCurrentLayer()->drawDebug();
		}
	}

	// draw box2d debug lines if level is loaded and option is enabled

	if (m_app->gameProvider->gameIsCreated())
	{
		if (ServiceLocator::getConfigurationService()->pEditor_drawbox2ddebuglines)
		{
			//ServiceLocator::getLevelService()->World->DrawDebugData();
		}
	}
}