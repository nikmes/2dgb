#include "gamebar.h"
#include <imgui/imgui.h>
#include "servicelocator.h"

GameBar::GameBar(TextureAtlas *textureAtlas, string name) :ImGUIComponent(textureAtlas, name)
{
	this->addIcons();
}

GameBar::~GameBar()
{
}

void GameBar::addIcons()
{
	// populate the toolbar with icons in the same order you want them to appear
	textureAtlasItem taItem;

	// drawing modes

	taItem = m_textureAtlas->getItem("mario");
	taItem.hint = "";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("newmap");
	taItem.hint = "Create a new game wizard";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("openmap");
	taItem.hint = "Load an existing game from file system";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("savemap");
	taItem.hint = "Save current game to file system";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("saveas");
	taItem.hint = "Save current game under different name";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("closemap");
	taItem.hint = "Close current game";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("gamestructure");
	taItem.hint = "Navigate game structure";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("joystick");
	taItem.hint = "Test drive your game";
	icons.push_back(taItem);


}

void GameBar::run(bool *opened)
{
	string xmlGameFileName;
	static bool showGameDialogue = false;
	static bool showGameProperties = false;
	static bool showNewGameDialog = false;
	static bool showGameLoadDialog = false;
	ImVec2 uv0, uv1;

	vector<textureAtlasItem>::iterator it;

	ImGui::Begin(m_name.c_str(), opened, ImVec2((45 * icons.size()), 46), 0.70f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
	{
		it = icons.begin();

		textureAtlasItem currItem = *it;

		uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
		uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);

		ImGui::Image(texId, ImVec2(20, 20), uv0, uv1, ImColor(255, 255, 255, 255));

		for (it = icons.begin()+1; it != icons.end(); it++)
		{
			textureAtlasItem currItem = *it;
			uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
			uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);
			ImGui::PushID(currItem.id);

			ImGui::SameLine();

			if (ImGui::ImageButton(texId, ImVec2(40, 40), uv0, uv1, 0, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255)))
			{
				if (currItem.name == "openmap")
				{
					TRACE(T_DEB, "Bring up user dialogue to load a new game");
					showGameLoadDialog = true;
				}

				if (currItem.name == "newmap")
				{
					TRACE(T_DEB, "Bring up user dialogue to create a new game");
					showGameDialogue = true;
				}

				if (currItem.name == "savemap")
				{
					TRACE(T_DEB, "Saving current game");
					ServiceLocator::getGameService()->saveGame();
				}
				
				if (currItem.name == "savemap")
				{
					ServiceLocator::getGameService()->saveGame();
				}

				if (currItem.name == "gamestructure")
				{
					showGameProperties = true;
				}
			}


			// for every icon display hint if hint is set and hints are enabled

			if (ServiceLocator::getConfigurationService()->pEditor_hints == true)
			{
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip(currItem.hint.c_str());
				}
			}

			ImGui::PopID();
		}


	}

	if (showGameDialogue)
	{
		showGameDialogue = newGameDialogue();
	}

	if (showGameLoadDialog)
	{
		xmlGameFileName = ServiceLocator::getImguiService()->fileDialogWindow->runSelectXmlFileName("Load Game");

		if (!xmlGameFileName.empty())
		{
			showGameLoadDialog = false;

			if (xmlGameFileName == "Cancel")
			{
				xmlGameFileName = "";
			}
			else
			{
				ServiceLocator::getGameService()->loadGame(xmlGameFileName);
			}
		}
	}

	ImGui::End();

	if (showGameProperties)
	{
		this->gameProperties(&showGameProperties);
	}

}

