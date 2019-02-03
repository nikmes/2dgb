/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file introstate.cpp
*
*/

#include <stdio.h>
#include <glib2d/glib2d.h>
#include "physicsstate.h"
#include "globals.h"

CPhysicsState CPhysicsState::m_PhysicsState;

void CPhysicsState::init(Application *app)
{
	m_app = app;
	m_app->m_currentState = PHYSIC;
	ServiceLocator::getStateService()->setState(PHYSIC);
	io = ImGui::GetIO();
	mouseClicks = 0;
}

void CPhysicsState::cleanup()
{
}

void CPhysicsState::pause()
{
}

void CPhysicsState::resume()
{
}

void CPhysicsState::handleEvents()
{
	CameraProvider *cam = ServiceLocator::getCameraService();
	double xMouse, yMouse, omY, omX, x1, x2, y1, y2;
	point a,b;
	glfwGetCursorPos(gWin, &xMouse, &yMouse);

	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (g_MouseWasPressed[LEFT_MOUSE_BUTTON])
		{
			mouseClicks++;
		}
		if (g_MouseWasPressed[RIGHT_MOUSE_BUTTON])
		{
			
		}

		Sprite *currSprite = ServiceLocator::getGridService()->getCurrentSprite();

		if (mouseClicks == 1)
		{
			// draw pick first point			
			float angle = cam->rot * (b2_pi / 180);

			omX = xMouse;
			omY = yMouse;

			a.x = xMouse / cam->zoom + cam->x - (G2D_SCR_W / 2) / cam->zoom;
			a.y = yMouse / cam->zoom + cam->y - (G2D_SCR_H / 2) / cam->zoom;

			x1 = cosf(angle) * ((a.x) - cam->x) - sinf(angle) * ((a.y) - cam->y) + cam->x;
			y1 = sinf(angle) * ((a.x) - cam->x) + cosf(angle) * ((a.y) - cam->y) + cam->y;

			a.x = x1;
			a.y = y1;

			mouseClicks = 2;
		}

		if (mouseClicks == 3)
		{
			// draw pick second point			

			float angle = cam->rot * (b2_pi / 180);

			b.x = xMouse / cam->zoom + cam->x - (G2D_SCR_W / 2) / cam->zoom;
			b.y = yMouse / cam->zoom + cam->y - (G2D_SCR_H / 2) / cam->zoom;

			x2 = cosf(angle) * ((b.x) - cam->x) - sinf(angle) * ((b.y) - cam->y) + cam->x;
			y2 = sinf(angle) * ((b.x) - cam->x) + cosf(angle) * ((b.y) - cam->y) + cam->y;

			b.x = x2;
			b.y = y2;

			mouseClicks = 0;

			//ServiceLocator::getLevelService()->CreateLine(a.x, a.y, b.x, b.y);
		}
	}
	
	if (mouseClicks == 2)
	{
		g2dBeginLines(G2D_NULL);
		g2dSetColor(WHITE);
		g2dSetCoordXY(omX, omY);
		g2dAdd();
		g2dSetCoordXY(xMouse, yMouse);
		g2dAdd();
		g2dEnd();
	}
}

