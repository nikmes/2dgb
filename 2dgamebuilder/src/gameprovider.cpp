#include "servicelocator.h"
#include <algorithm>

GameProvider::GameProvider()
{
	sprintf(m_gameName, "%s", "");
	sprintf(m_levelName,"%s", "");
	sprintf(m_layerName,"%s", "");
	sprintf(m_gamePath, "[]");
	sprintf(m_textureAtlasName, "%s", "");
	sprintf(m_levelStaticBackground, "%s", "");

	m_gameCreated = false;
	m_gameSaved = false;
	m_layerHorizontalSpeed = 1.0;
	m_layerVerticalSpeed = 1.0;
	m_layerDepth = 1;
	m_layerActive = true;
	m_currentLevel = 0;
	m_levelStaticBackgroundIsVisible = false;
}

GameProvider::~GameProvider()
{

}

void GameProvider::createNewGame()
{
	//TRACE(T_DEB, "This is from Project GAME_PATH=%s", GAME_PATH);

	TRACE(T_INF, "Game Name:        [%s]", m_gameName);
	TRACE(T_INF, "Game Path:        [%s]", m_gamePath);
	TRACE(T_INF, "Game Level:       [%s]", m_levelName);
	TRACE(T_INF, "Game Layer:       [%s]", m_layerName);
	TRACE(T_INF, "Game SpriteBatch: [%s]", m_textureAtlasName);

	if (!m_gameCreated)
	{

		bool parentDirCreated = false;
		string gameDir = "";
		string currentFile = "";

		gameDir.append(m_gamePath);
		gameDir.append(m_gameName);
		gameDir.append("/");
		parentDirCreated = utils::make_directory(gameDir);
		
		m_directory = gameDir;

		if (parentDirCreated)
		{
			// save the actuall xml that describes the game
			currentFile = gameDir;
			currentFile.append(m_gameName);
			currentFile.append(".xml");
			m_fileName = currentFile;

			currentFile = gameDir;
			currentFile.append("levels");
			utils::make_directory(currentFile);

			currentFile = gameDir;
			currentFile.append("texturemaps");
			utils::make_directory(currentFile);

			currentFile = gameDir;
			currentFile.append("music");
			utils::make_directory(currentFile);

			currentFile = gameDir;
			currentFile.append("sounds");
			utils::make_directory(currentFile);

			currentFile = gameDir;
			currentFile.append("images");
			utils::make_directory(currentFile);

			currentFile = gameDir;
			currentFile.append("fonts");
			utils::make_directory(currentFile);
		}

		// add the level to the game
		this->addNewLevel(m_levelName);
	
		this->saveGame();
	}
	else
	{
		// do you want to save current game?
		// if yes 
		//  save current game
		//  close current game 
		//  create new game 
		// else 
		// if no 
		//   do nothing
		// endif
	}
}

void GameProvider::closeGame()
{

}

string GameProvider::getGameName()
{
	return m_gameName;
}

string GameProvider::getGameFileName()
{
	return m_gameFileName;
}

bool GameProvider::gameIsCreated()
{
	return m_gameCreated;
}

void GameProvider::addLevel(string levelName)
{
	// add a new level to existing game
	Level *newLevel = new Level(levelName);
	newLevel->m_backgroundFileName = m_levelStaticBackground;
	newLevel->m_fileName = levelName;
	newLevel->m_fileName.append(".xml");
	m_levels.push_back(newLevel);
}

