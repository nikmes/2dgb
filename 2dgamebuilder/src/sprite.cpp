/*
* Sprite.cpp
*
*  Created on: Oct 18, 2013
*      Author: nikmes
*/

#include "servicelocator.h"
#include <stdlib.h>
#include <math.h>
#include "sprite.h"
#include "globals.h"

int Sprite::next_id = 0;

Sprite::~Sprite()
{

}

Sprite::Sprite()
{
	this->alpha = 255; // initial value for transparency, default is not transparent
	this->rotation = 0; // intial rotation
	this->id = ServiceLocator::getHelpService()->generateId(); // sprite id
	this->scale = 1; // no scale if just created but actual size
	this->flipState = FLIP_NONE; // fliped state of sprite
	this->visible = true;		// by default sprite is visible when created
	this->texRepeat = false;
	this->col[0] = 255;
	this->col[1] = 255;
	this->col[2] = 255;
	this->col[3] = 255;
	this->setColor(ServiceLocator::getHelpService()->convertRGBtoColor(this->col[0], this->col[1], this->col[2]));
	this->m_z = 0.0;
	sX = .0;
	sY = .0;
	sHeight =.0;
	sWidth = .0;
	sCamZoom = .0;
	sCamX = .0;
	sCamY = .0;
};


Sprite::Sprite(int height, int width, int cropX, int cropY, int scaleX, int scaleY)
{
	this->alpha = 255;				// initial value for transparency, default is not transparent
	this->rotation = 0;				// intial rotation
	this->id = ServiceLocator::getHelpService()->generateId(); // unique id of sprite
	this->sH = scaleY;				// scaled highth
	this->sW = scaleX;				// scaled width
	this->flipState = FLIP_NONE;	// fliped mode
	this->visible = true;			// by default sprite is visible when created
	this->texRepeat = false;		// sprite is filled with a texture patern
	this->col[0] = 255;				// Default R
	this->col[1] = 255;				// Default G
	this->col[2] = 255;				// Default B
	this->col[3] = 255;				// Defailt Alpha
	this->setColor(ServiceLocator::getHelpService()->convertRGBtoColor(this->col[0], this->col[1], this->col[2]));
	this->m_z = 0;
	sX = .0;
	sY = .0;
	sX = .0;
	sY = .0;
	sHeight = .0;
	sWidth = .0;
	sCamZoom = .0;
	sCamX = .0;
	sCamY = .0;
}

void Sprite::setPositionRelative(float x, float y)
{
	m_rX = x;
	m_rY = y;
}

void Sprite::setPosition(float pX, float pY, float z)
{
	x = pX;
	y = pY;
	m_z = z;
}

void Sprite::setScale(float newScale)
{
	// dont allow near 0 value other wise it will crash at some point when 
	// we try divide 0 with a value.
	// here we might need to allo values also < -0.10

	if ((scale + newScale) > 0.10)
	{
		scale = newScale;
	}
	else
	{
		return;
	}

	sW = floor(cW*scale);
	sH = floor(cH*scale);
}

void Sprite::setRotation(float currRotation)
{
	rotation = currRotation;
}

void Sprite::setColor(g2dColor col)
{
	this->color = col;
}

void Sprite::draw(Flip flipMode)
{
	/*
	* Draw the sprite
	*/

	g2dPush();

	g2dSetCoordMode(G2D_CENTER);

	g2dSetCoordXYRelative(x, y);

	if (this->color)
	{
		g2dSetColor(color);
	}

	g2dSetCropXY(cX, cY);

	g2dSetCropWH(cW, cH);

	switch (flipMode)
	{
	case FLIP_HV:
		g2dSetScaleWH((sW - (sW * 2)), (sH - (sH * 2)));
		break;

	case FLIP_H:
		g2dSetScaleWH((sW - (sW * 2)), sH);
		break;

	case FLIP_V:
		g2dSetScaleWH(sW*scale, (sH - (sH * 2))*scale);
		break;

	default:
		g2dSetScaleWH(sW*scale, sH*scale);
		break;
	}

	g2dSetRotationRelative(rotation);

	g2dSetAlpha(alpha);

	if (isVisible())
	{
		totalRenderedSprites += 1;

		g2dAdd();
	}

	g2dPop();
}

