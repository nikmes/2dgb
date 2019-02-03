#include "servicelocator.h"
#include "layer.h"

int Layer::next_id = 1000;

Layer::Layer()
{
	id = Layer::next_id++;
	TRACE(T_DEB, "Initialize Layer ID to [%d]", next_id);
	totalSprites = 0;
	cam = ServiceLocator::getCameraService();
	m_active = true;											// is this the active layer that we are currently editing?
}

Layer::Layer(string name, float hSpeed, float vSpeed, float zDepth)
{
	Layer::next_id++;
	this-> id = Layer::next_id;
	this->name = name;
	this->alpha = 255;
	this->color = WHITE;
	this->horizSpeed = hSpeed;
	this->vertSpeed = vSpeed;
	this->totalSprites = 0;
	this->visible = true;
	this->zDepth = zDepth;

	TRACE(T_DEB, "Layer created with below details:");
	TRACE(T_DEB, "   id               = [%d]", id);
	TRACE(T_DEB, "   name             = [%s]", this->name.c_str());
	TRACE(T_DEB, "   alpha            = [%d]", alpha);
	TRACE(T_DEB, "   color            = [%d]", color);
	TRACE(T_DEB, "   horizontal speed = [%f]", horizSpeed);
	TRACE(T_DEB, "   vertical speed   = [%f]", vertSpeed);
	TRACE(T_DEB, "   depth            = [%f]", this->zDepth);

	cam = ServiceLocator::getCameraService();
	
	m_active = true;											// is this the active layer that we are currently editing?
}

Layer::Layer(string name, float hSpeed, float vSpeed, float zDepth, int id)
{
	this->id = id;
	Layer::next_id = id;
	this->name = name;
	this->alpha = 255;
	this->color = WHITE;
	this->horizSpeed = hSpeed;
	this->vertSpeed = vSpeed;
	this->totalSprites = 0;
	this->visible = true;
	this->zDepth = zDepth;

	TRACE(T_DEB, "Layer created with below details:");
	TRACE(T_DEB, "   id               = [%d]", id);
	TRACE(T_DEB, "   name             = [%s]", this->name.c_str());
	TRACE(T_DEB, "   alpha            = [%d]", alpha);
	TRACE(T_DEB, "   color            = [%d]", color);
	TRACE(T_DEB, "   horizontal speed = [%f]", horizSpeed);
	TRACE(T_DEB, "   vertical speed   = [%f]", vertSpeed);
	TRACE(T_DEB, "   depth            = [%f]", this->zDepth);

	cam = ServiceLocator::getCameraService();
	m_active = true;											// is this the active layer that we are currently editing?
}

int Layer::getTotalSprites()
{
	return this->totalSprites;
}

Layer::~Layer()
{
	// default destructor

	TRACE(T_DEB,"Layer destructor called!");

	spriteList.clear();

	ServiceLocator::getTextureService()->ReleaseTexture(spriteBatch->m_textureFileName);
}

void Layer::addSprite(Sprite spr, layerPostion pos)
{
	/*
	* to add a sprite to the layer
	*/
	float maxz = .0;
	float minz = .0;

	spr.id = ServiceLocator::getHelpService()->generateId();

	TRACE(T_INF, "Adding sprite to layer ID[%d] Name[%s]", this->id, this->name.c_str());
	TRACE(T_INF, " Sprite Properties: ID[%d] Z[%f]",spr.id,spr.m_z);

	int height, width;
	
	ServiceLocator::getRenderService()->getWindowSize(&width, &height);
	
	CameraProvider *cam = ServiceLocator::getCameraService();

	float x, y;

	x = spr.x / cam->zoom + cam->x*this->horizSpeed - (width / 2) / cam->zoom;

	y = spr.y / cam->zoom + cam->y*this->vertSpeed - (height / 2) / cam->zoom;

	spr.sX = spr.x;
	spr.sY = spr.y;
	spr.sHeight = height;
	spr.sWidth = width;
	spr.sCamZoom = cam->zoom;
	spr.sCamX = cam->x;
	spr.sCamY = cam->y;

	vector<Sprite*> colSprites = ServiceLocator::getGameService()->getColSpriteList();

	if (!colSprites.empty())
	{
		vector<Sprite*>::iterator it = colSprites.begin();
	
		maxz = (*it)->m_z;
		minz = (*it)->m_z;

		for (it = colSprites.begin(); it != colSprites.end(); it++)
		{
			if ((*it)->m_z > maxz) maxz = (*it)->m_z;
			if ((*it)->m_z < minz) minz = (*it)->m_z;
		}
	}


	switch (pos)
	{
		case FRONT:
			spr.setPosition(x, y,maxz+0.01);			// this will go. need to get all colliding sprites and get smaller z and make it slightly smaller
			spriteList.push_back(spr);
			break;

		case BACK:
			spr.setPosition(x, y,minz-0.01);
			spriteList.push_back(spr);
//			spriteList.insert(spriteList.begin(), spr);
			break;

		default:
			spr.setPosition(x, y,maxz+0.01);
			spriteList.push_back(spr);
	}

	totalSprites++;
	TRACE(T_INF, "Before return from addSprite");

}