void GameProvider::addNewLevel(string levelName)
{
	// Copy the 1st Level from game creation wizard to the levels
	Level *newLevel = new Level(levelName);
	newLevel->m_backgroundFileName = m_levelStaticBackground;
	newLevel->m_fileName = m_levelName;
	newLevel->m_fileName.append(".xml");
	m_levels.push_back(newLevel);
	
	/*
	 * Physicaly copy the .ta and .png files to the game/texturemaps directory
	 */

	// Copy texture atlas xml definition to game_path/texturemaps
	string textureAtlasNewPath = m_directory;
	textureAtlasNewPath.append("texturemaps/");
	string textureAtlasFileName = utils::SplitFilename(m_textureAtlasName);
	textureAtlasNewPath.append(textureAtlasFileName);
	TRACE(T_INF, "Copy from [%s] to [%s]", m_textureAtlasName, textureAtlasNewPath.c_str());
	utils::copy_file(m_textureAtlasName, textureAtlasNewPath.c_str());

	// Copy texture atlas png file to game_path/texturemaps
	textureAtlasNewPath = utils::RemoveExtension(textureAtlasNewPath);
	textureAtlasNewPath.append(".png");
	string textureAtlasPNGFileName = utils::RemoveExtension(m_textureAtlasName);
	textureAtlasPNGFileName.append(".png");
	TRACE(T_INF, "Copy from [%s] to [%s]", textureAtlasPNGFileName.c_str(), textureAtlasNewPath.c_str());
	utils::copy_file(textureAtlasPNGFileName.c_str(), textureAtlasNewPath.c_str());
	
	// Now we copied the files physicaly we can add spriteBatch to the created level

	newLevel->addNewSpriteBatch(textureAtlasFileName);

	// add layer to spritebatch
	newLevel->addLayer(m_layerName, m_layerHorizontalSpeed, m_layerVerticalSpeed, m_layerDepth);
	
	/*
	 * Physicaly copy static background to game/images directory
	 */

	// Copy level static background selected to game_path/images
	string destBackgroundFile = m_directory;
	destBackgroundFile.append("images/");
	destBackgroundFile.append(utils::SplitFilename(newLevel->m_backgroundFileName));
	if (!newLevel->m_backgroundFileName.empty())
	{
		TRACE(T_INF, "Copy from [%s] to [%s]", newLevel->m_backgroundFileName.c_str(), destBackgroundFile.c_str());
		utils::copy_file(newLevel->m_backgroundFileName.c_str(), destBackgroundFile.c_str());
	}

	TRACE(T_DEB, "Level added with name [%s] and Layer name [%s] with filename [%s] was created", m_levelName, m_layerName, newLevel->m_fileName.c_str());

	TRACE(T_DEB, "Layer has textureatlas with filename [%s]", m_textureAtlasName);
	vector<SpriteBatch*> *sprBatches = newLevel->getSpriteBatches();

	//ServiceLocator::getImguiService()->spriteSelector->setSpriteBatch(this->getCurrentSpriteBatch());
	ServiceLocator::getImguiService()->spriteSelector->isVisible = true;
}

void GameProvider::addExistingLevel(string levelFileName, string levelName)
{
	// Copy the 1st Level from game creation wizard to the levels
	Level *newLevel = new Level(levelName);
	newLevel->m_backgroundFileName = m_levelStaticBackground;
	newLevel->m_fileName = utils::SplitFilename(levelFileName);
	m_levels.push_back(newLevel);
	newLevel->load(levelFileName);
}

void GameProvider::deleteLevel(string levelName)
{
	// 1. get current level
	// 2. delete current level
	// 3. make current next level

}

int GameProvider::getTotalLevels()
{
	return this->m_levels.size();
}

Level *GameProvider::getCurrentLevel()
{
	int i = 0;

	vector<Level*>::iterator it;

	for (it = m_levels.begin(); it != m_levels.end(); it++)
	{
		if (i == m_currentLevel)
		{
			return (*it);
		}

		i++;
	}
	return NULL;
}

Level *GameProvider::getNextLevel()
{
	int i = 0;

	vector<Level*>::iterator it;

	if (m_currentLevel >= this->getTotalLevels() - 1)
	{
		m_currentLevel = 0;
	}
	else
	{
		this->m_currentLevel++;
	}

	for (it = m_levels.begin(); it != m_levels.end(); it++)
	{
		if (i == m_currentLevel)
		{		
			ServiceLocator::getImguiService()->spriteSelector->setSpriteBatch((*it)->getCurrentSpriteBatch());
			return (*it);
		}
		i++;
	}

	return NULL;
}

