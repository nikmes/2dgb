#ifndef LEVELPROVIDER_H
#define LEVELPROVIDER_H

#include <string>
#include <map>
#include <Box2D/Box2D.h>
#include "box2ddebugdraw.h"
#include "layer.h"

using namespace std;

class LevelProvider
{
private:
	vector<Layer*> layers; // list of layers
	int	m_currentLayer; // current layer under edit

public:

	// Box2d Related members
	Box2dDebugDraw  fooDrawInstance; // enable debug drawing of box2d shapes
	b2Vec2	*Gravity; // Gravity of the Box2d World
	b2World	*World; // The Box2d World
	b2Body	*playerBody; 
	b2Body	*weightBody;

	string	 m_name; // the name of the level, will be used also as filename when saving the map
	string   m_backgroundFileName; // name of png that holds the background picture
	bool	 m_backgroundVisible; // background png visible or not
	bool	 m_levelIsLoaded; // indicates whether the level is loaded or not

	LevelProvider(); // default constructor
	LevelProvider(string name); // custom constructor, accepts map name as a string
	~LevelProvider(); // default drstructor
	
	void	save(); // save the level in an xml file
	void	saveAs(string fileName); // save the level in a different filename
	void	load(string fileName); // loads the level from an xml file
	
	// Box2d Related Methods
	void	createPhysicalWorld(); // creates the box2d world
	int		CreateLine(float x1, float y1, float x2, float y2); // create a box2d collision line
	void	CreateBox(int MouseX, int MouseY); // add random box at mouse pointer
	void	CreatePlayer(float x, float y);	// creates our player in the level

	Layer*	getCurrentLayer(); // get current layer
	void	removeLayer(string name); // removes a layer from the map
	void	draw(); // draws the map on screen
	Layer*	getLayer(string name); // returns the layer that has name equals to incomming name
	int		getTotalLayers(); // returns the number of layers in the map
	int		getTotalSprites(); // returns the total number of sprites in all layers
	Layer*	getNextLayer();	// get the next layer from level, if no more gets the first one again
	Layer*	getPreviousLayer(); // get the previous layer from level, if no more gets the last one again
	string	getName(); // returns the level name
	void	setName(string name); // sets the level name
	string  getActiveLayer(); // gets the name of the currently active layer
	//void	addLayer(string name, string sbFileName, float hSpeed, float vSpeed, float zDepth); // adds a new layer to the map
	vector<Layer*> getLayers(); // return a vector that include all layers of the level
};

#endif