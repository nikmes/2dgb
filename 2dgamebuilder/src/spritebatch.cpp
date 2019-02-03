/*
* SpriteBatch.cpp
*
*  Created on: Oct 18, 2013
*      Author: nikmes
*/

#include "servicelocator.h"
#include "spritebatch.h"

SpriteBatch::SpriteBatch()
{

}

SpriteBatch::~SpriteBatch()
{
	this->spriteMap.clear();
}

SpriteBatch::SpriteBatch(string fileName)
{
	m_currentLayer = 0;
	// Load Texture Packer xml file
	this->totalSprites = 0;

	string fullPath	= ServiceLocator::getGameService()->m_directory;
	fullPath.append("texturemaps/");
	fullPath.append(fileName);

	TRACE(T_DEB, "Will attempt to load [%s]", fullPath.c_str());

	TiXmlDocument *doc = new TiXmlDocument(fullPath.c_str());
	m_xmlFileName = fileName; // the filename ie "sprites.xml"
	m_name = fileName.substr(0, fileName.length() - 3); // the filename without the extension i.e "sprites"
	m_name = ServiceLocator::getHelpService()->SplitFilename(m_name);
	doc->LoadFile();
	read_xml_file(doc);
	doc->Clear();
	delete doc;
}

SpriteBatch::SpriteBatch(string fileName, string name)
{
	/*
	* Load Texture Packer xml file
	*/
	m_currentLayer = 0;

	this->totalSprites = 0;
	this->m_name = name;
	string fullPath = ServiceLocator::getGameService()->m_directory;

	if (name != "preview")
	{
		fullPath.append("texturemaps/");
		fullPath.append(fileName);
	}
	
	fullPath.append(fileName);
	TiXmlDocument *doc = new TiXmlDocument(fullPath.c_str());
	m_xmlFileName = fileName; // the filename ie "sprites.xml"	this->m_xmlFileName = fileName;	// the filename ie "sprites.xml"
	this->m_name = fileName.substr(0, fileName.length() - 4); // the filename without the extension i.e "sprites"
	m_name = name; 
	doc->LoadFile();
	read_xml_file(doc);
	doc->Clear();
	this->m_xmlFileName = fileName;
}

string SpriteBatch::getFileName()
{
	return this->m_xmlFileName;
}

Sprite SpriteBatch::getSprite(string spriteName)
{
	// Returns the sprite with name = to spriteName if it exists
#if defined(PLATFORM_PS3)
	return this->spriteMap.find(spriteName)->second;
#else
	return this->spriteMap.at(spriteName);
#endif

}

void SpriteBatch::beginDraw()
{
	// begin the rendering loop of current spritebatch by loading its texture in GPU
	
	g2dBeginRects(ServiceLocator::getTextureService()->GetTexture(m_textureFileName));

	g2dPop();

	g2dPush();

	g2dSetCoordMode(G2D_CENTER);

}

void SpriteBatch::endDraw()
{
	// end rendering loop of current spritebatch

	g2dEnd();
}

Sprite* SpriteBatch::getCollidingSpriteMap(int mouseX, int mouseY)
{

	/*
	* Returns the sprite that is colliding with the mouse pointer
	* Sets that sprite to full brightnes while maintaining the rest of them to dark
	*/

	map<string, Sprite>::iterator it;

	for (it = spriteMap.begin(); it != spriteMap.end(); it++)
	{
		if (it->second.visible)
		{
			if ((mouseX > it->second.x && mouseX < it->second.x + it->second.sW) && (mouseY > it->second.y && mouseY < it->second.y + it->second.sH))
			{
				return &(*it).second;
			}
			else
			{
				continue;
			}
		}
	}

	return NULL;
}

