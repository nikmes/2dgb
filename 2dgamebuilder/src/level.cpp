#include "servicelocator.h"
#include "level.h"
#include "layer.h"

Level::Level()
{
	m_name = "";
	m_currentLayer = 0;
	m_backgroundVisible = true; // background png visible or not
	m_levelIsLoaded = false; // indicates whether the level is loaded or not
	m_currentSpriteBatch = 0;
}

Level::Level(string name)
{
	m_name = name;
	m_currentLayer = 0;
	m_backgroundVisible = true; // background png visible or not
	m_levelIsLoaded = false; // indicates whether the level is loaded or not
	m_currentSpriteBatch = 0;

	TRACE(T_INF, "Level Created:");
	TRACE(T_INF, "Name:	                [%s] ", m_name.c_str());
	TRACE(T_INF, "Filename:             [%s] ", m_fileName.c_str());

	if (m_backgroundVisible)
	{
		TRACE(T_INF, "Static Background:	[%s][Visible] ", m_backgroundFileName.c_str());
	}
	else
	{
		TRACE(T_INF, "Static Background:	[%s][Not Visible] ", m_backgroundFileName.c_str());
	}

	TRACE(T_INF, "Total Layers:         [%d] ", this->getTotalLayers());
	TRACE(T_INF, "Total Sprites:        [%d] ", this->getTotalSprites());
	TRACE(T_INF, "Level XML File:       [%s] ", this->m_fileName.c_str());
}

Level::~Level()
{
	layers.clear();

	TRACE(T_DEB, "Clear box2d World");

	delete World;

	TRACE(T_DEB, "Clear box2d Gravity");

	delete Gravity;
}

void Level::traceInfo()
{
	TRACE(T_INF, "Level Created:");
	TRACE(T_INF, "Name:					[%s] ", m_name.c_str());
	if (m_backgroundVisible)
	{
		TRACE(T_INF, "Static Background:	[%s][Visible] ", m_backgroundFileName.c_str());
	}
	else
	{
		TRACE(T_INF, "Static Background:	[%s][Not Visible] ", m_backgroundFileName.c_str());
	}
	TRACE(T_INF, "Total Layers:         [%d] ", this->getTotalLayers());
	TRACE(T_INF, "Total Sprites:        [%d] ", this->getTotalSprites());
}

SpriteBatch *Level::getCurrentSpriteBatch()
{
	/*
	* Get the current spritebatch of this level
	*/

	int i = 0;

	vector<SpriteBatch*>::iterator it;

	for (it = sprBatches.begin(); it != sprBatches.end(); it++)
	{
		if (i == m_currentSpriteBatch)
		{
			return (*it);
		}

		i++;
	}

	return NULL;
}

int Level::getTotalSpriteBatches()
{
	return this->sprBatches.size();
}

string Level::getActiveLayer()
{
	/*
	* Returns the name of the active layer
	*/

	vector<Layer*>::iterator it;

	for (it = layers.begin(); it != layers.end(); it++)
	{
		if ((*it)->isActive())
		{
			return ((*it)->getName());
		}
	}
}

void Level::addLayer(string name, float hSpeed, float vSpeed, float zDepth)
{
	TRACE(T_DEB, "Layer [%s] will be added to all SpriteBatches of current Level [%s]", name.c_str(), this->m_name.c_str());

	vector<SpriteBatch*> *sprBatches;

	sprBatches = this->getSpriteBatches();

	vector<SpriteBatch*>::iterator it;

	it = sprBatches->begin();
	Layer *newLayer = (*it)->addLayer(name, hSpeed, vSpeed, zDepth);

	for (it = sprBatches->begin() + 1; it != sprBatches->end(); it++)
	{
		TRACE(T_INF, "Adding Layer [%s] to spritebatch [%s][%s]", name.c_str(), (*it)->m_name.c_str(), (*it)->m_xmlFileName.c_str());
		(*it)->addLayer(name, hSpeed, vSpeed, zDepth, newLayer->id);
	}
}

