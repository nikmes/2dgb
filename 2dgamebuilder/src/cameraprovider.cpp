
#include "servicelocator.h"
#include <stdio.h>

CameraProvider::CameraProvider(float x, float y)
{
	// init camera
	this->active = true;
	this->x_target = 0.f;
	this->x = 0.f;
	this->y_target = 0.f;
	this->y = 0.f;
	this->rot_target = 0.f;
	this->rot = 0.f;
	this->zoom_target = 1.0f;
	this->zoom = 1.f;

	this->hSpeed = 1.f;
	this->vSpeed = 1.f;

	this->moving = false;

	CAM_STEP_X = 0.f;
	CAM_STEP_Y = 0.f;

	CAM_POS_SPEED = 0.06f; 
	CAM_ROT_SPEED = 0.18f; 
	CAM_ZOOM_SPEED = 0.05f;

}

CameraProvider::CameraProvider()
{
	// init camera
	this->active = true;
	this->x_target = 0.f;
	this->x = 0.f;
	this->y_target = 0.f;
	this->y = 0.f;
	this->rot_target = 0.f;
	this->rot = 0.f;
	this->zoom_target = 1.0f;
	this->zoom = 1.f;

	this->hSpeed = 1.f;
	this->vSpeed = 1.f;

	this->moving = false;

	CAM_STEP_X = 0.f;
	CAM_STEP_Y = 0.f;
	CAM_POS_SPEED = 0.06f;
	CAM_ROT_SPEED = 0.18f;
	CAM_ZOOM_SPEED = 0.05f;

}

CameraProvider::~CameraProvider()
{

}

void CameraProvider::init()
{

}

void CameraProvider::setSpeed(float hs, float vs)
{
	// sets the horizontal and vertical speed of the camera
	this->hSpeed = hs;
	this->vSpeed = vs;
}

void CameraProvider::setCameraRot(int rot)
{
	int r = (int)(rot - this->rot_target) % 360;

	if (r < -180)
	{
		this->rot_target += r + 360;
	}
	else if (r > 180)
	{
		this->rot_target += r - 360;
	}
	else
	{
		this->rot_target += r;
	}

}

void CameraProvider::placeCamera(float pX, float pY)
{

	int width, height;

	ServiceLocator::getRenderService()->getWindowSize(&width, &height);

	// Camera center too far from player

	if (this->active)
	{
		if (this->x - pX > CAM_STEP_X)
		{
			this->x_target = (pX + CAM_STEP_X);
		}
		else if (pX - this->x > CAM_STEP_X)
		{
			this->x_target = (pX - CAM_STEP_X) ;
		}

		if (this->y - pY > CAM_STEP_Y)
		{
			this->y_target = pY + CAM_STEP_Y;
		}
		else if (pY - this->y > CAM_STEP_Y)
		{
			this->y_target = pY - CAM_STEP_Y;
		}
	}

	// Place the camera
	g2dBeginRects(NULL);
	{
		g2dPop();
		g2dSetCoordXY((width / 2), (height / 2));
		g2dSetRotation(-this->rot);
		g2dSetGlobalScale(this->zoom);
		g2dSetCoordXYRelative(-x*(hSpeed), -y*(vSpeed));
//		g2dSetCoordXYRelative(-x, -y);
		g2dPush();
	}
	g2dEnd();
	
	moving = false;

}

point CameraProvider::getScreenCoordinates(float wx, float wy)
{
	/*
	 * Returns the sprite that is colliding with the mouse pointer
	 */

	int height, width;

	point p;

	ServiceLocator::getRenderService()->getWindowSize(&width, &height);

	CameraProvider *cam = ServiceLocator::getCameraService();

	float angle, x, y;

	angle = cam->rot * (PI / 180);

	//x = wx / cam->zoom + cam->x*ServiceLocator::getLevelService()->getCurrentLayer()->horizSpeed - (width / 2) / cam->zoom;
	//y = wy / cam->zoom + cam->y*ServiceLocator::getLevelService()->getCurrentLayer()->vertSpeed - (height / 2) / cam->zoom;

	x = wx / cam->zoom + cam->x*ServiceLocator::getGameService()->getCurrentLayer()->horizSpeed - (width / 2) / cam->zoom;
	y = wy / cam->zoom + cam->y*ServiceLocator::getGameService()->getCurrentLayer()->vertSpeed - (height / 2) / cam->zoom;

	p.x = cosf(angle) * (x - cam->x) - sinf(angle) * (y - cam->y) + cam->x;
	p.y = sinf(angle) * (x - cam->x) + cosf(angle) * (y - cam->y) + cam->y;

	return p;
}

void CameraProvider::decceleration()
{
	this->x += ((this->x_target - this->x) * CAM_POS_SPEED);

	this->y += ((this->y_target - this->y) * CAM_POS_SPEED);

	this->rot += (this->rot_target - this->rot) * CAM_ROT_SPEED;
	
	this->zoom += (this->zoom_target - this->zoom) * CAM_ZOOM_SPEED;
}