Level *GameProvider::getPreviousLevel()
{
	int i = 0;

	vector<Level*>::iterator it;

	if (m_currentLevel <= 0)
	{
		m_currentLevel = this->getTotalLevels() - 1;
	}
	else
	{
		this->m_currentLevel--;
	}

	for (it = m_levels.begin(); it != m_levels.end(); it++)
	{
		if (i == m_currentLevel)
		{
			return (*it);
		}
		i++;
	}

	return NULL;
}

SpriteBatch* GameProvider::getCurrentSpriteBatch()
{
	//return m_currentSpriteBatch;
	return (this->getCurrentLevel()->getCurrentSpriteBatch());
}

vector<Sprite*> GameProvider::getColSpriteList()
{
	vector<SpriteBatch*> *sprBatches;

	Level *currLevel = this->getCurrentLevel();
	
	sprBatches = currLevel->getSpriteBatches();

	vector<SpriteBatch*>::iterator it;
	
	vector<Layer*>::iterator lyIt;

	vector<Sprite*> finalColSpritesList;

	// for all spritebatches of level

	for (it = sprBatches->begin(); it != sprBatches->end(); it++)
	{
		// for current layer of each sprite batch get colliding sprites 

//		for (lyIt = (*it)->m_layers.begin(); lyIt != (*it)->m_layers.end(); lyIt++)
//		{
//			vector<Sprite*> colSpritesList = (*lyIt)->getCollidingSprites();

			vector<Sprite*> colSpritesList = (*it)->getCurrentLayer()->getCollidingSprites();

			finalColSpritesList.insert(finalColSpritesList.end(), colSpritesList.begin(), colSpritesList.end());
//		}
	}

	return finalColSpritesList;
}

void GameProvider::deleteColSprite(Sprite *colSpr)
{
	// if we stre on sprite the layerID and SpriteBatch ID (or pointer to them) we dont have to do all this loops to search

	vector<SpriteBatch*> *sprBatches;
	Level *currLevel = this->getCurrentLevel();
	sprBatches = currLevel->getSpriteBatches();
	vector<SpriteBatch*>::iterator it;
	vector<Layer*>::iterator lyIt;
	vector<Sprite>::iterator sprIt;
	bool spriteFound = false;

	for (it = sprBatches->begin(); it != sprBatches->end(); it++)
	{
		for (lyIt = (*it)->m_layers.begin(); lyIt != (*it)->m_layers.end(); lyIt++)
		{
			for (sprIt = (*lyIt)->spriteList.begin(); sprIt != (*lyIt)->spriteList.end(); sprIt++)
			{
				if (sprIt->id == colSpr->id)
				{
					(*lyIt)->deleteSprite(colSpr);
					spriteFound = true;
					break;
				}
			}
			if (spriteFound) break;
		}
		if (spriteFound) break;
	}
}

void GameProvider::updateSpritePositions(float step)
{
	vector<SpriteBatch*> *sprBatches;
	Level *currLevel = this->getCurrentLevel();
	sprBatches = currLevel->getSpriteBatches();
	vector<SpriteBatch*>::iterator it;

	it = sprBatches->begin();
	Layer *newLayer = (*it)->getCurrentLayer();

	newLayer->updateSpritePos(step);

	for (it = sprBatches->begin() + 1; it != sprBatches->end(); it++)
	{
		newLayer = (*it)->getCurrentLayer();

		newLayer->updateSpritePos(step);
	}

}