void Level::addLayer(string name, float hSpeed, float vSpeed, float zDepth, int id)
{
	// 1. retrieve the current Level
	// 2. get all layers and check if we try to add a layer with id that already exists
	// 3. if it already exists dont do anything just return
	// 2. retrieve list of sprite batches current level has
	// 3. to each of the sprite batches add the new layer with the given properties

	vector<Layer *> *allLayers = this->getLayers();

	vector<Layer*>::iterator itL;
 
	for (itL = allLayers->begin(); itL != allLayers->end(); itL++)
	{
		TRACE(T_INF, "Checking Layer [%s] with id[%d] ", (*itL)->name.c_str(), (*itL)->id);

		if (id == (*itL)->id)
		{
			return;
		}
	}

	TRACE(T_DEB, "Layer [%s] will be added to all SpriteBatches of current Level [%s]", name.c_str(), this->m_name.c_str());

	vector<SpriteBatch*> *sprBatches;

	sprBatches = this->getSpriteBatches();

	vector<SpriteBatch*>::iterator it;

	for (it = sprBatches->begin(); it != sprBatches->end(); it++)
	{
		TRACE(T_INF, "Adding Layer [%s] to spritebatch [%s][%s]", name.c_str(), (*it)->m_name.c_str(), (*it)->m_xmlFileName.c_str());
		(*it)->addLayer(name, hSpeed, vSpeed, zDepth);
	}
}


void Level::removeLayer(string name)
{
	// FIX: removes a layer from the map

	TRACE(T_DEB, "Layer [%s] will be deleted from the level", name.c_str());

	//	layers.erase(name);
}

void Level::draw()
{
	// draws the map on screen by drawindowg all its layers

	totalRenderedSprites = 0;

	vector<Layer*>::iterator it;
	
	vector<SpriteBatch*>::iterator sbIt;

	ServiceLocator::getCameraService()->decceleration();


	for (sbIt = sprBatches.begin(); sbIt != sprBatches.end(); sbIt++)
	{
		for (it = (*sbIt)->m_layers.begin(); it != (*sbIt)->m_layers.end(); it++)
		{
			g2dResetGlobalScale(); // very important to keep layers aligned

			(*it)->eraseMarkedForDeletion(); // delete what ever sprites of the layer where marked for deletion

			(*it)->draw((*sbIt));
		}
	}
}

Layer* Level::getLayer(string name)
{
	// returns the layer that has name equals to incomming name
	/// !! NEED FIXING !!

	TRACE(T_DEB, "Get Layer [%s]", name.c_str());

	return NULL;
	//	return &this->layers.at(name);
}

int Level::getTotalLayers()
{
	// returns the number of layers in the map

	return this->layers.size();
}

void Level::createPhysicalWorld()
{
	Gravity = new b2Vec2(0.f, 0.f);

	World = new b2World(*Gravity);

	World->SetAllowSleeping(false);

	World->SetDebugDraw(&fooDrawInstance);

	fooDrawInstance.SetFlags(b2Draw::e_shapeBit);

	CreateLine(-50000, 500, 50000, 500);

	CreatePlayer(0, 0);
}

int Level::CreateLine(float x1, float y1, float x2, float y2)
{
	static int lineCount = 0;

	b2BodyDef lineBodyDef;

	lineBodyDef.type = b2_staticBody;

	b2Body* lineBody = World->CreateBody(&lineBodyDef);

	b2EdgeShape lineShape;

	lineShape.Set(b2Vec2(x1 / BSCALE, y1 / BSCALE), b2Vec2(x2 / BSCALE, y2 / BSCALE));

	b2FixtureDef lineFixtureDef;

	lineFixtureDef.shape = &lineShape;

	lineBody->CreateFixture(&lineFixtureDef);

	return lineCount++;
}

