/*
* SpriteBatch.h
*
*  Created on: Oct 18, 2013
*      Author: nikmes
*/

#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include <map>
#include <vector>
#include <string>
#include <iostream>

#include <tinyxml/tinyxml.h>
#include "sprite.h"
#include "layer.h"

using namespace std;

class Sprite;
class Layer;

class SpriteBatch
{
private:
	void read_attribs(TiXmlElement* pElement, char *elementName);
	void read_xml_file(TiXmlNode* pParent);

public:

	SpriteBatch();
	SpriteBatch(string fileName);
	SpriteBatch(string fileName, string name);
	~SpriteBatch();

	vector<Layer*>  m_layers;
	map<string, Sprite> spriteMap;													// store all sprites of spritebatch in dictionary

	int		m_currentLayer;															// current layer under edit
	string  m_lastSelectedSprite;													// last srpite used to draw something from this batch
	string	m_textureFileName;														// the filename of png file that holds the real texture
	int		totalSprites;															// total sprites in spritebatch		
	string	m_name;																	// the name of the spritebatch
	string	m_xmlFileName;

	void	beginDraw();															// initialize drawing for this spritebatch
	void	endDraw();																// finalize drawing for this spritebatch
	Sprite	getSprite(string spriteName);											// Returns a sprite based on sprite name if it exists in the dictionary
	Sprite* getCollidingSpriteMap(int mouseX, int mouseY);							// Checks if any of the sprites in the batch is colliding with the mouse
	string	getFileName();															// Returns the xml filename of the spritebatch
	Layer*	addLayer(string name, float hSpeed, float vSpeed, float zDepth);		// adds a new layer to the map
	Layer*	addLayer(string name, float hSpeed, float vSpeed, float zDepth, int id);// adds a new layer to the map
	void    deleteLayer(string name);												// delete layer by name
	Layer*	getNextLayer();															// get the next layer from level, if no more gets the first one again
	Layer*	getPreviousLayer();														// get the previous layer from level, if no more gets the last one again
	Layer*	getCurrentLayer();														// get current layer
	int     getTotalLayers();														// returns total number of layers in spritebatch
	int     getTotalSprites();														// returns total number of sprites in spritebatch
	
	map<string, Sprite>* getAllSprites();										
	vector<Layer*> *getLayers();
	Layer* getLayer(int id);


};

#endif /* SPRITEBATCH_H_ */
