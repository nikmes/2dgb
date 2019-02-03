/**
* \date    14 / 02 / 2015
* \author  Nicholas Messaritis
*
* \file    maintoolbar.h
*
* Implementation of the main tool bar window
*
*/

#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include <string>
#include <vector>
#include <map>
#include <tinyxml/tinyxml.h>
#include "definitions.h"

using namespace std;

class TextureAtlas
{
private:
	
	map<string, textureAtlasItem> mapItems;
	vector<textureAtlasItem> vecItems;

	string	m_name;
	string	m_xmlFileName;
	string	m_texFileName;
	g2dTexture* m_texture;

	void read_attribs(TiXmlElement* pElement, char *elementName);
	void read_xml_file(TiXmlNode* pParent);

public:

	~TextureAtlas();
	
	TextureAtlas(string fileName);
	
	//textureAtlasItem* getItem(string name);

	textureAtlasItem getItem(string name);

	string getName();

	string getXMLFileName();

	string getTexFileName();

	g2dTexture* getTexture();

	vector<textureAtlasItem>* getAllItems();

};

#endif