void Level::CreateBox(int MouseX, int MouseY)
{
	b2BodyDef BodyDef;

	BodyDef.position = b2Vec2(MouseX / BSCALE, MouseY / BSCALE);

	BodyDef.type = b2_dynamicBody;

	b2Body* Body = World->CreateBody(&BodyDef);

	Body->SetUserData((char*)"Box");

	b2PolygonShape Shape;

	Shape.SetAsBox((66.f / 2) / BSCALE, (66.f / 2) / BSCALE);

	b2FixtureDef FixtureDef;

	FixtureDef.density = 1.f;

	FixtureDef.friction = 0.7f;

	FixtureDef.shape = &Shape;

	Body->CreateFixture(&FixtureDef);
}

void Level::CreatePlayer(float x, float y)
{
	b2BodyDef BodyDef;

	BodyDef.position = b2Vec2(y / BSCALE, x / BSCALE);

	BodyDef.type = b2_dynamicBody;

	playerBody = World->CreateBody(&BodyDef);

	playerBody->SetUserData((char*)"Player");

	b2CircleShape Shape;

	b2FixtureDef FixtureDef;

	Shape.m_radius = 8.f / BSCALE;

	FixtureDef.density = 10.f;

	FixtureDef.friction = 0.8f;

	FixtureDef.restitution = 0.0;

	FixtureDef.shape = &Shape;

	FixtureDef.density = 10;

	playerBody->CreateFixture(&FixtureDef);

	playerBody->SetGravityScale(0);

	playerBody->SetFixedRotation(true);
}

Layer* Level::getNextLayer()
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

	for (it = layers.begin(); it != layers.end(); it++)
	{
		if (i == m_currentLayer)
		{
			//TRACE(T_DEB, "Next layer [%d][%s]", m_currentLayer, (*it)->getName().c_str());
			return (*it);
		}
		i++;
	}

	return NULL;
}

Layer* Level::getCurrentLayer()
{
	int i = 0;

	vector<Layer*>::iterator it;

	for (it = layers.begin(); it != layers.end(); it++)
	{
		if (i == m_currentLayer)
		{
			//TRACE(T_DEB, "Current layer [%d][%s]", m_currentLayer, (*it)->getName().c_str());
			return (*it);
		}

		i++;
	}

	return NULL;
}

Layer* Level::getPreviousLayer()
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

	//TRACE(T_DEB, "Current Layer %d", m_currentLayer);

	for (it = layers.begin(); it != layers.end(); it++)
	{
		if (i == m_currentLayer)
		{
			//TRACE(T_DEB, "Previous layer [%d][%s]", m_currentLayer, (*it)->getName().c_str());
			return (*it);
		}
		i++;
	}

	return NULL;
}