void Sprite::setFlipState(Flip fState)
{
	flipState = fState;
}

void Sprite::draw(float z)
{
	g2dPush();
	
	z += this->m_z;

	g2dSetCoordXYZRelative(x, y, z);

	if (this->color)
	{
		g2dSetColor(color);
	}

	if (this->texRepeat) // if texRepeat is enabled for this sprite enable it also for the rendering context
	{
		g2dSetTexRepeat(true);
	}
	else
	{
		g2dSetCropXY(cX, cY); // this croping should only be applied when we drawing tiles from textureAtlas
	}

	g2dSetCropWH(cW, cH);

	switch (flipState)
	{
	case FLIP_HV:
		g2dSetScaleWH((sW - (sW * 2)), (sH - (sH * 2)));
		break;

	case FLIP_H:
		g2dSetScaleWH((sW - (sW * 2)), sH);
		break;

	case FLIP_V:
		g2dSetScaleWH(sW, (sH - (sH * 2)));
		break;

	case FLIP_NONE:
	default:
		g2dSetScaleWH(sW, sH);
		break;
	}

	g2dSetRotationRadRelative(rotation);

	g2dSetAlpha(alpha);

	if (isVisible())
	{
		totalRenderedSprites += 1;
		g2dAdd();
#ifndef PLATFORM_PSP
		if (ServiceLocator::getConfigurationService()->pEditor_drawdebuglines)
		{
			float z = ServiceLocator::getGameService()->getCurrentLayer()->zDepth;
			z += this->m_z;
			ServiceLocator::getTextService()->pushLine(vx[0], vy[0], "pos=[%.0f,%.0f,%.3f]", x, y,z);
			ServiceLocator::getTextService()->pushLine(vx[0], vy[0] - 20, "pos=[%.0f,%.0f]", sX, sY);
		}
#endif
	}

	g2dPop();

}

void Sprite::draw()
{
	g2dPush();

	g2dSetCoordXYRelative(x, y);

	if (this->color)
	{
		g2dSetColor(color);
	}

	g2dSetCropXY(cX, cY);

	g2dSetCropWH(cW, cH);

	switch (flipState)
	{
	case FLIP_HV:
		g2dSetScaleWH((sW - (sW * 2)), (sH - (sH * 2)));
		break;

	case FLIP_H:
		g2dSetScaleWH((sW - (sW * 2)), sH);
		break;

	case FLIP_V:
		g2dSetScaleWH(sW, (sH - (sH * 2)));
		break;

	case FLIP_NONE:
	default:
		g2dSetScaleWH(sW, sH);
		break;
	}

	g2dSetRotationRadRelative(rotation);

	g2dSetAlpha(alpha);

	if (isVisible())
	{
		totalRenderedSprites += 1;
		g2dAdd();

#ifndef PLATFORM_PSP
		if (ServiceLocator::getConfigurationService()->pEditor_drawdebuglines)
		{
			ServiceLocator::getTextService()->pushLine(vx[0], vy[0], "pos=[%.0f,%.0f]", x, y);
		}
#endif

	}

	g2dPop();

}

void Sprite::drawRelative()
{
	g2dPush();

	if (this->color)
	{
		g2dSetColor(color);
	}

	g2dSetCoordXYRelative(m_rX, m_rY);

	g2dSetCropXY(cX, cY);

	g2dSetCropWH(cW, cH);

	g2dSetScaleWH(sW, sH);

	g2dSetRotation(rotation);

	g2dSetAlpha(alpha);

	g2dAdd();

	g2dPop();

}

