#ifndef LAYER_H
#define LAYER_H

#include <string>
#include <vector>

#include "spritebatch.h"
#include "definitions.h"

using namespace std;

class SpriteBatch;
class CameraProvider;

class Layer
{
private:

	static int next_id; // next id to assign to newly created layer

	int alpha; // the alpha value of layer

	g2dColor color; // the color of layer

	bool visible; // should layer be rendered yes or no

	bool debugLines; // enable or disable sprite debug lines

	vector<vector<Sprite>::iterator> markedForDeletion; // sprite ids marked for deletion
	
	CameraProvider *cam;

	point m_cameraPos;

public:

	vector<Sprite> spriteList; // list of sprites 

	string name; // the name of the layer

	int	id; // id of current layer

	bool m_active; // is this the active layer that we are currently editing?

	int totalSprites; // total number of sprites in the layer

	float horizSpeed; // speed for horizontal movement

	float vertSpeed; // speed for vertical movement

	float zDepth;	// how far or close to the camera layer is

	SpriteBatch *spriteBatch; // each layer can be linked to one spriteBatch 


	Layer(); // default constructor

	//Layer(string name, string sbFileName, float hSpeed, float vSpeed, float zDepth); // custom layer constructor, accepts the layer name as string

	Layer(string name, float hSpeed, float vSpeed, float zDepth); // custom layer constructor, accepts the layer name as string

	Layer(string name, float hSpeed, float vSpeed, float zDepth, int id); // custom layer constructor, accepts the layer name as string

	~Layer(); // default destructor

	int getTotalSprites(); // returns total number of sprites in layer

	string getName(); // return the layer name

	void setName(string name); // sets the layers name

	void addSprite(Sprite spr, layerPostion pos); // to add a sprite to the layer

	void removeSprite(); // ro remove a sprite from the layer

	void setVerticalSpeed(float vSpeed); // to set the vertical speed of the scroll

	void setHorizontalSpeed(float hSpeed); // to set the horizontal speed of the scroll

	void setGlobalAlpha(int al); // to give all sprites in the list same alpha value

	void setGlobalColor(g2dColor col); // to shade all sprites to a darker color

	void enableDebugLines(bool statys); // to enable debug lines for all sprites in layer

	void isVisible(bool status); // enables or disable the layer visibility

	void setActive(bool status); // enable or disable layer for editing

	bool isActive(); // returns true or false based on layer active flag

	void draw(SpriteBatch *sprBatch); // renders the layer on screen

	void drawDebug(); // draw debug lines arround sprites

	void updateLayerPosX(int x); // update all sprites x position								

	void updateLayerPosY(int y); // update all sprites y position								

	Sprite* getCollidingSprite(); // returns pointer to sprite colliding with the mouse

	vector<Sprite*> getCollidingSprites(); // returns a list of pointers to all sprites of the layer colliding with the mouse

	vector<Sprite*> getCollidingSprites(float spriteX, float spriteY);      

	int getCollidingSpritePosition(); // returns colliding sprite and allow modification

	Position getCollidingSpriteCamPos(); // returns sprite absolute coordinates

	void deleteCollidingSprite(Sprite *s); // deletes colliding sprite

	SpriteBatch* getSpriteBatch(); // returns the sprite batch assosiated with the layer

	bool checkPointInRect(float rx, float ry, float rw, float rh, float rot, float px, float py);

	void deleteSprite(Sprite *colSprite);

	void eraseMarkedForDeletion();

	void SetHSpeed(float r);

	float GetHSpeed() const;

	void SetVSpeed(float r);

	float GetVSpeed() const;

	void swapWithNextLayer();

	void swapWithPreviousLayer();

	void updateSpritePos(float step);

	void setLayerCameraPos(point cPos);
};

#endif 