void Level::save()
{
	TRACE(T_DEB, "Save level [%s]", m_name.c_str());

	char fileName[128];

	TiXmlDocument *doc = new TiXmlDocument();
	TiXmlElement *msg;
	TiXmlComment *comment;
	string		  s;
	Layer		 *myLayer;
	char color[32];

	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc->LinkEndChild(decl);

	sprintf(fileName, "%s%s.xml", BASE_PATH, this->m_name.c_str());
	TRACE(T_DEB, "Save level [%s]", fileName);

	TiXmlElement * root = new TiXmlElement("level");
	root->SetAttribute("name", this->m_name.c_str());
	doc->LinkEndChild(root);

	comment = new TiXmlComment("Interpretation");
	comment->SetValue("n  = layer name");
	root->LinkEndChild(comment);

	comment = new TiXmlComment("Interpretation");
	comment->SetValue("sh = layer horizontal scrolling speed");
	root->LinkEndChild(comment);

	comment = new TiXmlComment("Interpretation");
	comment->SetValue("sv = layer vertical scrolling speed");
	root->LinkEndChild(comment);

	comment = new TiXmlComment("Interpretation");
	comment->SetValue("ts = total sprites in layer");
	root->LinkEndChild(comment);

	comment = new TiXmlComment("Interpretation");
	comment->SetValue("a  = global layer transparency");
	root->LinkEndChild(comment);

	comment = new TiXmlComment("Interpretation");
	comment->SetValue("sb = filename of spritebatch used by layer");
	root->LinkEndChild(comment);

	comment = new TiXmlComment("Interpretation");
	comment->SetValue("n  = sprite name");
	root->LinkEndChild(comment);

	comment = new TiXmlComment("Interpretation");
	comment->SetValue("c  = sprite color");
	root->LinkEndChild(comment);

	comment = new TiXmlComment("Interpretation");
	comment->SetValue("sw  = sprite scaled width");
	root->LinkEndChild(comment);

	comment = new TiXmlComment("Interpretation");
	comment->SetValue("sh  = sprite scaled height");
	root->LinkEndChild(comment);

	TiXmlElement * camera = new TiXmlElement("camera");
	camera->SetDoubleAttribute("x", camX);
	camera->SetDoubleAttribute("y", camY);
	camera->SetDoubleAttribute("zoom", ServiceLocator::getCameraService()->zoom_target);
	root->LinkEndChild(camera);

	TiXmlElement *background = new TiXmlElement("background");
	background->SetAttribute("filename", ServiceLocator::getGameService()->m_levelStaticBackground);
	root->LinkEndChild(background);

	vector<Sprite>::iterator it;

	vector<Layer*>::iterator lIt;

	for (lIt = layers.begin(); lIt != layers.end(); lIt++)
	{
		myLayer = (*lIt);

		TiXmlElement * layer = new TiXmlElement("layer");
		layer->SetAttribute("n", myLayer->getName().c_str());
		layer->SetDoubleAttribute("sh", myLayer->horizSpeed);
		layer->SetDoubleAttribute("sv", myLayer->vertSpeed);
		layer->SetDoubleAttribute("z", myLayer->zDepth);
		layer->SetAttribute("a", myLayer->getTotalSprites());
		layer->SetAttribute("sb", myLayer->spriteBatch->getFileName().c_str());
		root->LinkEndChild(layer);

		TiXmlElement * sprites = new TiXmlElement("sprites");
		layer->LinkEndChild(sprites);

		for (it = myLayer->spriteList.begin(); it != myLayer->spriteList.end(); it++)
		{
			layer = new TiXmlElement("sprite");

			layer->SetAttribute("n", it->name.c_str());

			sprintf(color, "%X", it->color);
			layer->SetAttribute("c", color);

			layer->SetAttribute("a", it->alpha);

			layer->SetDoubleAttribute("x", it->x);

			layer->SetDoubleAttribute("y", it->y);

			layer->SetDoubleAttribute("sw", it->sW);

			layer->SetDoubleAttribute("sh", it->sH);

			layer->SetDoubleAttribute("s", it->scale);

			layer->SetDoubleAttribute("r", it->rotation);

			if (it->flipState == FLIP_H)
				layer->SetAttribute("f", "H");
			else if (it->flipState == FLIP_V)
				layer->SetAttribute("f", "V");
			else if (it->flipState == FLIP_HV)
				layer->SetAttribute("f", "HV");
			else
				layer->SetAttribute("f", "N");

			sprites->LinkEndChild(layer);
		}
	}
	doc->SaveFile(fileName);
	doc->Clear();
	delete doc;
}