void Sprite::drawRelativeBlackBox()
{
	g2dPush();

	g2dSetColor(BLACK);

	g2dSetAlpha(230);

	g2dSetCoordXYRelative(m_rX, m_rY);

	g2dSetScaleWH(sW, sH);

	g2dAdd();

	g2dPop();
}

void Sprite::drawDebug(g2dCoord_Mode coordMode)
{
	g2dSetCoordMode(coordMode);

	g2dSetCoordXY(x, y);

	g2dSetCropXY(cX, cY);

	g2dSetCropWH(cW, cH);

	g2dSetScaleWH(sW, sH);

	g2dSetRotation(rotation);

	g2dSetAlpha(alpha);

	isVisible();

}

void Sprite::DrawDebug(g2dColor col)
{
	// Draw Box arround sprite for debuging reasons
	g2dBeginLines(G2D_STRIP);
	{
		g2dSetColor(col);
		g2dSetAlpha(150);

		//g2dSetScaleWH(this->sW, this->sH);

		g2dSetCoordXY(vx[0], vy[0]);
		g2dAdd();

		g2dSetCoordXY(vx[3], vy[3]);
		g2dAdd();

		g2dSetCoordXY(vx[1], vy[1]);
		g2dAdd();

		g2dSetCoordXY(vx[2], vy[2]);
		g2dAdd();

		g2dSetCoordXY(vx[0], vy[0]);
		g2dAdd();
	}

	g2dEnd();

}

void Sprite::DrawDebugForDelete(g2dColor col)
{
	// Draw Box arround sprite for debuging reasons
	g2dBeginLines(G2D_STRIP);
	{
		g2dSetColor(col);
		g2dSetAlpha(255);

		//g2dSetScaleWH(this->sW, this->sH);

		g2dSetCoordXY(vx[0], vy[0]);
		g2dAdd();

		g2dSetCoordXY(vx[3], vy[3]);
		g2dAdd();

		g2dSetCoordXY(vx[1], vy[1]);
		g2dAdd();

		g2dSetCoordXY(vx[2], vy[2]);
		g2dAdd();

		g2dSetCoordXY(vx[0], vy[0]);
		g2dAdd();
	}

	g2dEnd();

}
/*
int Sprite::isVisible()
{
	int height, width;
	ServiceLocator::getVideoService()->getWindowSize(&width, &height);

	// Get first corner's position

	g2dPush();

	g2dSetCoordXYRelative(-this->sW / 2, -this->sH / 2);
	g2dGetCoordXYZ(&vx[0], &vy[0], NULL);
	//g2dPush();

	// Get second corner's position
	g2dSetCoordXYRelative(this->sW, this->sH);
	g2dGetCoordXYZ(&vx[1], &vy[1], NULL);

	//Get third corner's position
	g2dSetCoordXYRelative(.0f, -this->sH);
	g2dGetCoordXYZ(&vx[2], &vy[2], NULL);

	//Get fourth corner's position
	g2dSetCoordXYRelative(-this->sW, this->sH);
	g2dGetCoordXYZ(&vx[3], &vy[3], NULL);

	g2dPop();

	// Check if the whole object is off-screen.
	return (!((vx[0] < 0 || vx[0] > width || vy[0] < 0 || vy[0] > height) &&
		(vx[1] < 0 || vx[1] > width || vy[1] < 0 || vy[1] > height) &&
		(vx[2] < 0 || vx[2] > width || vy[2] < 0 || vy[2] > height) &&
		(vx[3] < 0 || vx[3] > width || vy[3] < 0 || vy[3] > height)));
}
*/

