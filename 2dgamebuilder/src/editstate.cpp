/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file introstate.cpp
*
*/

#include <stdio.h>
#include <glib2d/glib2d.h>
#include "editstate.h"
#include "globals.h"

CEditState CEditState::m_EditState;

point rot_point(float cx, float cy, float angle, point p)
{
	float s = sinf(angle);
	float c = cosf(angle);

	// translate point back to origin:
	p.x -= cx;
	p.y -= cy;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + cx;
	p.y = ynew + cy;
	return p;
}

void CEditState::init(Application *app)
{
	m_app = app;
	m_app->m_currentState = EDIT;
	ServiceLocator::getStateService()->setState(EDIT);
	io = ImGui::GetIO();
	TRACE(T_INF, "Init function for EditState finished");
	//myParticles.setup();
}

void CEditState::cleanup()
{
}

void CEditState::pause()
{
}

void CEditState::resume()
{
}

void CEditState::handleEvents()
{	
	float ellapsed = ImGui::GetIO().DeltaTime;

	//EditBar *editBar = (EditBar*)ServiceLocator::getImguiService()->getToolBar("EditBar");

	SubState subState = ServiceLocator::getStateService()->getSubState(); // get subState of editstate
	MoveType moveType = ServiceLocator::getStateService()->getMoveType(); // get moveType of editstate
	DrawType drawType = ServiceLocator::getStateService()->getDrawType(); // get drawType
	
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (glfwGetKey(gWin, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
		{
			ServiceLocator::getCameraService()->zoom_target += 0.02;
		}

		if (glfwGetKey(gWin, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
		{
			ServiceLocator::getCameraService()->zoom_target -= 0.02;
		}

		Sprite *currSprite = ServiceLocator::getGridService()->getCurrentSprite();

		
		switch (subState)
		{
		case EST_DRAWTILED:

			switch (drawType)
			{
			case DT_TILED_CLICKED:

				if (g_MouseWasPressed[LEFT_MOUSE_BUTTON])
				{
					currSprite->sprBatch->getCurrentLayer()->addSprite(*currSprite, FRONT);
				}
				if (g_MouseWasPressed[RIGHT_MOUSE_BUTTON])
				{
					currSprite->sprBatch->getCurrentLayer()->addSprite(*currSprite, BACK);
				}
				break;

			case DT_TILED_PRESSED:

				bool leftMouseIsPressed = glfwGetMouseButton(gWin, LEFT_MOUSE_BUTTON);
				bool rightMouseIsPressed = glfwGetMouseButton(gWin, RIGHT_MOUSE_BUTTON);

				if (leftMouseIsPressed || rightMouseIsPressed) 
				{
					bool collidingWithSelf = false;

					vector<Sprite*> collidingSpriteList = ServiceLocator::getGameService()->getColSpriteList(); //getCurrentLayer()->getCollidingSprites(currSprite->x, currSprite->y);

					if (collidingSpriteList.empty())
					{
						// No collisions with existing sprites so draw 
						if (leftMouseIsPressed)
						{
							currSprite->sprBatch->getCurrentLayer()->addSprite(*currSprite, FRONT);
						}
						else
						{
							currSprite->sprBatch->getCurrentLayer()->addSprite(*currSprite, BACK);
						}
					}
					else
					{
						// check if we are colliding and with what we are colliding

						vector<Sprite*>::iterator it;
						
						for (it = collidingSpriteList.begin(); it != collidingSpriteList.end(); it++)
						{
							Sprite *spr = *it;

							if (spr->name == currSprite->name)
							{
								// There is a collision so check if we try to draw on top of a sprite that has same name as our sprite
								collidingWithSelf = true;
								point p = ServiceLocator::getCameraService()->getScreenCoordinates(currSprite->x, currSprite->y);

								if ((floor(spr->x) == floor(p.x)) && (floor(spr->y) == floor(p.y)))
								{			
									// Colliding with our self on exactly the same position so there is no need to print our sprite
									collidingWithSelf = true;
								}
								else
								{
									TRACE(T_ERR, "Collision with self=false: (cPX=%f,cPY=%f) with (pX=%f,cY=%f)", floor(spr->x), floor(spr->y), floor(p.x), floor(p.y));
								}
							}
						}

						if (!collidingWithSelf)
						{
							if (leftMouseIsPressed)
							{
								currSprite->sprBatch->getCurrentLayer()->addSprite(*currSprite, FRONT);
							}
							else
							{
								currSprite->sprBatch->getCurrentLayer()->addSprite(*currSprite, BACK);
							}
						}
					}
				}
			}
			hdlRollerOperation(currSprite);
			break;

		case EST_DRAWFREEHAND:

			if (g_MouseWasPressed[LEFT_MOUSE_BUTTON])
			{
				currSprite->sprBatch->getCurrentLayer()->addSprite(*currSprite, FRONT);
			}
			if (g_MouseWasPressed[RIGHT_MOUSE_BUTTON])
			{
				currSprite->sprBatch->getCurrentLayer()->addSprite(*currSprite, BACK);
			}
			hdlRollerOperation(currSprite);
			break;

		case EST_DELETE:
			this->hdlErase();
			break;
		
		case EST_MOVEITEMS:
			
			break;
		
		case EST_MULTIPLE_SELECT:
			
			break;
		
		case EST_SINGLE_SELECT:
			this->hdlSingleSelect();
			break;

		case EST_BOX2D:
			this->hdlBox2dOperations();
			break;

		case EST_SQUARE:
			this->hdlSquareDrawOperation();
			break;
		}
	}
}

void CEditState::update(float tpf)
{
	//myParticles.update();
	
	//rot = rot + 2.0;
	float prevCamX = camX;
	float prevCamY = camY;

	MoveType moveType = ServiceLocator::getStateService()->getMoveType(); // get moveType of editstate
	
	Sprite *spr = ServiceLocator::getGridService()->getCurrentSprite();
	float speed = spr->sW;//*tpf;

	if (!ImGui::GetIO().WantCaptureMouse)
	{
		switch (moveType)
		{
		case MT_CAMERA:

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

			if (prevCamX != camX)
			{
				point cPos;
				cPos.x = camX;
				cPos.y = camY;

				TRACE(T_INF, "Camera is moving on X");
				//ServiceLocator::getGameService()->setLayerCameraPos(cPos);
			}
			break;

		case MT_LAYER:

			if (glfwGetKey(gWin, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				ServiceLocator::getGameService()->updateLayerPosition(VERTICAL,2);
			}

			if (glfwGetKey(gWin, GLFW_KEY_UP) == GLFW_PRESS)
			{
				ServiceLocator::getGameService()->updateLayerPosition(VERTICAL, -2);
			}

			if (glfwGetKey(gWin, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				ServiceLocator::getGameService()->updateLayerPosition(HORIZONTAL,2); 
			}

			if (glfwGetKey(gWin, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				ServiceLocator::getGameService()->updateLayerPosition(HORIZONTAL,-2);
			}
			break;

		case MT_PLAYER:

			break;
		}
	}
}

void CEditState::draw()
{

	SubState subState = ServiceLocator::getStateService()->getSubState();

	double xMouse, yMouse;
	int width, height;
	float prevZoom;

	CameraProvider *cam = ServiceLocator::getCameraService();

	glfwGetCursorPos(gWin, &xMouse, &yMouse);

	m_app->renderProvider->getWindowSize(&width, &height);

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
				g2dSetCoordXYZ(width / 2, (height / 2),-99.0);
				g2dSetScaleWH(480*cam->zoom,272*cam->zoom);
				g2dSetCropXY(cx, cy);
				g2dAdd();
				g2dEnd();
			}
			g2dSetGlobalScale(prevZoom);
		}

		if (ServiceLocator::getConfigurationService()->pEditor_drawpspbox)
		{
			g2dSetScissor((width / 2 - (480 * cam->zoom) / 2), (height / 2 - (272 * cam->zoom) / 2), 480 * cam->zoom, 272 * cam->zoom);
		}
		m_app->gameProvider->getCurrentLevel()->draw();
		
		g2dResetScissor();

		// draw debug lines for current layer items if parameter enabled
		if (ServiceLocator::getConfigurationService()->pEditor_drawdebuglines)
		{	
			//ServiceLocator::getLevelService()->getCurrentLayer()->drawDebug();
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

	if (mouseClicks == 2 && subState==EST_BOX2D)
	{
		g2dBeginLines(G2D_NULL);
		g2dSetColor(WHITE);
		g2dSetCoordXY(omX/ServiceLocator::getCameraService()->zoom, omY/ ServiceLocator::getCameraService()->zoom);
		g2dAdd();
		g2dSetCoordXY(xMouse / ServiceLocator::getCameraService()->zoom, yMouse / ServiceLocator::getCameraService()->zoom);
		g2dAdd();
		g2dEnd();
	}

	if (mouseClicks == 2 && subState == EST_SQUARE)
	{
				// first mouse click gives us the upper left corner of the rectangle
		float upX = omX / ServiceLocator::getCameraService()->zoom; 
		float upY = omY / ServiceLocator::getCameraService()->zoom;

		// second point we always assume is where the mouse is
		float w = (xMouse / ServiceLocator::getCameraService()->zoom) - upX;
		float h = (yMouse / ServiceLocator::getCameraService()->zoom) - upY;

		g2dBeginRects(ServiceLocator::getTextureService()->GetTexture(ServiceLocator::getImguiService()->spriteSelector->getCurrentSprite().name));
		g2dSetTexRepeat(true);
		g2dSetCoordMode(G2D_UP_LEFT);
		g2dSetCoordXY(upX,upY);
		g2dSetScaleWH(w,h);
		g2dSetCropWH(w, h); 
		g2dAdd();
		g2dEnd();

		// draw box while draggin mouse to draw rectangular area
		glLineWidth(2.0);
		g2dBeginLines(G2D_NULL);
			g2dSetColor(YELLOW);
			g2dSetCoordXY(omX / ServiceLocator::getCameraService()->zoom, omY / ServiceLocator::getCameraService()->zoom);
			g2dAdd();

			g2dSetCoordXY(xMouse / ServiceLocator::getCameraService()->zoom, omY / ServiceLocator::getCameraService()->zoom);
			g2dAdd();

			g2dSetCoordXY(xMouse / ServiceLocator::getCameraService()->zoom, omY / ServiceLocator::getCameraService()->zoom);
			g2dAdd();

			g2dSetCoordXY(xMouse / ServiceLocator::getCameraService()->zoom, yMouse / ServiceLocator::getCameraService()->zoom);
			g2dAdd();

			g2dSetCoordXY(xMouse / ServiceLocator::getCameraService()->zoom, yMouse / ServiceLocator::getCameraService()->zoom);
			g2dAdd();

			g2dSetCoordXY(omX / ServiceLocator::getCameraService()->zoom, yMouse / ServiceLocator::getCameraService()->zoom);
			g2dAdd();

			g2dSetCoordXY(omX / ServiceLocator::getCameraService()->zoom, yMouse / ServiceLocator::getCameraService()->zoom);
			g2dAdd();

			g2dSetCoordXY(omX / ServiceLocator::getCameraService()->zoom, omY / ServiceLocator::getCameraService()->zoom);
			g2dAdd();
		g2dEnd();
		glLineWidth(1.0);

		//		g2dSetCoordXY(xMouse / ServiceLocator::getCameraService()->zoom, yMouse / ServiceLocator::getCameraService()->zoom);

	}
}

void CEditState::hdlSquareDrawOperation()
{

	int width, height;
	m_app->renderProvider->getWindowSize(&width, &height);

	CameraProvider *cam = ServiceLocator::getCameraService();
	double xMouse, yMouse;

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

		if (mouseClicks == 1)
		{
			omX = xMouse;
			omY = yMouse;

			a.x = xMouse / cam->zoom + cam->x - (width / 2) / cam->zoom;
			a.y = yMouse / cam->zoom + cam->y - (height / 2) / cam->zoom;

			mouseClicks = 2;
		}

		if (mouseClicks == 3)
		{
			// draw pick second point			

			b.x = xMouse / cam->zoom + cam->x - (width / 2) / cam->zoom;
			b.y = yMouse / cam->zoom + cam->y - (height / 2) / cam->zoom;

			mouseClicks = 0;

			// first mouse click gives us the upper left corner of the rectangle
			float upX = (omX / ServiceLocator::getCameraService()->zoom);
			float upY = (omY / ServiceLocator::getCameraService()->zoom);

			// second point we always assume is where the mouse is
			float w = (xMouse / ServiceLocator::getCameraService()->zoom) - upX;
			float h = (yMouse / ServiceLocator::getCameraService()->zoom) - upY;

			Sprite *currSpr = new Sprite();
			currSpr->x = omX+((xMouse-omX)/2);
			currSpr->y = omY+((yMouse-omY)/2);
			currSpr->alpha = 255;
			currSpr->sH = h;
			currSpr->sW = w;
			currSpr->texRepeat = true;
			currSpr->texName = ServiceLocator::getImguiService()->spriteSelector->getCurrentSprite().name;
			currSpr->cW = w;
			currSpr->cH = h;
			TRACE(T_INF, "Adding TexFill Sprite at[%f,%f] with texture[%s]", currSpr->x, currSpr->y,currSpr->texName.c_str());
			ServiceLocator::getGameService()->getCurrentLayer()->addSprite(*currSpr, FRONT);
			delete currSpr;
		}
	}
}

void CEditState::hdlBox2dOperations()
{
	int width, height;
	m_app->renderProvider->getWindowSize(&width, &height);

	CameraProvider *cam = ServiceLocator::getCameraService();
	double xMouse, yMouse;
	
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

		if (mouseClicks == 1)
		{
			// draw pick first point			
			float angle = cam->rot * (b2_pi / 180);

			omX = xMouse;
			omY = yMouse;

			a.x = xMouse / cam->zoom + cam->x - (width  / 2) / cam->zoom;
			a.y = yMouse / cam->zoom + cam->y - (height / 2) / cam->zoom;

			//x1 = cosf(angle) * ((a.x) - cam->x) - sinf(angle) * ((a.y) - cam->y) + cam->x;
			//y1 = sinf(angle) * ((a.x) - cam->x) + cosf(angle) * ((a.y) - cam->y) + cam->y;

			//a.x = x1;
			//a.y = y1;

			mouseClicks = 2;
		}

		if (mouseClicks == 3)
		{
			// draw pick second point			

			float angle = cam->rot * (b2_pi / 180);

			b.x = xMouse / cam->zoom + cam->x - (width  / 2) / cam->zoom;
			b.y = yMouse / cam->zoom + cam->y - (height / 2) / cam->zoom;

			//x2 = cosf(angle) * ((b.x) - cam->x) - sinf(angle) * ((b.y) - cam->y) +cam->x;
			//y2 = sinf(angle) * ((b.x) - cam->x) + cosf(angle) * ((b.y) - cam->y) +cam->y;

			//b.x = x2;
			//b.y = y2;

			mouseClicks = 0;

			//ServiceLocator::getLevelService()->CreateLine(a.x, a.y, b.x, b.y);
		}
	}
}

void CEditState::hdlErase()
{
	/*
	* handle input when deleting selected sprite if roller up and down get next colliding sprite from list. if left button pressed
	* delete colliding sprite from list and layer
	*/

	static int currentSprite = 0;

	int i = 0;

	vector<Sprite*> colSpritesList = ServiceLocator::getGameService()->getColSpriteList(); //getCurrentLayer()->getCollidingSprites();

	vector<Sprite*>::iterator it = colSpritesList.begin();

	if (!colSpritesList.empty() && currentSprite == 0)
	{
		// if colliding sprite list not empty get first collidable to be highlighted
		m_colListSelectedSprite = *colSpritesList.begin();
	}
	else if (colSpritesList.empty())
	{
		m_colListSelectedSprite = NULL;
		currentSprite = 0;
	}

	if (g_MouseWasPressed[LEFT_MOUSE_BUTTON])
	{
		if (m_colListSelectedSprite)
		{
			TRACE(T_DEB, "About to mark for deletion sprite ID[%d][%s]", m_colListSelectedSprite->id, m_colListSelectedSprite->name.c_str());

			ServiceLocator::getGameService()->deleteColSprite(m_colListSelectedSprite); 

			colSpritesList.erase(colSpritesList.begin() + currentSprite);

			m_colListSelectedSprite = NULL;

			if (!colSpritesList.empty())
			{
				m_colListSelectedSprite = *colSpritesList.begin();
				currentSprite = 0;
			}
		}
	}

	if (!colSpritesList.empty())
	{
		int mouseWheel = ImGui::GetIO().MouseWheel;

		if (mouseWheel>0)
		{
			if (currentSprite >= colSpritesList.size() - 1)
			{
				currentSprite = 0;
			}
			else
			{
				currentSprite++;
			}

			for (it = colSpritesList.begin(); it != colSpritesList.end(); it++)
			{
				if (i == currentSprite)
				{
					m_colListSelectedSprite = *it;
					TRACE(T_DEB, "MouseWheel UP: Next Selected Sprite is number [%d] name[%s] id[%d]", currentSprite, m_colListSelectedSprite->name.c_str(), m_colListSelectedSprite->id);
				}
				i++;
			}
		}
		else if (mouseWheel<0)
		{
			if (currentSprite <= 0)
			{
				currentSprite = colSpritesList.size() - 1;
			}
			else
			{
				currentSprite--;
			}

			for (it = colSpritesList.begin(); it != colSpritesList.end(); it++)
			{
				if (i == currentSprite)
				{
					m_colListSelectedSprite = *it;
					TRACE(T_DEB, "MouseWheel DOWN: Next Selected Sprite is number [%d] name[%s] id[%d]", currentSprite, m_colListSelectedSprite->name.c_str(), m_colListSelectedSprite->id);
				}
				i++;
			}
		}

	}
}

void CEditState::hdlSingleSelect()
{
	/*
	* handle input when deleting selected sprite
	* if roller up and down get next colliding sprite from list. if left button pressed
	* delete colliding sprite from list and layer
	*/
	int width, height;
	ServiceLocator::getRenderService()->getWindowSize(&width, &height);

	static int currentSprite = 0;

	int i = 0;
	//Layer *currentLayer = ServiceLocator::getLevelService()->getCurrentLayer();
	Layer *currentLayer = ServiceLocator::getGameService()->getCurrentLayer();

	if (currentLayer != NULL)
	{
		vector<Sprite*> colSpritesList = ServiceLocator::getGameService()->getColSpriteList(); //currentLayer->getCollidingSprites();

		vector<Sprite*>::iterator it = colSpritesList.begin();

		if (!colSpritesList.empty() && currentSprite == 0)
		{
			// if colliding sprite list not empty get first collidable to be highlighted
			m_colListSelectedSprite = *colSpritesList.begin();
		}
		else if (colSpritesList.empty())
		{
			m_colListSelectedSprite = NULL;
			currentSprite = 0;
		}

		if (g_MouseWasPressed[LEFT_MOUSE_BUTTON])
		{
			if (m_colListSelectedSprite)
			{
				TRACE(T_DEB, "About to align grid on sprite ID[%d][%s]", m_colListSelectedSprite->id, m_colListSelectedSprite->name.c_str());

				ServiceLocator::getImguiService()->spriteSelector->setSpriteBatch(m_colListSelectedSprite->sprBatch);

				ServiceLocator::getGridService()->setItem(m_colListSelectedSprite);

				ServiceLocator::getGridService()->setGridRotation(m_colListSelectedSprite->rotation);

				//!! below is hardcoded should be on sprite !!!!!
				ServiceLocator::getGridService()->setGridRotationOrigin((width / 2) / ServiceLocator::getCameraService()->zoom, (height / 2) / ServiceLocator::getCameraService()->zoom);

				TRACE(T_DEB, "Set Grid Rotation Origin to [%f]x[%f]", m_colListSelectedSprite->x, m_colListSelectedSprite->y);

				//!!!!!!!!!!!!************* B U G ****************!!!!!!!!!!!!!!!
				camX = ((m_colListSelectedSprite->x) / ServiceLocator::getGameService()->getCurrentLayer()->GetHSpeed());
				camY = ((m_colListSelectedSprite->y) / ServiceLocator::getGameService()->getCurrentLayer()->GetVSpeed());
				//!!!!!!!!!!!!************* B U G ****************!!!!!!!!!!!!!!!

				m_colListSelectedSprite = NULL;

				if (!colSpritesList.empty())
				{
					m_colListSelectedSprite = *colSpritesList.begin();
					currentSprite = 0;
				}
			}
		}

		if (g_MouseWasPressed[RIGHT_MOUSE_BUTTON])
		{
			if (m_colListSelectedSprite)
			{
				TRACE(T_DEB, "About to align grid on sprite ID[%d][%s]", m_colListSelectedSprite->id, m_colListSelectedSprite->name.c_str());
				//point currPoint, newPoint1;
				//ServiceLocator::getGridService()->setItem(ServiceLocator::getImguiService()->spriteSelector->getSpriteBatch()->getSprite(m_colListSelectedSprite->name));


				//ServiceLocator::getGridService()->setItem(m_colListSelectedSprite);

				ServiceLocator::getGridService()->setGridItemRotation(m_colListSelectedSprite->rotation);
				ServiceLocator::getGridService()->setGridRotation(m_colListSelectedSprite->rotation);
				ServiceLocator::getGridService()->setGridItemScale(m_colListSelectedSprite->scale);

				ServiceLocator::getGridService()->setItem(m_colListSelectedSprite);

				//!! below is hardcoded should be on sprite !!
				ServiceLocator::getGridService()->setGridRotationOrigin((width / 2) / ServiceLocator::getCameraService()->zoom, (height / 2) / ServiceLocator::getCameraService()->zoom);

				TRACE(T_DEB, "Set Grid Rotation Origin to [%f]x[%f]", m_colListSelectedSprite->x, m_colListSelectedSprite->y);

				camX = ((m_colListSelectedSprite->x) / ServiceLocator::getGameService()->getCurrentLayer()->GetHSpeed());
				camY = ((m_colListSelectedSprite->y) / ServiceLocator::getGameService()->getCurrentLayer()->GetVSpeed());


				m_colListSelectedSprite = NULL;

				if (!colSpritesList.empty())
				{
					m_colListSelectedSprite = *colSpritesList.begin();
					currentSprite = 0;
				}
			}
		}

		if (!colSpritesList.empty())
		{
			if (ImGui::GetIO().MouseWheel > 0)
			{
				if (currentSprite >= colSpritesList.size() - 1)
				{
					currentSprite = 0;
				}
				else
				{
					currentSprite++;
				}

				for (it = colSpritesList.begin(); it != colSpritesList.end(); it++)
				{
					if (i == currentSprite)
					{
						m_colListSelectedSprite = *it;
						TRACE(T_DEB, "Next Selected Sprite is [%d][%s]", currentSprite, m_colListSelectedSprite->name.c_str());
					}
					i++;
				}
			}
			else if (ImGui::GetIO().MouseWheel < 0)
			{
				if (currentSprite <= 0)
				{
					currentSprite = colSpritesList.size() - 1;
				}
				else
				{
					currentSprite--;
				}

				for (it = colSpritesList.begin(); it != colSpritesList.end(); it++)
				{
					if (i == currentSprite)
					{
						m_colListSelectedSprite = *it;
						TRACE(T_DEB, "Next Selected Sprite is [%d][%s]", currentSprite, m_colListSelectedSprite->name.c_str());
					}
					i++;
				}
			}

		}
	}
}

void CEditState::hdlRollerOperation(Sprite *currSprite)
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