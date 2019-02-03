/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file introstate.cpp
*
*/

#include <stdio.h>
#include "introstate.h"
#include "editstate.h"
#include <glib2d/glib2d.h>
#include "globals.h"

CIntroState CIntroState::m_IntroState;

void CIntroState::init(Application *app)
{

	rp = ServiceLocator::getRenderService();

	ImGuiIO& io = ImGui::GetIO();

	m_app = app;

	TRACE(T_INF, "INTRO state initialized");

	m_app->m_currentState = INTRO;

	ServiceLocator::getStateService()->setState(INTRO);

	screens.push(ServiceLocator::getTextureService()->GetTextureFromFileSystem("media/intrologos/2dgbLogo.png"));

	screens.push(ServiceLocator::getTextureService()->GetTextureFromFileSystem("media/intrologos/stc.png"));

	currentScreen = screens.top();

	screens.pop();

	color = 0;

	fader = 4;

	timepassed = io.DeltaTime;

	glfwSetInputMode(gWin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	TRACE(T_INF,"Init function for Introstate finished");

}

void CIntroState::cleanup()
{
	ServiceLocator::getTextureService()->ReleaseTexture("media/intrologos/stc.png");

	ServiceLocator::getTextureService()->ReleaseTexture("media/intrologos/2dgbLogo.png");

	color = 0;

	fader = 4;

	while (!screens.empty())
	{
		screens.top();
		screens.pop();
	}

	glfwSetInputMode(gWin, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	TRACE(T_INF, "Clean up function for Introstate finished");
}

void CIntroState::pause()
{
}

void CIntroState::resume()
{
}

void CIntroState::handleEvents()
{
	if (glfwGetKey(gWin, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		m_app->changeState(CEditState::Instance());
	}
}

void CIntroState::update(float tpf)
{
	float ellapsed = ImGui::GetIO().DeltaTime;
	
	this->timepassed += ellapsed * 1000;				// update time passed since we started the introstate

	if (this->timepassed > 10)							// if 1 second passed
	{
		if (screens.size() >= 0)						// there are more screens to display and 0 is the last one we poped out from the stack
		{
			color = color + fader;						// start transition from black to white

			if (color>255)								// if color is completely bright	
			{
				fader = -4;								// fade back to black
				color = 255;							// initial color is white
			}

			if (color<0)								// if color is completely dark	
			{
				if (screens.size() == 0)
				{
					m_app->changeState(CEditState::Instance());
				}
				else
				{
					currentScreen = screens.top();
					screens.pop();
					fader = 4;	// fade to white
					color = 0;	// initial color is black
					this->timepassed = .0f;
				}
			}
		}
	}
}

void CIntroState::draw()
{
	int width,height;
	
	rp->getWindowSize(&width, &height);
	
	rp->g2dBeginRects(currentScreen);

		rp->g2dSetCoordMode(G2D_CENTER);

		rp->g2dSetColor(ServiceLocator::getHelpService()->convertRGBtoColor(color, color, color));

		rp->g2dSetCoordXY(width/2,height/2);
	
		rp->g2dAdd();

	rp->g2dEnd();
}