void SpriteBatch::read_attribs(TiXmlElement* pElement, char *elementName)
{
	/*
	* Reading Texture Packer xml definition files and load texture Atlas image and sprite coordinates
	*/

	if (!pElement)
	{
		TRACE(T_DEB,"Unable to load the spritebatch file %s", this->m_xmlFileName.c_str());
		TRACE(T_DEB, "Looks like there is an xml syntax error.... ");
		return;
	}

	Sprite currentSprite;

	TiXmlAttribute* pAttrib = pElement->FirstAttribute();

	while (pAttrib)
	{
		// Handle Texture Atlas

		if (!strcmp(elementName, "TextureAtlas"))
		{
			if (!strcmp(pAttrib->Name(), "imagePath"))
			{
				m_textureFileName = (char*)pAttrib->Value();
				ServiceLocator::getTextureService()->GetTexture(m_textureFileName.c_str());
			}
			else if (!strcmp(pAttrib->Name(), "width"))
			{

			}
			else if (!strcmp(pAttrib->Name(), "height"))
			{

			}
		}

		// Handle Sprite

		if (!strcmp(elementName, "sprite"))
		{
			/*
			* Create a new Sprite and populate its fields or even better,
			* call Sprite with default constructor that takes arguments all the below
			*/

			currentSprite.setColor(WHITE);

			if (!strcmp(pAttrib->Name(), "n"))
			{
				currentSprite.name.append(pAttrib->Value());
			}
			else if (!strcmp(pAttrib->Name(), "x"))
			{
				currentSprite.cX = atoi(pAttrib->Value());
			}
			else if (!strcmp(pAttrib->Name(), "y"))
			{
				currentSprite.cY = atoi(pAttrib->Value());
			}
			else if (!strcmp(pAttrib->Name(), "w"))
			{
				currentSprite.cW = (float)atoi(pAttrib->Value());
				currentSprite.sW = currentSprite.cW;
			}
			else if (!strcmp(pAttrib->Name(), "h"))
			{
				currentSprite.cH = (float)atoi(pAttrib->Value());
				currentSprite.sH = currentSprite.cH;
			}
			else if (!strcmp(pAttrib->Name(), "oX"))
			{

			}
			else if (!strcmp(pAttrib->Name(), "oY"))
			{

			}
			else if (!strcmp(pAttrib->Name(), "oW"))
			{

			}
			else if (!strcmp(pAttrib->Name(), "oH"))
			{

			}
			else if (!strcmp(pAttrib->Name(), "hint"))
			{
				currentSprite.hint = (char*)pAttrib->Value();
			}
		}
		pAttrib = pAttrib->Next();
	}

	// Add the sprite to the sprites dictionary

	if (!strcmp(elementName, "sprite"))
	{
		// Add the sprite to the dictionary

		spriteMap.insert(pair<string, Sprite>(currentSprite.name, currentSprite));

		if (m_name != "preview")
		{
			/* !!!!!!!!! HACKISH IMPLEMENTATION OF TEXREPEAT !!!!!!!!!!!*/
			ServiceLocator::getTextureService()->copy2texture(currentSprite.cX, currentSprite.cY, currentSprite.cW, currentSprite.cH, currentSprite.name, ServiceLocator::getTextureService()->GetTexture(this->m_textureFileName), G2D_SWIZZLE);
		}

		// Add the sprite to the list (maybe not needed, have to check the code)

		//spriteList.push_back(currentSprite);

		// Increase sprite counter by one
		this->totalSprites++;
	}
}

void SpriteBatch::read_xml_file(TiXmlNode* pParent)
{
	if (!pParent)
	{
		return;
	}

	TiXmlNode* pChild;

	int t = pParent->Type();

	switch (t)
	{

	case TiXmlNode::TINYXML_DOCUMENT:
		break;

	case TiXmlNode::TINYXML_ELEMENT:

		if (!strcmp(pParent->Value(), "TextureAtlas") || !strcmp(pParent->Value(), "sprite"))
		{
			read_attribs(pParent->ToElement(), (char *)pParent->Value());
		}

		break;

	case TiXmlNode::TINYXML_COMMENT:
		break;

	case TiXmlNode::TINYXML_UNKNOWN:
		break;

	case TiXmlNode::TINYXML_TEXT:
		break;

	case TiXmlNode::TINYXML_DECLARATION:
		break;

	default:
		break;

	}

	for (pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
	{
		read_xml_file(pChild);
	}

}

map<string,Sprite>* SpriteBatch::getAllSprites()
{
	return &spriteMap;
}

Layer* SpriteBatch::addLayer(string name, float hSpeed, float vSpeed, float zDepth)
{
	TRACE(T_DEB, "Layer [%s] will be added to the SpriteBatch [%s]", name.c_str(), m_xmlFileName.c_str());

	Layer *newLayer = new Layer(name, hSpeed, vSpeed, zDepth);

	m_layers.push_back(newLayer);

	return newLayer;
}

Layer* SpriteBatch::addLayer(string name, float hSpeed, float vSpeed, float zDepth, int id)
{
	TRACE(T_DEB, "Layer [%s] will be added to the SpriteBatch [%s]", name.c_str(), m_xmlFileName.c_str());

	Layer *newLayer = new Layer(name, hSpeed, vSpeed, zDepth, id);

	m_layers.push_back(newLayer);

	return newLayer;
}



int SpriteBatch::getTotalLayers()
{
	// returns the number of layers in the map

	return this->m_layers.size();
}

Layer* SpriteBatch::getNextLayer()
{
	int i = 0;

	vector<Layer*>::iterator it;

	if (m_currentLayer >= this->getTotalLayers() - 1)
	{
		m_currentLayer = 0;
	}
	else
	{
		this->m_currentLayer++;
	}

	for (it = m_layers.begin(); it != m_layers.end(); it++)
	{
		if (i == m_currentLayer)
		{
			return (*it);
		}
		i++;
	}

	return NULL;
}

Layer* SpriteBatch::getCurrentLayer()
{
	int i = 0;

	vector<Layer*>::iterator it;

	for (it = m_layers.begin(); it != m_layers.end(); it++)
	{
		if (i == m_currentLayer)
		{
			return (*it);
		}

		i++;
	}

	return NULL;
}

Layer* SpriteBatch::getPreviousLayer()
{
	int i = 0;

	vector<Layer*>::iterator it;

	if (m_currentLayer <= 0)
	{
		m_currentLayer = this->getTotalLayers() - 1;
	}
	else
	{
		this->m_currentLayer--;
	}

	for (it = m_layers.begin(); it != m_layers.end(); it++)
	{
		if (i == m_currentLayer)
		{
			return (*it);
		}
		i++;
	}

	return NULL;
}

vector<Layer*> *SpriteBatch::getLayers()
{
	return &m_layers;
}

Layer *SpriteBatch::getLayer(int id)
{
	vector<Layer*>::iterator it;

	for (it = m_layers.begin(); it != m_layers.end(); it++)
	{
		if ((*it)->id==id) 
		{
			return (*it);
		}
		else
		{
			return NULL;
		}
	}

}