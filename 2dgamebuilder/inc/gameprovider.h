#ifndef GAMEPROVIDER_H
#define GAMEPROVIDER_H

#include <string>
#include "level.h"

using namespace std;

class GameProvider
{
private:

	vector<Level*> m_levels; // list of game levels

	int m_currentLevel;		 // current level under modification

	string m_gameFileName;   // the file name that will be used to save the game
	bool m_gameCreated;      // game was created from menu New Game
	bool m_gameSaved;		 // the game was saved at some point

	TiXmlDocument *gameDoc;
	TiXmlDocument *levelDoc;
	TiXmlDocument *subLevelDoc;

public:
	
	char   m_gamePath[256];
	char   m_gameName[64];
	char   m_levelName[64];
	char   m_levelStaticBackground[256];
	char   m_layerName[64];
	char   m_textureAtlasName[128];
	float  m_layerHorizontalSpeed;
	float  m_layerVerticalSpeed;
	int    m_layerDepth;
	
	string m_fileName;
	string m_directory;

	bool   m_layerActive;
	bool   m_levelStaticBackgroundIsVisible;



	GameProvider(); 

	~GameProvider();

	//  game related operations

	string getGameName();

	string getGameFileName();

	bool gameIsCreated();

	void closeGame();

	void createNewGame();

	// level related operations

	void addNewLevel(string levelName);

	void addExistingLevel(string levelFileName, string levelName);

	void deleteLevel(string levelName);
	
	Level *getCurrentLevel();

	Level *getNextLevel();

	Level *getPreviousLevel();

	int getTotalLevels();

	Layer *getCurrentLayer();

	//void addLayer(string name, float hSpeed, float vSpeed, float zDepth);

	//void addLayer(string name, float hSpeed, float vSpeed, float zDepth, int id);

	void deleteLayer(string name);

	SpriteBatch* getCurrentSpriteBatch();

	void updateSpritePositions(float step);

	void updateLayerPosition(Orientation orient, float step);

	void loadGame(string fileName);

	void saveGame();

	vector<Level*>* getLevels();

	vector<Sprite*> getColSpriteList();

	void deleteColSprite(Sprite *colSpr);

	void getNextLayer();

	void getPreviousLayer();

	void setLayerCameraPos(point cPos);

	void GameProvider::addLevel(string levelName);
};

#endif