bool GameBar::newGameDialogue()
{
	static bool selectPNG = false;
	static bool selectTATLAS = false;
	static bool selectDIR = false;

	static string gameDirToSave;

	static string pngFileToLoad;
	static string tatlasFileToLoad;

	ImVec2 uv0, uv1;
	uv0 = ImVec2(0, 0);
	uv1 = ImVec2(1, 1);

	bool close = true;
	float prevAlpha = 0.0f;

	ImGui::OpenPopup("Create New Game");
	
	//prevAlpha = ImGui::GetStyle().Alpha;
	//ImGui::GetStyle().Alpha = 50.0;

	ImGui::SetNextWindowSize(ImVec2(384, 512), ImGuiSetCond_FirstUseEver);

	if (ImGui::BeginPopupModal("Create New Game", NULL, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::PushItemWidth(150);

		if (ImGui::Button("Game Directory", ImVec2(150, 0)))
		{
			TRACE(T_INF, "Show file dialogue for user to choose image to be used as static background");
			selectDIR = true;
		}

		ImGui::TextWrapped("%s", ServiceLocator::getGameService()->m_gamePath);

		ImGui::InputText("Game Name", ServiceLocator::getGameService()->m_gameName, 128);

		ImGui::InputText("Level Name", ServiceLocator::getGameService()->m_levelName, 128);

		if (ImGui::Button("Load Static Background", ImVec2(150, 0)))
		{
			TRACE(T_INF, "Show file dialogue for user to choose image to be used as static background");
			selectPNG = true;
		}

		ImGui::Text("Static background Preview");

		if (pngFileToLoad.empty())
		{
			ImGui::Image(NULL, ImVec2(192, 192), uv0, uv1, ImColor(255, 255, 255, 55), ImColor(100, 100, 100, 100));
			ImGui::Text("File: []");
		}
		else
		{
			g2dTexture *t = ServiceLocator::getTextureService()->GetTextureFromFileSystem(pngFileToLoad);
			ImGui::Image((ImTextureID*)t->id, ImVec2(256, 192), uv0, uv1, ImColor(255, 255, 255, 200), ImColor(100, 100, 100, 100));
			ImGui::TextWrapped("File: [%s]",pngFileToLoad.c_str());
		}

		ImGui::InputText("Layer Name", ServiceLocator::getGameService()->m_layerName, 128);

		ImGui::DragFloat("Horizontal Speed", &ServiceLocator::getGameService()->m_layerHorizontalSpeed, 0.01);

		ImGui::DragFloat("Vertical Speed", &ServiceLocator::getGameService()->m_layerVerticalSpeed, 0.01);

		ImGui::DragInt("Layer Depth", &ServiceLocator::getGameService()->m_layerDepth, 1);

		ImGui::Checkbox("Active", &ServiceLocator::getGameService()->m_layerActive);

		ImGui::PopItemWidth();

		if (ImGui::Button("Load Texture Atlas", ImVec2(150, 0)))
		{
			TRACE(T_INF, "Show file dialogue for user to choose texture atlas");
			selectTATLAS = true;
		}
			
		ImGui::Text("Texture Atlas Preview");

		if (tatlasFileToLoad.empty())
		{
			ImGui::Image(NULL, ImVec2(192, 192), uv0, uv1, ImColor(255, 255, 255, 55), ImColor(100, 100, 100, 100));
			ImGui::Text("File: []");
		}
		else
		{
			SpriteBatch *sprBatch = new SpriteBatch(tatlasFileToLoad, "preview");
			string newPath = utils::SplitPath(tatlasFileToLoad);
			string newFilePath = newPath;
			string newFileName = utils::RemoveExtension(utils::SplitFilename(tatlasFileToLoad));
			newFilePath.append("/");
			newFileName.append(".png");
			newFilePath.append(newFileName);
			g2dTexture *t = ServiceLocator::getTextureService()->GetTextureFromFileSystem(newFilePath);
			ImGui::Image((ImTextureID*)t->id, ImVec2(256, 192), uv0, uv1, ImColor(255, 255, 255, 200), ImColor(100, 100, 100, 100));
			ImGui::TextWrapped("File: [%s]", tatlasFileToLoad.c_str());
			delete sprBatch;
		}

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			TRACE(T_INF, "Validate all entered information and if OK create the new game");
			strcpy(ServiceLocator::getGameService()->m_textureAtlasName,tatlasFileToLoad.c_str());
			ServiceLocator::getGameService()->createNewGame();
			ImGui::CloseCurrentPopup();
			close = false;
		}
		
		ImGui::SameLine();

		if (ImGui::Button("CANCEL", ImVec2(120, 0)))
		{
			TRACE(T_INF, "User cancelled. Reset all information already entered");
			ImGui::CloseCurrentPopup();
			close = false;
		}

	}
	// Prompt user to select GAME directory
	if (selectDIR)
	{
		gameDirToSave = ServiceLocator::getImguiService()->fileDialogWindow->runDirSelect();
		
		if (!gameDirToSave.empty())
		{
			selectDIR = false;

			if (gameDirToSave == "Cancel")
			{
				gameDirToSave = "";
			}
			else
			{
				strcpy(ServiceLocator::getGameService()->m_gamePath, gameDirToSave.c_str());
			}
		}

	}

	// Prompt user to choose a PNG
	if (selectPNG)
	{
		pngFileToLoad = ServiceLocator::getImguiService()->fileDialogWindow->runPNGSelect();

		if (!pngFileToLoad.empty())
		{
			selectPNG = false;
			
			strcpy(ServiceLocator::getGameService()->m_levelStaticBackground, pngFileToLoad.c_str());

			if (pngFileToLoad == "Cancel")
			{
				pngFileToLoad = "";
				strcpy(ServiceLocator::getGameService()->m_levelStaticBackground, "\0");
			}
		}
	}

	// Prompt user to choose an XML Texture Atlas
	if (selectTATLAS)
	{
		tatlasFileToLoad = ServiceLocator::getImguiService()->fileDialogWindow->runTAtlasSelect();

		if (!tatlasFileToLoad.empty())
		{
			selectTATLAS = false;

			if (tatlasFileToLoad == "Cancel")
			{
				tatlasFileToLoad = "";
			}
			else
			{
				//!!! check for empty  filanemme
			}
		}
	}

	ImGui::EndPopup();

	//ImGui::GetStyle().Alpha = prevAlpha;

	return close;
}

