/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file camera.h
*
*/

#ifndef CAMERA_H
#define CAMERA_H

#include <stdlib.h>
#include <glib2d/glib2d.h>

class Camera
{
public:
	float CAM_STEP_X;
	float CAM_STEP_Y;

	float CAM_POS_SPEED;
	float CAM_ROT_SPEED;
	float CAM_ZOOM_SPEED;

	bool active;
	bool moving;

	float x_target;
	float y_target;

	float x;
	float y;

	Camera();

	Camera(float x, float y);

	~Camera();

	float zoom_target;								// target zoom of the camera

	float zoom;										// current zoom of the camera

	float rot_target;								// target rotation of the camera

	float rot;										// current rotation of the camera

	void setCameraRot(int rot);						// set camera rotation

	void placeCamera(float pX, float pY);			// place camera (after a setCameraPos)

	void setSpeed(float hs, float vs);				// set camera speed

	point getScreenCoordinates(float x, float y);	// gets a set of word coordinates and return the screen coordinates

	void init();

	void decceleration();
};

#endif