int Sprite::isVisible()
{
	int height, width, xOffSet=0, yOffSet=0;
	ServiceLocator::getRenderService()->getWindowSize(&width, &height);
	float camZoom;

	if (ServiceLocator::getConfigurationService()->pEditor_drawpspbox)
	{
		camZoom = ServiceLocator::getCameraService()->zoom;
		xOffSet = ((width / 2) - (480 / 2) * camZoom);
		yOffSet = ((height / 2) - (272 / 2) * camZoom);
		width = xOffSet + (480 * camZoom);
		height = yOffSet + (272 * camZoom);
	}

	// Get first corner's position

	g2dPush();

	g2dSetCoordXYRelative(-this->sW / 2, -this->sH / 2);
	g2dGetCoordXYZ(&vx[0], &vy[0], NULL);
	//g2dPush();

	// Get second corner's position
	g2dSetCoordXYRelative(this->sW, this->sH);
	g2dGetCoordXYZ(&vx[1], &vy[1], NULL);

	//Get third corner's position
	g2dSetCoordXYRelative(.0f, -this->sH);
	g2dGetCoordXYZ(&vx[2], &vy[2], NULL);

	//Get fourth corner's position
	g2dSetCoordXYRelative(-this->sW, this->sH);
	g2dGetCoordXYZ(&vx[3], &vy[3], NULL);

	g2dPop();

	// Check if the whole object is off-screen.
	return (!((vx[0] < xOffSet || vx[0] > width || vy[0] < yOffSet || vy[0] > height) &&
			  (vx[1] < xOffSet || vx[1] > width || vy[1] < yOffSet || vy[1] > height) &&
			  (vx[2] < xOffSet || vx[2] > width || vy[2] < yOffSet || vy[2] > height) &&
			  (vx[3] < xOffSet || vx[3] > width || vy[3] < yOffSet || vy[3] > height)));
}

/*
int Sprite::isVisible()
{
	int height, width, xOffSet = 0, yOffSet = 0;
	ServiceLocator::getVideoService()->getWindowSize(&width, &height);
	float camZoom;

	if (ServiceLocator::getConfigurationService()->pEditor_drawpspbox)
	{
		camZoom = ServiceLocator::getCameraService()->zoom;
		xOffSet = ((width / 2) - (984 / 2) * camZoom);
		yOffSet = ((height / 2) - (610 / 2) * camZoom);
		width = xOffSet + (984 * camZoom);
		height = yOffSet + (610 * camZoom);
	}

	// Get first corner's position

	g2dPush();

	g2dSetCoordXYRelative(-this->sW / 2, -this->sH / 2);
	g2dGetCoordXYZ(&vx[0], &vy[0], NULL);
	//g2dPush();

	// Get second corner's position
	g2dSetCoordXYRelative(this->sW, this->sH);
	g2dGetCoordXYZ(&vx[1], &vy[1], NULL);

	//Get third corner's position
	g2dSetCoordXYRelative(.0f, -this->sH);
	g2dGetCoordXYZ(&vx[2], &vy[2], NULL);

	//Get fourth corner's position
	g2dSetCoordXYRelative(-this->sW, this->sH);
	g2dGetCoordXYZ(&vx[3], &vy[3], NULL);

	g2dPop();

	// Check if the whole object is off-screen.
	return (!((vx[0] < xOffSet || vx[0] > width || vy[0] < yOffSet || vy[0] > height) &&
		(vx[1] < xOffSet || vx[1] > width || vy[1] < yOffSet || vy[1] > height) &&
		(vx[2] < xOffSet || vx[2] > width || vy[2] < yOffSet || vy[2] > height) &&
		(vx[3] < xOffSet || vx[3] > width || vy[3] < yOffSet || vy[3] > height)));
}
*/
void Sprite::copyFrom(Sprite *spr)
{
	x = spr->x;
	y = spr->y;
	cX = spr->cX;
	cY = spr->cY;
	visible = spr->visible;
	sW = spr->cW;
	sH = spr->cH;
	cW = spr->cW;
	cH = spr->cH;
}

void Sprite::resetProperties()
{
	x = -1000;
	y = -1000;
	cH = 1;
	cW = 1;
	cX = 1;
	cY = 1;
	sW = 1;
	sH = 1;
}