void GameBar::gameProperties(bool *opened)
{
	static bool	  selectTATLAS = false;
	static string tatlasFileToLoad;

	bool layerNameChanged		= false;
	bool layerHorizSpeedChanged = false;
	bool layerVertSpeedChanged	= false;
	bool layerZDepthChanged		= false;
	bool layerIsActiveChanged	= false;

	float layerHorizSpeedPrevValue  = 0.0f; 
	float layerVertSpeedPrevValue   = 0.0f; 

	static Level *selectedLevel;

	// Game
	//  | Game Name
	//  |   | Texture Atlases
	//  |   ----| Name
	//  |       | Preview
	//  | Level 1
	//  |   | Texture Atlases
	//  |   ----| Name
	//  |       | Preview
	//  |   | Layer A
	//  |   ----| Name
	//  |       | Horizontal Speed
	//  |       | Vertical Speed
	//  |       | Visible
	//  |       | Alpha
	//  |   | Layer B
	//  |   ----| Name
	//  |       | Horizontal Speed
	//  |       | Vertical Speed
	//  |       | Visible
	//  |       | Alpha
	//  |   | Layer C
	//  |   ----| Name
	//  |       | Horizontal Speed
	//  |       | Vertical Speed
	//  |       | Visible
	//  |       | Alpha
	//  | Level 2
	//  |   | Layer A
	//  |   ----| Name
	//  |       | Horizontal Speed
	//  |       | Vertical Speed
	//  |       | Visible
	//  |       | Alpha
	//  |   | Layer B
	//  |   ----| Name
	//  |       | Horizontal Speed
	//  |       | Vertical Speed
	//  |       | Visible
	//  |       | Alpha
	ImVec2 uv0, uv1;

	vector<textureAtlasItem>::iterator it;

	it = icons.begin();

	textureAtlasItem currItem = *it;

	uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
	uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);

	//ImGui::Image(texId, ImVec2(20, 20), uv0, uv1, ImColor(255, 255, 255, 255));

	if (opened)
	{
		char gameName[128]; 
		int i = 0;
		int q = 0; 

		vector<Level*>::iterator it;
		vector<Level*> *levels = ServiceLocator::getGameService()->getLevels();
		vector<Layer*> *layers;
		vector<Layer*>::iterator itL;

		if (!ImGui::Begin("Game Properties", opened, ImVec2(0, 0), 0.90f))
		{
			ImGui::End();
			return;
		}

		ImGui::PushItemWidth(150);	
		
		if (ImGui::TreeNode("Game"))
		{
			strcpy(gameName, ServiceLocator::getGameService()->m_gameName);
			
			if (ImGui::InputText("Game Name", gameName, 128))
			{
				strcpy(ServiceLocator::getGameService()->m_gameName,gameName);
			}
			if (ImGui::Button("Add Level"))
			{
				ServiceLocator::getGameService()->addLevel("level_2");
			}

			if (ImGui::Button("Get Next Level"))
			{
				ServiceLocator::getGameService()->getNextLevel();
			}

			for (it = levels->begin(); it != levels->end(); it++)
			{
				i++;

				char levelName[128];

				strcpy(levelName, (*it)->m_name.c_str());

				if (ImGui::TreeNode((void*)(intptr_t)i, "%s", levelName))
				{
					//if (ImGui::ImageButton(texId, ImVec2(32, 32), uv0, uv1, ImColor(255, 255, 255, 255)))
					//{

					//}

					if (ImGui::Button("Add Layer"))
					{
						(*it)->addLayer("test", 1, 1, 2);
					}

					if (ImGui::Button("Add SpriteBatch"))
					{
						selectTATLAS = true;
						selectedLevel = (*it);

					}

					if (ImGui::Button("Get Next Layer"))
					{
						ServiceLocator::getGameService()->getNextLayer();
						//(*it)->getNextLayer();
					}

					if (ImGui::InputText("Level Name", levelName, 128))
					{
						(*it)->m_name = levelName;
					}
					// add all layers of level
					// !IMPORTANT!: this will add all layers of first sprite batch. we should actually update all layers
					//              of all other sprite batches every time to match the layer properties of the layers of the
					//              first spritebatch

					layers = (*it)->getLayers();
					if (layers)
					{
						for (itL = layers->begin(); itL != layers->end(); itL++)
						{
							q++;

							char layerName[128];

							strcpy(layerName, (*itL)->getName().c_str());

							if (ImGui::TreeNode((void*)(intptr_t)q, "%s", layerName))
							{
								if (ImGui::InputText("Layer Name", layerName, 128))
								{
									(*itL)->setName(layerName);
								}

								// save previous values so we know to shift sprite positions

								layerHorizSpeedPrevValue = (*itL)->horizSpeed;
								layerVertSpeedPrevValue = (*itL)->vertSpeed;

								layerHorizSpeedChanged = ImGui::DragFloat("Horizontal Speed", &(*itL)->horizSpeed, 0.01);
								layerVertSpeedChanged = ImGui::DragFloat("Vertical Speed", &(*itL)->vertSpeed, 0.001);
								layerZDepthChanged = ImGui::DragFloat("Depth", &(*itL)->zDepth, 1.0);
								layerIsActiveChanged = ImGui::Checkbox("Active", &(*itL)->m_active);

								if (layerHorizSpeedChanged || layerVertSpeedChanged || layerZDepthChanged || layerIsActiveChanged || layerNameChanged)
								{
									vector<SpriteBatch*> *sprBatches = (*it)->getSpriteBatches();
									vector<SpriteBatch*>::iterator sprbIt;
									vector<Layer*> *layers;
									vector<Layer*>::iterator lIt;

									//(*itL)->updateLayerPosX(-(layerHorizSpeedPrevValue - (*itL)->horizSpeed));
									//ServiceLocator::getGameService()->updateSpritePositions((*itL)->horizSpeed);
									TRACE(T_INF, "Horizontal Speed is now [%f]", (*itL)->horizSpeed);
									// +1 because first one already changed

									for (sprbIt = sprBatches->begin() + 1; sprbIt != sprBatches->end(); sprbIt++)
									{
										layers = (*sprbIt)->getLayers();

										for (lIt = layers->begin(); lIt != layers->end(); lIt++)
										{
											if ((*lIt)->id == (*itL)->id)
											{
												//(*lIt)->updateLayerPosX(-(layerHorizSpeedPrevValue - (*itL)->horizSpeed));
												(*lIt)->setName((*itL)->name);
												(*lIt)->horizSpeed = (*itL)->horizSpeed;
												(*lIt)->vertSpeed = (*itL)->vertSpeed;
												(*lIt)->zDepth = (*itL)->zDepth;
												(*lIt)->m_active = (*itL)->m_active;
											}
										}
									}
								}

								ImGui::LabelText("Layer ID", "%d", (*itL)->id);

								ImGui::TreePop();
							}

						}
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		ImGui::PopItemWidth();
		
		// Prompt user to choose an XML Texture Atlas
		if (selectTATLAS)
		{
			tatlasFileToLoad = ServiceLocator::getImguiService()->fileDialogWindow->runTAtlasSelect();

			if (!tatlasFileToLoad.empty())
			{
				selectTATLAS = false;

				if (tatlasFileToLoad == "Cancel")
				{
					tatlasFileToLoad = "";
				}
				else
				{
					/*
					* Physicaly copy the .ta and .png files to the game/texturemaps directory
					*/

					// Copy texture atlas xml definition to game_path/texturemaps
					string textureAtlasNewPath = ServiceLocator::getGameService()->m_directory;
					textureAtlasNewPath.append("texturemaps/");
					string textureAtlasFileName = utils::SplitFilename(tatlasFileToLoad);
					textureAtlasNewPath.append(textureAtlasFileName);
					TRACE(T_INF, "Copy from [%s] to [%s]", tatlasFileToLoad.c_str(), textureAtlasNewPath.c_str());
					utils::copy_file(tatlasFileToLoad, textureAtlasNewPath.c_str());

					// Copy texture atlas png file to game_path/texturemaps
					textureAtlasNewPath = utils::RemoveExtension(textureAtlasNewPath);
					textureAtlasNewPath.append(".png");
					string textureAtlasPNGFileName = utils::RemoveExtension(tatlasFileToLoad);
					textureAtlasPNGFileName.append(".png");
					TRACE(T_INF, "Copy from [%s] to [%s]", textureAtlasPNGFileName.c_str(), textureAtlasNewPath.c_str());
					utils::copy_file(textureAtlasPNGFileName.c_str(), textureAtlasNewPath.c_str());

					// Now we copied the files physicaly we can add spriteBatch to the created level

					//ServiceLocator::getGameService()->addSpriteBatch(textureAtlasFileName);
					selectedLevel->addNewSpriteBatch(textureAtlasFileName);
				}
			}
		}
		ImGui::End();
	}
}