void CPhysicsState::update(float tpf)
{
	float speed = 6;

	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (glfwGetKey(gWin, GLFW_KEY_UP) == GLFW_PRESS)
		{
			camY -= speed;
		}

		if (glfwGetKey(gWin, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			camY += speed;
		}

		if (glfwGetKey(gWin, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			camX += speed;
			cx += 1.0;
		}

		if (glfwGetKey(gWin, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			camX -= speed;
			cx -= 1.0;
		}
	}
}

void CPhysicsState::draw()
{
	double xMouse, yMouse;
	int width, height;
	float prevZoom;
	m_app->renderProvider->getWindowSize(&width, &height);

	glfwGetCursorPos(gWin, &xMouse, &yMouse);
	static float rotSpeed = 0.0;
	//static point pos[4] = { (1 * width / 4,height / 4),(2 * width / 4,1 * height / 4),(2 * width / 4,2 * height / 4),(1 * width / 4, 2 * height / 4) };
	static point pos[4]; // = { (1 * width / 4,height / 4),(2 * width / 4,1 * height / 4),(2 * width / 4,2 * height / 4),(1 * width / 4, 2 * height / 4) };
	static float wind_direction = -1.0;
	static float wind_strength = 1;
	static float rot = 0;
	static bool  reverse = false;
	static point cord[2];// = { (15,-5),(15,-5) };

	pos[0].x = 1 * width / 4;
	pos[0].y = 1 * height / 4;

	pos[1].x = 2 * width / 4;
	pos[1].y = 1 * height / 4;

	pos[2].x = 2 * width / 4;
	pos[2].y = 2 * height / 4;

	pos[3].x = 1 * width / 4;
	pos[3].y = 2 * height / 4;


	cord[0].x = 15;
	cord[0].y = -5;

	cord[1].x = 15;
	cord[1].y = -5;
	
	if (m_app->gameProvider->gameIsCreated())
	{
		string background = ServiceLocator::getGameService()->m_levelStaticBackground;

		// if level has a background display it first

		if (!background.empty() && ServiceLocator::getGameService()->m_levelStaticBackgroundIsVisible)
		{
			g2dGetGlobalScale(&prevZoom);
			g2dSetGlobalScale(1.0);
			g2dBeginRects(m_app->textureProvider->GetTexture(background));
			{
				g2dSetTexRepeat(true);
				g2dSetCoordMode(G2D_CENTER);
				g2dSetCoordXYZ(width / 2, (height / 2), -99.0);
				g2dSetScaleWH(width * ServiceLocator::getCameraService()->zoom, height * ServiceLocator::getCameraService()->zoom);
				g2dSetCropXY(cx, cy);
				g2dAdd();
				g2dEnd();
			}
			g2dSetGlobalScale(prevZoom);
		}

		m_app->gameProvider->getCurrentLevel()->draw();

		if (ServiceLocator::getConfigurationService()->pEditor_drawdebuglines)
		{
			ServiceLocator::getGameService()->getCurrentLayer()->drawDebug();
		}
	}

	if (m_colListSelectedSprite)
	{
		float glScale = 1.0;
		g2dGetGlobalScale(&glScale);
		g2dResetGlobalScale();
		m_colListSelectedSprite->DrawDebugForDelete(RED);
		g2dSetGlobalScale(glScale);
	}

	// draw box2d debug lines if level is loaded and option is enabled

	if (ServiceLocator::getGameService()->gameIsCreated())
	{
		if (ServiceLocator::getConfigurationService()->pEditor_drawbox2ddebuglines)
		{
			ServiceLocator::getGameService()->getCurrentLevel()->World->DrawDebugData();
		}
	}

	float currScale;
	g2dGetGlobalScale(&currScale);
	g2dSetGlobalScale(1.f);
	g2dResetScale();

	double time = glfwGetTime();

	rotSpeed = sin(time + (pos[0].x + pos[0].y)*0.1f) * wind_strength;

	if ((rot += rotSpeed) > 360) rot -= 360;
	/*
	if (reverse)
	rot -= 1;
	else
	rot += 1;

	if (rot > 180)
	{
	reverse = true;
	}

	if (rot < 0)
	{
	reverse = false;
	}*/


	// 
	//		g2dBeginQuads(ServiceLocator::getTextureService()->GetTextureFromFileSystem("resources/Tree_22.png"));
	g2dBeginQuads(ServiceLocator::getTextureService()->GetTextureFromFileSystem("resources/grass.png"));
	{
		g2dSetCoordXY(pos[0].x, pos[0].y);
		g2dPush();
		g2dSetRotation(rot);
		g2dSetCoordXYRelative(cord[0].x, cord[0].y);
		g2dAdd();
		g2dPop();

		g2dSetCoordXY(pos[1].x, pos[1].y);
		g2dPush();
		g2dSetRotation(rot);
		g2dSetCoordXYRelative(cord[1].x, cord[1].y);
		g2dAdd();
		g2dPop();

		g2dSetCoordXY(pos[2].x, pos[2].y);
		g2dAdd();

		g2dSetCoordXY(pos[3].x, pos[3].y);
		g2dAdd();

		g2dEnd();
	}

	g2dBeginRects(NULL);
	{
		g2dSetColor(RED);
		g2dSetCoordMode(G2D_CENTER);
		g2dSetCoordXY(pos[0].x, pos[0].y);
		g2dPush();
		g2dSetRotation(rot);
		g2dSetCoordXYRelative(cord[0].x, cord[0].y);
		g2dSetScaleWH(8, 8);
		g2dAdd();
		g2dPop();

		g2dSetCoordXY(pos[1].x, pos[1].y);
		g2dPush();
		g2dSetRotation(rot);
		g2dSetCoordXYRelative(cord[1].x, cord[1].y);
		g2dSetScaleWH(8, 8);
		g2dAdd();
		g2dPop();

		g2dEnd();
	}

	//	double time = glfwGetTime();

	//	float wind = 0;

	//	wind = sin(time + (pos[0].x + pos[0].y)*0.1f)*wind_strength*wind_direction;

	//	ImGui::DragFloat("Wind direction", &wind_direction,0.1);
	
	ImGui::DragFloat("Wind strength", &wind_strength, 0.1);
	ImGui::DragFloat("Rotation Speed", &rotSpeed, 0.1);
	ImGui::DragFloat("leftX", &cord[0].x, 1);
	ImGui::DragFloat("leftY", &cord[0].y, 1);
	ImGui::DragFloat("rightX", &cord[1].x, 1);
	ImGui::DragFloat("rightY", &cord[1].y, 1);


	//ImGui::Text("Pos: [%f-%f]", pos[0].x,pos[0].y);
	//ImGui::Text("Wind1: %f", wind);
	//pos[0].x += wind;
	//pos[0].y += wind;

	//time = glfwGetTime();
	//wind = sin(time + (pos[1].x + pos[1].y)*0.1f)*wind_strength*wind_direction;
	//ImGui::Text("Wind2: %f", wind);

	//pos[1].x += wind;
	//pos[1].y += wind;
	g2dSetGlobalScale(currScale);

}

void CPhysicsState::hdlErase()
{

}

void CPhysicsState::hdlSingleSelect()
{

}

void CPhysicsState::hdlRollerOperation(Sprite *currSprite)
{
	RollerOperation rollerOperation = ServiceLocator::getStateService()->getRollerOperation(); // get roller operation

	switch (rollerOperation)
	{
	case RO_SCALE_ITEM:
		currSprite->setScale(currSprite->scale + ImGui::GetIO().MouseWheel *0.01);
		break;

	case RO_ROTATE_ITEM:
		currSprite->rotation += ImGui::GetIO().MouseWheel * 0.01;
		break;

	case RO_ADJUST_TRANSPARENCY:
		currSprite->alpha += ImGui::GetIO().MouseWheel * 10;
		break;

	case RO_FLIP_HORIZONTALLY:
		break;

	case RO_FLIP_VERTICALLY:
		break;
	}
}