void Layer::removeSprite()
{
	// ro remove a sprite from the layer
}

void Layer::setVerticalSpeed(float vSpeed)
{

	// to set the vertical speed of the scroll

	vertSpeed = vSpeed;

}

void Layer::setHorizontalSpeed(float hSpeed)
{

	// to set the horizontal speed of the scroll

	horizSpeed = hSpeed;

}

void Layer::setGlobalAlpha(int al)
{

	// to give all sprites in the list same alpha value

	this->alpha = al;

}

void Layer::setGlobalColor(g2dColor col)
{

	// to shade all sprites to a darker color

	this->color = col;

}

void Layer::enableDebugLines(bool status)
{

	// to enable debug lines for all sprites in layer

	this->debugLines = status;

}

void Layer::isVisible(bool status)
{

	// enables or disable the layer visibility

	this->visible = status;

}

void Layer::setActive(bool status)
{
	// sets the layer is the layer that is being edited

	m_active = status;
}

bool Layer::isActive()
{
	// sets the layer is the layer that is being edited
	return m_active;
}

SpriteBatch* Layer::getSpriteBatch()
{

	// returns the sprite batch assosiated with the layer

	return spriteBatch;

}

string Layer::getName()
{
	return this->name;
}

void Layer::setName(string name)
{
	this->name = name;
}

void Layer::setLayerCameraPos(point cPos)
{
	m_cameraPos.x = cPos.x * horizSpeed;
	m_cameraPos.y = cPos.y * vertSpeed;
	TRACE(T_INF, "Layer Cam POs [%fx%f]", m_cameraPos.x, m_cameraPos.y);
}

void Layer::draw(SpriteBatch *sprBatch)
{
	std::vector<Sprite>::iterator it;

	// draws the layer on screen

	// set camera speed for the parralax scrolling for this layer

	cam->setSpeed(this->horizSpeed, this->vertSpeed);
	cam->placeCamera(camX, camY);
	//
	//m_camera->placeCamera(camX, camY);

	sprBatch->beginDraw();

	if (this->visible)
	{
		for (it = spriteList.begin(); it != spriteList.end(); it++)
		{
			if (m_active)
			{
				if (it->texRepeat)
				{
					sprBatch->endDraw();

					g2dTexture *tex = ServiceLocator::getTextureService()->GetTexture(it->texName);

					g2dBeginRects(tex);

					g2dSetTexLinear(false);

					g2dPop();

					g2dPush();

					g2dSetCoordMode(G2D_CENTER);

					it->draw(this->zDepth);

					g2dEnd();

					sprBatch->beginDraw();
				}
				else
				{
					g2dSetTexLinear(false);

					it->draw(this->zDepth);
				}
			}
		}
	}

	sprBatch->endDraw();

	if (m_active)
	{
		g2dBeginRects(NULL);
			g2dPop();
			g2dPush();
			g2dPush();
			g2dSetColor(RED);
			g2dSetScaleWH(10, 10);
			g2dSetCoordMode(G2D_CENTER);
			g2dSetCoordXYRelative(camX*this->horizSpeed, camY*this->vertSpeed);
			g2dAdd();
			g2dPop();
		g2dEnd();
	}

}
void Layer::updateSpritePos(float step)
{
	float x, y;
	std::vector<Sprite>::iterator it;
	CameraProvider *cam = ServiceLocator::getCameraService();

	TRACE(T_INF, "Layer [%d][%s] Updating Sprite positions", this->id, this->name.c_str());

	for (it = spriteList.begin(); it != spriteList.end(); it++)
	{

		x = (it)->sX / (it)->sCamZoom + (it)->sCamX * this->horizSpeed - ((it)->sWidth / 2) / (it)->sCamZoom;
		y = (it)->sY / (it)->sCamZoom + (it)->sCamY * this->vertSpeed - ((it)->sHeight / 2) / (it)->sCamZoom;

		(it)->x = x;
		(it)->y = y;
	}

}