void GameProvider::setLayerCameraPos(point cPos)
{
	vector<SpriteBatch*> *sprBatches;

	Level *currLevel = this->getCurrentLevel();

	sprBatches = currLevel->getSpriteBatches();

	vector<SpriteBatch*>::iterator it;

	vector<Layer*>::iterator lyIt;

	for (it = sprBatches->begin(); it != sprBatches->end(); it++)
	{
		// for all layers of sprite batch get colliding sprites
		for (lyIt = (*it)->m_layers.begin(); lyIt != (*it)->m_layers.end(); lyIt++)
		{
			(*lyIt)->setLayerCameraPos(cPos);
		}
	}
}

void GameProvider::updateLayerPosition(Orientation orient, float step)
{
	// this is if we want to move the all layer at once using cursor keys
	// this will move the actual layer not the camera

	vector<SpriteBatch*> *sprBatches;
	Level *currLevel = this->getCurrentLevel();
	sprBatches = currLevel->getSpriteBatches();
	vector<SpriteBatch*>::iterator it;

	it = sprBatches->begin();
	Layer *newLayer = (*it)->getCurrentLayer();

	if (orient == HORIZONTAL)
	{
		newLayer->updateLayerPosX(step);
	}
	else if (orient == VERTICAL)
	{
		newLayer->updateLayerPosY(step);
	}

	for (it = sprBatches->begin()+1; it != sprBatches->end(); it++)
	{
			newLayer = (*it)->getCurrentLayer();
			
			if (orient == HORIZONTAL)
			{
				newLayer->updateLayerPosX(step);
			}
			else if (orient == VERTICAL)
			{
				newLayer->updateLayerPosY(step);
			}
	}
}