void Level::load(string fileName)
{
	// Before you attempt to open filename append the BASE_PATH

	TRACE(T_DEB, "Open Level -> %s", fileName.c_str());

	SpriteBatch *currSpriteBatch;

	Layer *currLayer;

	TiXmlDocument *doc = new TiXmlDocument(fileName.c_str());

	
	bool loaded = doc->LoadFile();
	if (!loaded)
	{
		TRACE(T_DEB, "Failed to load file: No root element");
		doc->Clear();
		return;
	}

	TiXmlElement* root = doc->FirstChildElement();

	if (root == NULL)
	{
		TRACE(T_DEB, "Failed to load file: No root element");
		doc->Clear();
		return;
	}

	m_levelIsLoaded = true;

	string elemName = root->Value();

	if (elemName == "level")
	{
		this->m_name = root->Attribute("name");
		TRACE(T_DEB, "Level -> name %s", this->m_name.c_str());
	}

	for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		string elemName = elem->Value();

		if (elemName == "camera")
		{
			CameraProvider *cam = ServiceLocator::getCameraService();

			elem->QueryFloatAttribute("x", &camX);
			elem->QueryFloatAttribute("y", &camY);
			elem->QueryFloatAttribute("zoom", &cam->zoom_target);
			elem->QueryFloatAttribute("cps", &cam->CAM_POS_SPEED);
			elem->QueryFloatAttribute("czs", &cam->CAM_ZOOM_SPEED);
			elem->QueryFloatAttribute("crs", &cam->CAM_ROT_SPEED);
		}

		if (elemName == "background")
		{
			strcpy(ServiceLocator::getGameService()->m_levelStaticBackground, elem->Attribute("filename"));
			ServiceLocator::getTextureService()->GetTexture(ServiceLocator::getGameService()->m_levelStaticBackground);
			TRACE(T_DEB, "Background filename [%s]", ServiceLocator::getGameService()->m_levelStaticBackground);
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
					TRACE(T_DEB, "    Load all texturemaps and their layers");

					for (TiXmlElement* elem3 = elem2->FirstChildElement(); elem3 != NULL; elem3 = elem3->NextSiblingElement())
					{
						string elem3Name = elem3->Value();

						TRACE(T_DEB, "      Node: [%s] with attributes name[%s] filename[%s]", elem3Name.c_str(), elem3->Attribute("name"), elem3->Attribute("filename"));

						currSpriteBatch = this->addSpriteBatch(elem3->Attribute("filename"));

						// load any layers linked to texturepack

						for (TiXmlElement* elem4 = elem3->FirstChildElement()->FirstChildElement(); elem4 != NULL; elem4 = elem4->NextSiblingElement())
						{
							string elem4Name = elem4->Value();

							TRACE(T_DEB, "        Node: [%s] with attributes name[%s] hs[%s] vs[%s] z[%s] id[%s]", elem4Name.c_str(), elem4->Attribute("name"), elem4->Attribute("hs"), elem4->Attribute("vs"), elem4->Attribute("z"), elem4->Attribute("id"));

							float hs;
							elem4->QueryFloatAttribute("hs", &hs);

							float vs;
							elem4->QueryFloatAttribute("vs", &vs);

							float z;
							elem4->QueryFloatAttribute("z", &z);

							int id;
							elem4->QueryIntAttribute("id", &id);

							currLayer = currSpriteBatch->addLayer(elem4->Attribute("name"), hs, vs, z, id);

							// load any sprites linked to the layer

							for (TiXmlElement* elem5 = elem4->FirstChildElement()->FirstChildElement(); elem5 != NULL; elem5 = elem5->NextSiblingElement())
							{
								string elem5Name = elem5->Value();

								TRACE(T_DEB, "          Node: [%s] with attributes n[%s] c[%s] a[%s] x[%s] y[%s] sw[%s] sh[%s] s[%s] r[%s] rz[%s] f[%s]", elem5Name.c_str(), elem5->Attribute("n"), elem5->Attribute("c"), elem5->Attribute("a"), elem5->Attribute("x"), elem5->Attribute("y"), elem5->Attribute("sw"), elem5->Attribute("sh"), elem5->Attribute("s"), elem5->Attribute("r"), elem5->Attribute("rz"), elem5->Attribute("f"));

								// set sprite name and based on name get croping properties from spritebatch
								Sprite spr;
								spr.name = elem5->Attribute("n");
								Sprite sprD = currSpriteBatch->getSprite(spr.name);
								spr.sprBatch = currSpriteBatch;
								spr.cX = sprD.cX;
								spr.cY = sprD.cY;
								spr.cW = sprD.cW;
								spr.cH = sprD.cH;

								// set sprite color
								g2dColor sprColor;
								string color;
								color = elem5->Attribute("c");
								sscanf(color.c_str(), "%x", &sprColor);
								spr.setColor(sprColor);

								// set sprite alpha value
								elem5->QueryIntAttribute("a", &spr.alpha);

								// set sprite x value
								elem5->QueryFloatAttribute("x", &spr.x);

								// set sprite y value
								elem5->QueryFloatAttribute("y", &spr.y);

								// set sprite scaled width value
								elem5->QueryFloatAttribute("sw", &spr.sW);

								// set sprite scaled height value
								elem5->QueryFloatAttribute("sh", &spr.sH);

								// set sprite scale factor
								elem5->QueryFloatAttribute("s", &spr.scale);

								// set sprite roation value
								elem5->QueryFloatAttribute("r", &spr.rotation);

								// set sprite z value
								elem5->QueryFloatAttribute("rz", &spr.m_z);

								// set flip state
								string fstate;
								fstate = elem5->Attribute("f");

								if (fstate == "H")
								{
									spr.flipState = FLIP_H;
								}
								else if (fstate == "V")
								{
									spr.flipState = FLIP_V;
								}
								else if (fstate == "HV")
								{
									spr.flipState = FLIP_HV;
								}
								else
								{
									spr.flipState = FLIP_NONE;
								}

								spr.id = ServiceLocator::getHelpService()->generateId();

								currLayer->spriteList.push_back(spr);
								currLayer->totalSprites++;
							}
						}
					}
				}

				// load any music that they will be used during intro state or menu state

				if (elem2Name == "music")
				{
					TRACE(T_DEB, "    Load all music files");
				}

				// load any sounds that they will be used during intro state or menu state

				if (elem2Name == "sounds")
				{
					TRACE(T_DEB, "    Load all sound files");
				}

				// load any images that they will be used during intro state or menu state

				if (elem2Name == "images")
				{
					TRACE(T_DEB, "    Load all image files");
				}

				// load any fonts that they will be used during intro state or menu state

				if (elem2Name == "fonts")
				{
					TRACE(T_DEB, "    Load all font files");
				}
			}
		}
	}
	root->Clear();
	doc->Clear();
	delete doc;
}

