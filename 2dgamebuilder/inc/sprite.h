/*
* Sprite.h
*
*  Created on: Oct 18, 2013
*      Author: nikmes
*/

#ifndef SPRITE_H
#define SPRITE_H

#include <iostream>
#include <glib2d/glib2d.h>

using namespace std;

class SpriteBatch;

typedef enum
{
	FLIP_H,
	FLIP_V,
	FLIP_HV,
	FLIP_NONE
} Flip;

class Sprite
{
private:

	static int next_id;

	float finishTime;
	float timer;
	float dt;

public:
	Flip		  flipState;
	g2dColor	  color;
	int			  id;
	string		  hint;
	bool		  isSubBar;
	int			  position;
	float		  rotation;
	float		  scale;
	bool		  visible;
	unsigned char col[4]; // = { 255,255,255,255 };
	string        dispName;
	bool		  texRepeat;
	string		  texName;
	SpriteBatch   *sprBatch;

	enum Rot
	{
		ROT_LEFT,
		ROT_RIGHT,
	};

	string name;

	float   m_z;  // z depth
	float	x;	// x position of sprite
	float	y;	// y position of sprite

	float   sX;
	float   sY;
	int		sHeight;
	int		sWidth;
	float   sCamZoom;
	float   sCamX;
	float   sCamY;

	float	m_rX; // x position relative
	float   m_rY; // y position relative

	float	cW;	// width of sprite
	float	cH;	// height of sprite

	int		cX;	// crop x of sprite
	int		cY;	// crop y of sprite

	float	sW;	// scale x
	float	sH;	// scale y



	float vx[5], vy[5];	// to check if sprite is visible and to draw debug lines

	int  alpha;			// transparency of sprite

	~Sprite();

	Sprite();

	Sprite(int height, int width, int cropX, int cropY, int scaleX, int scaleY);

	void setColor(g2dColor col);

	void setRotation(float currRotation);

	void setPosition(float pX, float pY, float m_z);

	void setPositionRelative(float x, float y);

	void setScale(float newScale);

	void draw(Flip flipMode);

	void draw();

	void draw(float z);

	void DrawDebug(g2dColor col);

	void DrawDebugForDelete(g2dColor col);

	void drawDebug(g2dCoord_Mode coordMode);

	int  isVisible();

	void drawRelative();

	void drawRelativeBlackBox();

	void copyFrom(Sprite *spr);

	void resetProperties();

	void setFlipState(Flip fState);
};

#endif /* SPRITE_H */