void Layer::drawDebug()
{
	CameraProvider *cam;
	cam = ServiceLocator::getCameraService();

	float glScale = 1.0;
	g2dGetGlobalScale(&glScale);
	g2dResetGlobalScale();

	// draws the layer on screen

	// set camera speed for the parralax scrolling for this layer
	cam->setSpeed(this->horizSpeed, this->vertSpeed);

	if (this->visible)
	{
		std::vector<Sprite>::iterator it;

		for (it = spriteList.begin(); it != spriteList.end(); it++)
		{
			if (m_active)
			{
				// if this is the layer we are currently editing draw it with proper colors
				it->DrawDebug(GREEN);
			}
			else
			{
				// is not the layer we are currently editing so draw it shaded and transparent
				it->DrawDebug(GREEN);
			}
		}
	}
	g2dSetGlobalScale(glScale);
}

Sprite* Layer::getCollidingSprite()
{
	/*
	* Returns the sprite that is colliding with the mouse pointer
	* Sets that sprite to full brightnes while maintaining the rest of them to dark
	* !! Should return a list of colliding sprites if more thatn one and then with roller of mouse u go through them. !!!
	*/

	int height, width;
	ServiceLocator::getRenderService()->getWindowSize(&width, &height);

	CameraProvider *cam;
	cam = ServiceLocator::getCameraService();

	float angle, mx, my, x, y;

	double mouseX,mouseY;

	mouseX = ImGui::GetIO().MousePos.x;
	mouseY = ImGui::GetIO().MousePos.y;

	std::vector<Sprite>::iterator it;

	angle = cam->rot * (PI / 180);

	for (it = spriteList.begin(); it != spriteList.end(); it++)
	{
		if (it->visible)
		{
			x = mouseX / cam->zoom + cam->x*this->horizSpeed - (width / 2) / cam->zoom;
			y = mouseY / cam->zoom + cam->y*this->vertSpeed - (height / 2) / cam->zoom;

			mx = cosf(angle) * (x - cam->x) - sinf(angle) * (y - cam->y) + cam->x;
			my = sinf(angle) * (x - cam->x) + cosf(angle) * (y - cam->y) + cam->y;

			if (checkPointInRect(it->x, it->y, it->sW, it->sH, it->rotation, mx, my))
			{
				return &(*it);
			}
			else
			{
				continue;
			}
		}
	}
	return NULL;
}

Position Layer::getCollidingSpriteCamPos()
{
	int height, width;
	ServiceLocator::getRenderService()->getWindowSize(&width, &height);

	CameraProvider *cam;
	cam = ServiceLocator::getCameraService();

	Position p = { 0, 0 };

	float angle, mx, my, x, y;

	double mouseX, mouseY;
	mouseX = ImGui::GetIO().MousePos.x;
	mouseY = ImGui::GetIO().MousePos.y;

	std::vector<Sprite>::iterator it;

	angle = cam->rot * (PI / 180);

	for (it = spriteList.begin(); it != spriteList.end(); it++)
	{
		if (it->visible)
		{
			x = mouseX / cam->zoom + cam->x*this->horizSpeed - (width / 2) / cam->zoom;
			y = mouseY / cam->zoom + cam->y*this->vertSpeed - (height / 2) / cam->zoom;

			mx = cosf(angle) * (x - cam->x) - sinf(angle) * (y - cam->y) + cam->x;
			my = sinf(angle) * (x - cam->x) + cosf(angle) * (y - cam->y) + cam->y;

			if (checkPointInRect(it->x, it->y, it->sW, it->sH, it->rotation, mx, my))
			{
				Sprite *cs = &(*it);
				p.x = cs->x;
				p.y = cs->y;
			}
			else
			{
				continue;
			}
		}
	}
	return p;
}