string Level::getName()
{
	// returns the level name

	return (this->m_name);
}

void Level::setName(string name)
{
	// sets the level name

	this->m_name = name;
}

vector<Layer*> *Level::getLayers()
{
	// returns all layers of the first spritebatch.
	vector<SpriteBatch*>::iterator it;
	if (sprBatches.empty())
	{
		return NULL;
	}
	else
	{
		it = sprBatches.begin();
		return (*it)->getLayers();
	}
}

vector<SpriteBatch*> *Level::getSpriteBatches()
{
	return &sprBatches;
}

void Level::saveAs(string fileName)
{
	string levelFullPath = ServiceLocator::getGameService()->m_directory;

	// make sure we update filename to be .xml of level name

	this->m_fileName = this->m_name;
	this->m_fileName.append(".xml");

	// create document and add xml version info

	TiXmlDocument *levelDoc = new TiXmlDocument(); // to save level.xml
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	levelDoc->LinkEndChild(decl);

	// add the level element with its name and filename

	TiXmlElement *level = new TiXmlElement("level");
	level->SetAttribute("name", this->m_name.c_str());
	level->SetAttribute("filename", this->m_fileName.c_str());
	levelDoc->LinkEndChild(level);

	// add camera position when level is starting

	TiXmlElement * camera = new TiXmlElement("camera");

	CameraProvider *cam = ServiceLocator::getCameraService();

	camera->SetDoubleAttribute("x", camX);
	camera->SetDoubleAttribute("y", camY);
	camera->SetDoubleAttribute("cps", cam->CAM_POS_SPEED);
	camera->SetDoubleAttribute("czs", cam->CAM_ZOOM_SPEED);
	camera->SetDoubleAttribute("crs", cam->CAM_ROT_SPEED);
	camera->SetDoubleAttribute("zoom", cam->zoom_target);
	level->LinkEndChild(camera);

	// create assets node

	TiXmlElement *assetsNode = new TiXmlElement("assets");
	level->LinkEndChild(assetsNode);

	// create spritebatches nodes

	TiXmlElement *texturemapsNode = new TiXmlElement("texturemaps");
	this->saveSpriteBatches(texturemapsNode);
	assetsNode->LinkEndChild(texturemapsNode);

	TiXmlElement *musicNode = new TiXmlElement("music");
	assetsNode->LinkEndChild(musicNode);

	TiXmlElement *soundsNode = new TiXmlElement("sounds");
	assetsNode->LinkEndChild(soundsNode);

	TiXmlElement *imagesNode = new TiXmlElement("images");

	assetsNode->LinkEndChild(imagesNode);

	TiXmlElement *fontsNode = new TiXmlElement("fonts");
	assetsNode->LinkEndChild(fontsNode);

	TRACE(T_INF, "Saving level as [%s] - Started", m_fileName.c_str())

		// save the level xml defintion file
		levelFullPath.append("levels/");

	levelFullPath.append(this->m_fileName);

	bool save = levelDoc->SaveFile(levelFullPath.c_str());

	if (save)
	{
		TRACE(T_INF, "File [%s] saved OK", levelFullPath.c_str());
	}
	else
	{
		TRACE(T_INF, "File [%s] NOT saved OK", levelFullPath.c_str());
	}

	// clear the level xml to be used with next level
	levelDoc->Clear();

	TRACE(T_INF, "Saving level [%s] - Completed", m_fileName.c_str())
}