Layer* GameProvider::getCurrentLayer()
{
	// this logic is wrong cause there is no correct logic for getCurrentSpriteBatch

	Level *currentLevel = this->getCurrentLevel();

	if (currentLevel)
	{
		SpriteBatch *currentSpriteBatch = currentLevel->getCurrentSpriteBatch();

		if (currentSpriteBatch)
		{
			return (currentSpriteBatch->getCurrentLayer());
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}

	// 1. for all spritebatch get their current layer
	// 2. 
}

void GameProvider::getNextLayer() 
{
	// advance current layer indicator to all spritebatches

	vector<SpriteBatch*> *sprBatches;
	Level *currLevel = this->getCurrentLevel();
	sprBatches = currLevel->getSpriteBatches();
	vector<SpriteBatch*>::iterator it;

	for (it = sprBatches->begin(); it != sprBatches->end(); it++)
	{
		(*it)->getNextLayer(); 
	}
}

void GameProvider::getPreviousLayer()
{
	// revert current layer indicator to all spritebatches

	vector<SpriteBatch*> *sprBatches;
	Level *currLevel = this->getCurrentLevel();
	sprBatches = currLevel->getSpriteBatches();
	vector<SpriteBatch*>::iterator it;

	for (it = sprBatches->begin(); it != sprBatches->end(); it++)
	{
		(*it)->getPreviousLayer();
	}
}
/*
void GameProvider::addLayer(string name, float hSpeed, float vSpeed, float zDepth)
{
	// 1. retrieve the current Level
	// 2. retrieve list of sprite batches current level has
	// 3. add the layer to the first batch, then u have the id of the layer added
	// 4. to each of the sprite batches add the new layer with the given properties and same id as one obtained at step 3

	Level *currLevel = this->getCurrentLevel();

	TRACE(T_DEB, "Layer [%s] will be added to all SpriteBatches of current Level [%s]", name.c_str(), currLevel->m_name.c_str());

	vector<SpriteBatch*> *sprBatches;

	sprBatches = currLevel->getSpriteBatches();

	vector<SpriteBatch*>::iterator it;

	it = sprBatches->begin();
	Layer *newLayer = (*it)->addLayer(name, hSpeed, vSpeed, zDepth);

	for (it = sprBatches->begin() + 1; it != sprBatches->end(); it++)
	{
		TRACE(T_INF, "Adding Layer [%s] to spritebatch [%s][%s]", name.c_str(), (*it)->m_name.c_str(), (*it)->m_xmlFileName.c_str());
		(*it)->addLayer(name, hSpeed, vSpeed, zDepth,newLayer->id);
	}
}

void GameProvider::addLayer(string name, float hSpeed, float vSpeed, float zDepth, int id)
{
	// 1. retrieve the current Level
	// 2. get all layers and check if we try to add a layer with id that already exists
	// 3. if it already exists dont do anything just return
	// 2. retrieve list of sprite batches current level has
	// 3. to each of the sprite batches add the new layer with the given properties
	
	Level *currLevel = this->getCurrentLevel();
	
	vector<Layer *> *allLayers = currLevel->getLayers();
	vector<Layer*>::iterator itL;

	// 1. 
	for (itL = allLayers->begin(); itL != allLayers->end(); itL++)
	{
		TRACE(T_INF, "Checking Layer [%s] with id[%d] ", (*itL)->name.c_str(), (*itL)->id);

		if (id == (*itL)->id)
		{
			return;
		}
	}
	
	TRACE(T_DEB, "Layer [%s] will be added to all SpriteBatches of current Level [%s]", name.c_str(), currLevel->m_name.c_str());

	vector<SpriteBatch*> *sprBatches;

	sprBatches = currLevel->getSpriteBatches();

	vector<SpriteBatch*>::iterator it;

	for (it = sprBatches->begin(); it != sprBatches->end(); it++)
	{
		TRACE(T_INF, "Adding Layer [%s] to spritebatch [%s][%s]", name.c_str(), (*it)->m_name.c_str(), (*it)->m_xmlFileName.c_str());
		(*it)->addLayer(name, hSpeed, vSpeed, zDepth);
	}
}
*/
void GameProvider::deleteLayer(string name)
{
	// 1. retrieve list of sprite batch current level has
	// 2. for each of the sprite batches find and delete the layer
}

void GameProvider::loadGame(string fileName)
{

	m_fileName = fileName.c_str();

	TRACE(T_DEB, "Loading game from %s", fileName.c_str());

	TiXmlDocument *doc = new TiXmlDocument(fileName.c_str());

	doc->LoadFile();

	TiXmlElement* root = doc->FirstChildElement();

	if (root == NULL)
	{
		TRACE(T_DEB, "Failed to load file: No root element");
		doc->Clear();
		return;
	}
	
	string elemName = root->Value();

	TRACE(T_DEB, "Root Element Name: [%s]", elemName.c_str());

	if (elemName == "game")
	{
		TRACE(T_DEB, "Node: [%s] with attributes: name[%s] path[%s]", elemName.c_str(), root->Attribute("name"), root->Attribute("path"));
		
		strcpy(this->m_gameName, root->Attribute("name"));

		this->m_directory = root->Attribute("path");

		for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			string elemName = elem->Value();

			TRACE(T_DEB, "Node: [%s]", elemName.c_str());

			if (elemName == "levels")
			{
				for (TiXmlElement* elem2 = elem->FirstChildElement(); elem2 != NULL; elem2 = elem2->NextSiblingElement())
				{
					TRACE(T_DEB, "    Node: [%s] with attributes: name[%s] filename[%s]", elem2->Value(), elem2->Attribute("name"), elem2->Attribute("filename"));
					// load the level
					string fullPath = this->m_directory;
					fullPath.append(elem2->Attribute("filename"));

					TRACE(T_INF, "    Load existing level named [%s] from file [%s]", elem2->Attribute("name"), fullPath.c_str());
					this->addExistingLevel(fullPath, elem2->Attribute("name"));

					//this->getCurrentLevel()->load(fullPath.c_str());
				}
			}

			if (elemName == "assets")
			{
				for (TiXmlElement* elem2 = elem->FirstChildElement(); elem2 != NULL; elem2 = elem2->NextSiblingElement())
				{
					string elem2Name = elem2->Value();

					TRACE(T_DEB, "    Node: [%s]", elem2Name.c_str());

					// load any texturepacks that they will be used during intro state or menu state

					if (elem2Name == "texturemaps")
					{

					}

					// load any music that they will be used during intro state or menu state

					if (elem2Name == "music")
					{

					}

					// load any sounds that they will be used during intro state or menu state

					if (elem2Name == "sounds")
					{

					}

					// load any images that they will be used during intro state or menu state

					if (elem2Name == "images")
					{

					}

					// load any fonts that they will be used during intro state or menu state

					if (elem2Name == "fonts")
					{

					}
				}
			}
		}
	}
	
	TRACE(T_DEB, "Game loaded succesfully");
	m_gameCreated = true;

	//ServiceLocator::getImguiService()->spriteSelector->setSpriteBatch(this->getCurrentSpriteBatch());
	ServiceLocator::getImguiService()->spriteSelector->isVisible = true;  

}

