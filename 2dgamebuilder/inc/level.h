#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <map>
#include <Box2D/Box2D.h>
#include "box2ddebugdraw.h"
#include "layer.h"

using namespace std;

class Level;

class Level
{
private:
	vector<Layer*>		 layers;		// layers of the level

	vector<Level>		 subLevels;		// subLevels of the level

	vector<SpriteBatch*> sprBatches;	// all spritebatches used in level

	vector<string>		 staticImages;	// all static images used in the level

	int	m_currentLayer; // current layer under edit

public:
	int m_currentSpriteBatch; // current Spritebatch
	string m_name;					 // the name of the level, will be used also as filename when saving the map
	string m_backgroundFileName;	 // name of png that holds the background picture
	string m_fileName;				 // the actual xml filename that stores the level

	bool m_backgroundVisible;		 // background png visible or not
	bool m_levelIsLoaded;			 // indicates whether the level is loaded or not

	Box2dDebugDraw fooDrawInstance;  // enable debug drawing of box2d shapes
	b2Vec2	*Gravity;				 // Gravity of the Box2d World
	b2World	*World;					 // The Box2d World

	b2Body	*playerBody;
	b2Body	*weightBody;

	Level();							// default constructor
	Level(string name);					// custom constructor, accepts map name as a string
	~Level();							// default drstructor

	void	save();						// save the level in an xml file
	void	saveAs(string fileName);	// save the level in a different filename
	void    saveSpriteBatches(TiXmlElement *texturemapsNode);
	void    saveLayers(TiXmlElement *layersNode, SpriteBatch* currSpritebatch);
	void    saveSprites(TiXmlElement *spritesNode, Layer* currLayer);

	void	load(string fileName);		// loads the level from an xml file

	// Box2d Related Methods
	void	createPhysicalWorld(); // creates the box2d world
	int		CreateLine(float x1, float y1, float x2, float y2); // create a box2d collision line
	void	CreateBox(int MouseX, int MouseY); // add random box at mouse pointer
	void	CreatePlayer(float x, float y);	// creates our player in the level

	void	removeLayer(string name); // removes a layer from the map
	void	draw(); // draws the map on screen
	Layer*	getLayer(string name); // returns the layer that has name equals to incomming name

	int     getTotalSpriteBatches(); // returns the number of total spritebatches
	int		getTotalLayers(); // returns the number of layers in the map
	int		getTotalSprites(); // returns the total number of sprites in all layers

	Layer*	getCurrentLayer(); // get current layer
	Layer*	getNextLayer();	// get the next layer from level, if no more gets the first one again
	Layer*	getPreviousLayer(); // get the previous layer from level, if no more gets the last one again

	SpriteBatch *getCurrentSpriteBatch(); // get current spritebatch we use to draw

	string	getName(); // returns the level name
	void	setName(string name); // sets the level name
	string  getActiveLayer(); // gets the name of the currently active layer
	void	addLayer(string name, float hSpeed, float vSpeed, float zDepth); // adds a new layer to the map
	void	addLayer(string name, float hSpeed, float vSpeed, float zDepth, int id); // adds a new layer to the map

	void    traceInfo(); // prints in logs level details

	SpriteBatch* addNewSpriteBatch(string textureAtlasFileName); // adds a spritebatch to be used withing the level
	SpriteBatch* addSpriteBatch(string textureAtlasFileName); // adds a spritebatch to be used withing the level

	vector<SpriteBatch*> *getSpriteBatches();  // gets the list with all spritebatches linked to current level
	vector<Layer*>		 *getLayers(); // return a vector that include all layers of the level
};

#endif