void Level::saveSpriteBatches(TiXmlElement *texturemapsNode)
{
	vector<SpriteBatch*>::iterator it;

	for (it = sprBatches.begin(); it != sprBatches.end(); it++)
	{
		TiXmlElement *texturemapNode = new TiXmlElement("texturemap");

		texturemapNode->SetAttribute("name", (*it)->m_name.c_str());
		texturemapNode->SetAttribute("filename", (*it)->m_xmlFileName.c_str());
		texturemapsNode->LinkEndChild(texturemapNode);

		TiXmlElement *layersNode = new TiXmlElement("layers");
		texturemapNode->LinkEndChild(layersNode);

		// create layers nodes
		this->saveLayers(layersNode, (*it));

		TRACE(T_INF, "Saving sprite batch [%s][%s]", (*it)->m_name.c_str(), (*it)->m_xmlFileName.c_str());
	}
}

void Level::saveLayers(TiXmlElement *layersNode, SpriteBatch* currSpriteBatch)
{
	vector<Layer*>::iterator lIt;

	for (lIt = currSpriteBatch->m_layers.begin(); lIt != currSpriteBatch->m_layers.end(); lIt++)
	{
		TiXmlElement *layerNode = new TiXmlElement("layer");

		layerNode->SetAttribute("name", (*lIt)->name.c_str());
		layerNode->SetDoubleAttribute("hs", (*lIt)->horizSpeed);
		layerNode->SetDoubleAttribute("vs", (*lIt)->vertSpeed);
		layerNode->SetDoubleAttribute("z", (*lIt)->zDepth);
		layerNode->SetAttribute("id", (*lIt)->id);

		TRACE(T_INF, "Saving Layer [%s][%d]", (*lIt)->name.c_str(), (*lIt)->id);

		TiXmlElement *spritesNode = new TiXmlElement("sprites");
		layerNode->LinkEndChild(spritesNode);

		// save all sprites of spritebatch current layer

		this->saveSprites(spritesNode, (*lIt));

		layersNode->LinkEndChild(layerNode);
	}
}

void Level::saveSprites(TiXmlElement *spritesNode, Layer* currLayer)
{
	char color[32];

	vector<Sprite>::iterator it;

	for (it = currLayer->spriteList.begin(); it != currLayer->spriteList.end(); it++)
	{
		TiXmlElement *spriteNode = new TiXmlElement("sprite");

		spriteNode->SetAttribute("n", it->name.c_str());

		sprintf(color, "%X", it->color);
		spriteNode->SetAttribute("c", color);
		spriteNode->SetAttribute("a", it->alpha);
		spriteNode->SetDoubleAttribute("x", it->x);
		spriteNode->SetDoubleAttribute("y", it->y);
		spriteNode->SetDoubleAttribute("sw", it->sW);
		spriteNode->SetDoubleAttribute("sh", it->sH);
		spriteNode->SetDoubleAttribute("s", it->scale);
		spriteNode->SetDoubleAttribute("r", it->rotation);
		spriteNode->SetDoubleAttribute("rz", it->m_z);

		if (it->flipState == FLIP_H)
		{
			spriteNode->SetAttribute("f", "H");
		}
		else if (it->flipState == FLIP_V)
		{
			spriteNode->SetAttribute("f", "V");
		}
		else if (it->flipState == FLIP_HV)
		{
			spriteNode->SetAttribute("f", "HV");
		}
		else
		{
			spriteNode->SetAttribute("f", "N");
		}

		spritesNode->LinkEndChild(spriteNode);
	}
}

