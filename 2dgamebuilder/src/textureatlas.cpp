#include "textureatlas.h"
#include "servicelocator.h"

TextureAtlas::TextureAtlas(string fileName)
{
	// load the xml file that describes texture atlas
	TiXmlDocument doc(fileName.c_str());

	m_xmlFileName = fileName; // the filename ie "sprites.xml"

	m_name = fileName.substr(0, fileName.length() - 4); // the filename without the extension i.e "sprites"

	if (doc.LoadFile())
	{
		TRACE(T_INF, "Texture Atlas loaded file %s", this->m_xmlFileName.c_str());
	}
	else
	{
		TRACE(T_INF, "Texture Atlas is unalbe to load file %s", this->m_xmlFileName.c_str());
	}
	read_xml_file(&doc);
}

TextureAtlas::~TextureAtlas()
{

}

g2dTexture* TextureAtlas::getTexture()
{
	return this->m_texture;
}

textureAtlasItem TextureAtlas::getItem(string name)
{
	return mapItems.at(name);
}

string TextureAtlas::getXMLFileName()
{
	return this->m_xmlFileName;
}

string TextureAtlas::getTexFileName()
{
	return this->m_texFileName;
}

string TextureAtlas::getName()
{
	return this->m_name;
}

void TextureAtlas::read_attribs(TiXmlElement* pElement, char *elementName)
{
	static int id = 1;
	/*
	* Reading Texture Packer xml definition files and load texture Atlas image and sprite coordinates
	*/

	if (!pElement)
	{
		TRACE(T_ERR,"Unable to load the spritebatch file %s", this->m_xmlFileName.c_str());
		TRACE(T_ERR,"Looks like there is an xml syntax error.... ");
		return;
	}

	textureAtlasItem item;

	TiXmlAttribute* pAttrib = pElement->FirstAttribute();

	while (pAttrib)
	{
		// Handle Texture Atlas

		if (!strcmp(elementName, "TextureAtlas"))
		{
			if (!strcmp(pAttrib->Name(), "imagePath"))
			{
				m_texFileName = (char*)pAttrib->Value();
				string fullPath = "resources/";
				fullPath.append(m_texFileName);
				m_texture = ServiceLocator::getTextureService()->GetTextureFromFileSystem((char*)fullPath.c_str());
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

			if (!strcmp(pAttrib->Name(), "n"))
			{
				item.name.append(pAttrib->Value());
			}
			else if (!strcmp(pAttrib->Name(), "x"))
			{
				item.x = (float)atof(pAttrib->Value());
			}
			else if (!strcmp(pAttrib->Name(), "y"))
			{
				item.y = (float)atof(pAttrib->Value());
			}
			else if (!strcmp(pAttrib->Name(), "w"))
			{
				item.w = (float)atof(pAttrib->Value());
			}
			else if (!strcmp(pAttrib->Name(), "h"))
			{
				item.h = (float)atof(pAttrib->Value());
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
				item.hint = (char*)pAttrib->Value();
			}
		}
		pAttrib = pAttrib->Next();
	}

	// Add the sprite to the sprites dictionary

	if (!strcmp(elementName, "sprite"))
	{
		// Add the sprite to the dictionary
		id+=1;
		item.id = id;

		mapItems.insert(pair<string, textureAtlasItem>(item.name, item));
		
		vecItems.push_back(item);

		// Increase sprite counter by one
		//this->totalSprites++;
	}
}

vector<textureAtlasItem>* TextureAtlas::getAllItems()
{
	return &vecItems;
}

void TextureAtlas::read_xml_file(TiXmlNode* pParent)
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