void GameProvider::saveGame()
{
	gameDoc = new TiXmlDocument(); // to save game.xml
	//levelDoc = new TiXmlDocument(); // to save level.xml

	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	gameDoc->LinkEndChild(decl);

	// create the game root node for game.xml and add it as root node
	TiXmlElement *game = new TiXmlElement("game");
	game->SetAttribute("name", m_gameName); // top xml level node
	game->SetAttribute("path", m_directory.c_str());
	gameDoc->LinkEndChild(game);

	// traverse all levels and create entries for each one in game xml
	vector<Level*>::iterator levelIt;
	vector<Layer*>::iterator layerIt;
	vector<Sprite>::iterator spriteIt;

	TiXmlElement *levelsNode = new TiXmlElement("levels");
	game->LinkEndChild(levelsNode);

	for (levelIt = this->m_levels.begin(); levelIt != m_levels.end(); levelIt++)
	{
		string levelFilePath;
		string levelFullPath;

		// for each level traverse all layers and save xml that describes layer

		Level *currLevel = (*levelIt);
		TRACE(T_INF, "Current Level:");
		TRACE(T_INF, "    Name     : %s", currLevel->m_name.c_str());
		TRACE(T_INF, "    Layers   : %d", currLevel->getTotalLayers());
		TRACE(T_INF, "    Sprites  : %d", currLevel->getTotalSprites());
		TRACE(T_INF, "    FileName : %s", currLevel->m_fileName.c_str());

		// create the level node and add it under the game.xml root node 
		TiXmlElement *levelNode = new TiXmlElement("level");
		levelFilePath = "levels/";
		levelFilePath.append(currLevel->m_name.c_str());
		levelFilePath.append(".xml");
		levelNode->SetAttribute("name", (*levelIt)->m_name.c_str());
		levelNode->SetAttribute("filename", levelFilePath.c_str());
		levelsNode->LinkEndChild(levelNode);
	}
	
	//game->LinkEndChild(gNode);
	TiXmlElement *gNode = new TiXmlElement("assets");
	game->LinkEndChild(gNode);

	TiXmlElement *texPackNode = new TiXmlElement("texturepacks");
	gNode->LinkEndChild(texPackNode);

	TiXmlElement *musicNode = new TiXmlElement("music");
	gNode->LinkEndChild(musicNode);

	TiXmlElement *soundsNode = new TiXmlElement("sounds");
	gNode->LinkEndChild(soundsNode);

	TiXmlElement *imagesNode = new TiXmlElement("images");
	gNode->LinkEndChild(imagesNode);

	TiXmlElement *fontsNode = new TiXmlElement("fonts");
	gNode->LinkEndChild(fontsNode);

	TRACE(T_INF, "Saving game in [%s] as [%s]", m_directory.c_str(), m_fileName.c_str());

	gameDoc->SaveFile(m_fileName.c_str());
	
	// save all level xml defintion files

	vector <Level*>::iterator it;

	for (it = m_levels.begin(); it != m_levels.end(); it++)
	{
		(*it)->saveAs("test");
	}

	m_gameCreated = true;

}

vector<Level*>* GameProvider::getLevels()
{
	return &m_levels;
}