SpriteBatch* Level::addSpriteBatch(string textureAtlasFileName)
{
	// adding spritebatch because of game loading from file
	// 1. retrieve the current level (the one we editing)
	// 2. add new sprite batch to the level
	// 3. retrieve the list of layers that this level (one of existing sprite batches) has
	// 4. add all layers (same one with same properties) to the newly added sprite batch

	TRACE(T_INF, "Adding [%s] to level [%s] sprite batches", textureAtlasFileName.c_str(), this->m_name.c_str());

	SpriteBatch *sprBatch = new SpriteBatch(textureAtlasFileName);

	/*
	TRACE(T_INF, "Adding all existing layers to the new spritebatch");

	vector<SpriteBatch*>::iterator it = this->getSpriteBatches()->begin();

	if (it != this->getSpriteBatches()->end())
	{
		vector<Layer*> *layers = (*it)->getLayers();

		vector<Layer*>::iterator itL;

		for (itL = layers->begin(); itL != layers->end(); itL++)
		{
			TRACE(T_INF, "Adding Layer [%s] to spritebatch [%s][%s]", (*itL)->name.c_str(), sprBatch->m_name.c_str(), sprBatch->m_xmlFileName.c_str());
			sprBatch->addLayer((*itL)->name, (*itL)->horizSpeed, (*itL)->vertSpeed, (*itL)->zDepth, (*itL)->id);
		}
	}
	*/
	sprBatches.push_back(sprBatch);

	return sprBatch;
}

SpriteBatch* Level::addNewSpriteBatch(string textureAtlasFileName)
{
	// 1. retrieve the current level (the one we editing)
	// 2. add new sprite batch to the level
	// 3. retrieve the list of layers that this level (one of existing sprite batches) has
	// 4. add all layers (same one with same properties) to the newly added sprite batch

	TRACE(T_INF, "Adding [%s] to level [%s] sprite batches", textureAtlasFileName.c_str(),m_name.c_str());

	SpriteBatch *sprBatch = new SpriteBatch(textureAtlasFileName);

	TRACE(T_INF, "Adding all existing layers to the new spritebatch");

	vector<SpriteBatch*>::iterator it = this->getSpriteBatches()->begin();

	if (it != this->getSpriteBatches()->end())
	{
		vector<Layer*> *layers = (*it)->getLayers();

		vector<Layer*>::iterator itL;

		for (itL = layers->begin(); itL != layers->end(); itL++)
		{
			TRACE(T_INF, "Adding Layer [%s] to spritebatch [%s][%s]", (*itL)->name.c_str(), sprBatch->m_name.c_str(), sprBatch->m_xmlFileName.c_str());
			sprBatch->addLayer((*itL)->name, (*itL)->horizSpeed, (*itL)->vertSpeed, (*itL)->zDepth, (*itL)->id);
		}
	}

	sprBatches.push_back(sprBatch);

	return sprBatch;
}

int Level::getTotalSprites()
{
	int totalSprites = 0;

	vector<SpriteBatch*>::iterator it;
	vector<Layer*>::iterator layerIt;

	if (ServiceLocator::getGameService()->gameIsCreated())
	{
		for (it = sprBatches.begin(); it != sprBatches.end(); it++)
		{
			for (layerIt = (*it)->m_layers.begin(); layerIt != (*it)->m_layers.end(); layerIt++)
			{
				totalSprites += (*layerIt)->spriteList.size();
			}
		}
	}

	return totalSprites;
}