vector<Sprite*> Layer::getCollidingSprites()
{
	int height, width;
	ServiceLocator::getRenderService()->getWindowSize(&width, &height);

	CameraProvider *cam;
	cam = ServiceLocator::getCameraService();

	vector<Sprite*> colSpriteList;
	/*
	* Returns the sprite that is colliding with the mouse pointer
	* Sets that sprite to full brightnes while maintaining the rest of them to dark
	* !! Should return a list of colliding sprites if more thatn one and then with roller of mouse u go through them. !!!
	*/

	float angle, mx, my, x, y;

	double mouseX, mouseY;

	vector<Sprite>::iterator it;

	angle = cam->rot * (PI / 180);

	for (it = spriteList.begin(); it != spriteList.end(); it++)
	{
		if (it->visible)
		{
			mouseX = ImGui::GetIO().MousePos.x;
			mouseY = ImGui::GetIO().MousePos.y;

			x = mouseX / cam->zoom + cam->x*this->horizSpeed - (width / 2) / cam->zoom;
			y = mouseY / cam->zoom + cam->y*this->vertSpeed - (height / 2) / cam->zoom;

			mx = cosf(angle) * (x - cam->x) - sinf(angle) * (y - cam->y) + cam->x;
			my = sinf(angle) * (x - cam->x) + cosf(angle) * (y - cam->y) + cam->y;

			if (checkPointInRect(it->x, it->y, it->sW, it->sH, it->rotation, mx, my))
			{
				colSpriteList.push_back(&(*it));
			}
		}
	}

	return colSpriteList;
}

vector<Sprite*> Layer::getCollidingSprites(float spriteX, float spriteY)
{
	int height, width;
	ServiceLocator::getRenderService()->getWindowSize(&width, &height);

	CameraProvider *cam;
	cam = ServiceLocator::getCameraService();

	vector<Sprite*> colSpriteList;
	/*
	* Returns the sprite that is colliding with the mouse pointer
	* Sets that sprite to full brightnes while maintaining the rest of them to dark
	* !! Should return a list of colliding sprites if more thatn one and then with roller of mouse u go through them. !!!
	*/

	float angle, mx, my, x, y;

	double mouseX=spriteX, mouseY=spriteY;

	vector<Sprite>::iterator it;

	angle = cam->rot * (PI / 180);

	for (it = spriteList.begin(); it != spriteList.end(); it++)
	{
		if (it->visible)
		{
			x = mouseX / cam->zoom + cam->x*this->horizSpeed - (width / 2) / cam->zoom;
			y = mouseY / cam->zoom + cam->y*this->vertSpeed - (height / 2) / cam->zoom;

			mx = cosf(angle) * (x - cam->x) - sinf(angle) * (y - cam->y) + cam->x;
			my = sinf(angle) * (x - cam->x) + cosf(angle) * (y - cam->y) + cam->y;

			if (checkPointInRect(it->x, it->y, it->sW, it->sH, it->rotation, mx, my))
			{
				colSpriteList.push_back(&(*it));
			}
		}
	}

	return colSpriteList;
}

int Layer::getCollidingSpritePosition()
{
	int height, width;
	ServiceLocator::getRenderService()->getWindowSize(&width, &height);

	CameraProvider *cam;
	cam = ServiceLocator::getCameraService();

	/*
	* Returns the sprite that is colliding with the mouse pointer
	* Sets that sprite to full brightnes while maintaining the rest of them to dark
	* !! Should return a list of colliding sprites if more thatn one and then with roller of mouse u go through them. !!!
	*/

	float angle, mx, my, x, y;

	double mouseX, mouseY;
	mouseX = ImGui::GetIO().MousePos.x;
	mouseY = ImGui::GetIO().MousePos.y;

	angle = cam->rot * (PI / 180);

	for (unsigned int i = 0; i != spriteList.size(); ++i)
	{
		if (spriteList[i].visible)
		{
			x = mouseX / cam->zoom + cam->x*this->horizSpeed - (width / 2) / cam->zoom;
			y = mouseY / cam->zoom + cam->y*this->vertSpeed - (height / 2) / cam->zoom;

			mx = cosf(angle) * (x - cam->x) - sinf(angle) * (y - cam->y) + cam->x;
			my = sinf(angle) * (x - cam->x) + cosf(angle) * (y - cam->y) + cam->y;

			if (checkPointInRect(spriteList[i].x, spriteList[i].y, spriteList[i].sW, spriteList[i].sH, spriteList[i].rotation, mx, my))
			{
				return i;
			}
			else
			{
				continue;
			}
		}
	}
	return 0;
}

