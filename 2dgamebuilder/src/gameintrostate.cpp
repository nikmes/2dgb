/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file introstate.cpp
*
*/

#include <stdio.h>
#include "gameintrostate.h"
#include "gamemenustate.h"
#include "gameplaystate.h"
#include <glib2d/glib2d.h>
#include "globals.h"

CGameIntroState CGameIntroState::m_GameIntroState;

void CGameIntroState::init(Application *app)
{
	m_app = app;

	TRACE(T_INF, "INTRO state initialized");

	m_app->m_currentState = GAMEINTRO;

	ServiceLocator::getStateService()->setState(INTRO);

	screens.push(ServiceLocator::getTextureService()->GetTexture("intrologos/2dgbLogo.png"));
	screens.push(ServiceLocator::getTextureService()->GetTexture("intrologos/stc.png"));

	currentScreen = screens.top();

	screens.pop();

	color = 0;

	fader = 64;

	glfwSetInputMode(gWin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	TRACE(T_INF, "Init function for Introstate finished");

	this->timepassed = .0f;
}

void CGameIntroState::cleanup()
{
	ServiceLocator::getTextureService()->ReleaseTexture("intrologos/stc.png");
	ServiceLocator::getTextureService()->ReleaseTexture("intrologos/2dgbLogo.png");

	color = 0;

	fader = 64;

	while (!screens.empty())
	{
		screens.top();
		screens.pop();
	}
}

void CGameIntroState::pause()
{
}

void CGameIntroState::resume()
{
}

void CGameIntroState::handleEvents()
{
}

void CGameIntroState::update(float tpf)
{
	double ellapsed = 0.05; //ServiceLocator::getHelpService()->GetDeltaTime();

	this->timepassed += ellapsed;							// update time passed since we started the introstate

	if (this->timepassed > 1)								// if 1 second passed
	{
		if (screens.size() >= 0)							// there are more screens to display and 0 is the last one we poped out from the stack
		{
			color = color + fader * ellapsed;				// start transition from black to white

			if (color>255)									// if color is completely bright	
			{
				fader = -64;									// fade back to black
				color = 255;								// initial color is white
			}

			if (color<0)									// if color is completely dark	
			{
				if (screens.size() == 0)
				{
					m_app->changeState(CGamePlayState::Instance());
				}
				else
				{
					currentScreen = screens.top();			
					screens.pop();
					fader = 64;	// fade to white
					color = 0.0f;	// initial color is black
					this->timepassed = 0.0f;
				}
			}
		}
	}
}

void CGameIntroState::draw()
{

	int width, height;
	
	ServiceLocator::getRenderService()->getWindowSize(&width, &height);

	g2dBeginRects(currentScreen);
	{
		g2dSetCoordMode(G2D_CENTER);

		g2dSetColor(ServiceLocator::getHelpService()->convertRGBtoColor(color, color, color));

		g2dSetCoordXY(width / 2, height / 2);

		g2dAdd();

		g2dEnd();
	}
}