bool Layer::checkPointInRect(float rx, float ry, float rw, float rh, float rot, float px, float py)
{
	// rotation in radians, because flash likes to be a pain and use them instead.

	//float rotRad = (PI * rot) / 180;

	float dx = px - rx;

	float dy = py - ry;

	// distance between point and centre of rectangle.
	float h1 = sqrtf(dx * dx + dy * dy);

	float currA = atan2f(dy, dx);

	// angle of point rotated by the rectangle amount around the centre of rectangle.
	// float newA = currA - rotRad;
	float newA = currA - rot;

	// x2 and y2 are the new positions of the point when rotated to offset the rectangles orientation.
	float x2 = cosf(newA) * h1;

	float y2 = sinf(newA) * h1;

	// the above points are relative to the centre of the rectangle, so the check is simple.
	if (x2 > -0.5 * rw && x2 < 0.5 * rw && y2 > -0.5 * rh && y2 < 0.5 * rh)
	{
		return true;
	}

	return false;

}

void Layer::deleteCollidingSprite(Sprite *s)
{
	int height, width;
	ServiceLocator::getRenderService()->getWindowSize(&width, &height);

	CameraProvider *cam;
	cam = ServiceLocator::getCameraService();

	/*
	* Returns the sprite that is colliding with the mouse pointer
	*/

	float angle, mx, my, x, y;

	angle = cam->rot * (PI / 180);

	double mouseX, mouseY;
	mouseX = ImGui::GetIO().MousePos.x;
	mouseY = ImGui::GetIO().MousePos.y;

	vector<Sprite>::iterator it;

	for (it = spriteList.begin(); it != spriteList.end(); it++)
	{
		if (it->visible)
		{

			x = mouseX / cam->zoom + cam->x*this->horizSpeed - (width / 2) / cam->zoom;
			y = mouseY / cam->zoom + cam->y*this->vertSpeed - (height / 2) / cam->zoom;

			mx = cosf(angle) * (x - cam->x) - sinf(angle) * (y - cam->y) + cam->x;
			my = sinf(angle) * (x - cam->x) + cosf(angle) * (y - cam->y) + cam->y;

			if (checkPointInRect(it->x, it->y, it->sW, it->sH, it->rotation, mx, my))
			{
				spriteList.erase(it);
				totalSprites--;
				return;
			}
			else
			{
				continue;
			}
		}
	}
}

void Layer::deleteSprite(Sprite *colSprite)
{
	// we can not delete immitiadely we should delete before we start itterating layer vector to draw sprites

	vector<Sprite>::iterator it;

	// find sprite ID and delete it from layer

	for (it = spriteList.begin(); it != spriteList.end(); it++)
	{
		if (it->id == colSprite->id)
		{
			// add sprite ID to deletion list
			TRACE(T_DEB, "Mark for deletion sprite ID[%d] NAME[%s]", colSprite->id, colSprite->name.c_str());
			markedForDeletion.push_back(it);
		}
	}
}

void Layer::eraseMarkedForDeletion()
{
	vector<vector<Sprite>::iterator>::iterator itMarkedForDeletion;

	// find sprite ID and delete it from layer

	for (itMarkedForDeletion = markedForDeletion.begin(); itMarkedForDeletion != markedForDeletion.end(); itMarkedForDeletion++)
	{
		TRACE(T_DEB, "Deleting sprite ID[%d]", (*itMarkedForDeletion)->id);
		spriteList.erase(*itMarkedForDeletion);
		totalSprites--;
	}

	markedForDeletion.clear();
}

void Layer::SetHSpeed(float r)
{
	this->horizSpeed = r;

	std::vector<Sprite>::iterator it;

	for (it = spriteList.begin(); it != spriteList.end(); it++)
	{


	}
}

float Layer::GetHSpeed() const
{
	return this->horizSpeed;
}

void Layer::SetVSpeed(float r)
{
	this->vertSpeed = r;
	
	std::vector<Sprite>::iterator it;

	for (it = spriteList.begin(); it != spriteList.end(); it++)
	{


	}
}

float Layer::GetVSpeed() const
{
	return this->vertSpeed;
}

void Layer::updateLayerPosX(int x)
{
	if (this->visible)
	{
		std::vector<Sprite>::iterator it;

		for (it = spriteList.begin(); it != spriteList.end(); it++)
		{
			it->x += x;
		}
	}
}

void Layer::updateLayerPosY(int y)
{
	if (this->visible)
	{
		std::vector<Sprite>::iterator it;

		for (it = spriteList.begin(); it != spriteList.end(); it++)
		{
			it->y += y;
		}
	}
}