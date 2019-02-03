/**
* \date    14/02/2015
* \author  Nicholas Messaritis
*
* \file    glfwvideoprovider.cpp
*
* Implementation of the glfw3 video service provider
*
*/

#include <stdio.h>
#include <imgui/imgui.h>

#include "filedialogwindow.h"
#include "servicelocator.h"

#include "definitions.h"

using namespace utils;

FileDialogWindow::FileDialogWindow(TextureAtlas *textureAtlas, std::string name) :ImGUIComponent(textureAtlas, name)
{
	addIcons();
	strcpy(m_selectedFile, "No File Selected");
	this->getDriveLetters();
}

FileDialogWindow::~FileDialogWindow()
{

}

void FileDialogWindow::setFileType(FileType ft)
{
	fileType = ft;
}

void FileDialogWindow::addIcons()
{
	textureAtlasItem taItem;


	taItem = m_textureAtlas->getItem("refresh");
	taItem.hint = "Rescan drive letters";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("folder-previous");
	taItem.hint = "Previous directory";
	icons.push_back(taItem);
}

void FileDialogWindow::getDriveLetters()
{
#ifndef PLATFORM_LINUX
	int ret = 0, len = 0;
	DWORD dwSize = MAXPATH;
	char szLogicalDrives[MAXPATH] = { 0 };
	memset(driveLetters, '\0', 128);

	DWORD dwResult = GetLogicalDriveStrings(dwSize, szLogicalDrives);

	if (dwResult > 0 && dwResult <= MAX_PATH)
	{
		char* szSingleDrive = szLogicalDrives;

		while (*szSingleDrive)
		{
			szSingleDrive[2] = '\0';
			szSingleDrive[3] = '\0';
			ret = sprintf(driveLetters + len, "%s\0", szSingleDrive);
			len += ret + 1;
			driveList.push_back(szSingleDrive);
			szSingleDrive[2] = '/';

			szSingleDrive += strlen(szSingleDrive) + 1; // get the next drive
		}
	}
#else
	driveList.push_back("/");
	strcpy(driveLetters, "/");
#endif

}

bool FileDialogWindow::showModalWindow(std::string name, std::string caption)
{
	bool shouldClose = false;
	ImVec2 uv0, uv1;
	float prevAlpha = 0.0f;

	ImGui::OpenPopup(name.c_str());
	
	prevAlpha = ImGui::GetStyle().Alpha;
	ImGui::GetStyle().Alpha = 50.0;

	if (ImGui::BeginPopupModal(name.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

		textureAtlasItem currItem = m_textureAtlas->getItem("warning");

		uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
		uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);

		ImGui::Image(texId, ImVec2(46, 46), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(100, 100, 100, 100));
		ImGui::SameLine();
		ImGui::Text(caption.c_str());

		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			shouldClose = false;
		}
		else
		{
			shouldClose = true;
		}

		ImGui::SameLine();

		ImGui::EndPopup();
	}

	ImGui::GetStyle().Alpha = prevAlpha;

	return shouldClose;
}

std::string FileDialogWindow::runTAtlasSelect()
{
	SpriteBatch *sprBatch;
	ImVec2 uv0, uv1;
	vector<textureAtlasItem>::iterator it;
	vector<std::string> dl;
	vector<std::string>::iterator dlIt;

	char dirName[MAXPATH];
	char fileName[MAXPATH];
	std::string fullPath;

	ImVec4 imGreen(0.0f, 1.0f, 0.0f, 1.0f);

	memset(dirName, '\0', sizeof(dirName));
	memset(fileName, '\0', sizeof(fileName));
	
	ImGui::OpenPopup("Texture Atlas Select");

	ImGui::SetNextWindowSize(ImVec2(384, 512), ImGuiSetCond_FirstUseEver);

	static std::string dirPath;

	if (ImGui::BeginPopupModal("Texture Atlas Select", NULL, ImGuiWindowFlags_HorizontalScrollbar))
	{
		static int selection = 0;
		static int prevSelection = 0;

		for (it = icons.begin(); it != icons.end(); it++)
		{
			textureAtlasItem currItem = *it;
			uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
			uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);
			ImGui::PushID(currItem.id);

			ImGui::SameLine();

			if (ImGui::ImageButton(texId, ImVec2(32, 32), uv0, uv1, 0, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255)))
			{
				if (currItem.name == "folder-previous")
				{
					TRACE(T_DEB, "Previous directory: [%s%s]", driveList.at(selection).c_str(), dirPath.c_str());
					size_t found = dirPath.find_last_of("/\\");
					dirPath = dirPath.substr(0, found);
					TRACE(T_DEB, "Current directory: [%s%s]", driveList.at(selection).c_str(), dirPath.c_str());
				}

				if (currItem.name == "refresh")
				{
					this->getDriveLetters();
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

		ImGui::InputText("Filename", m_selectedFile, 128);

		ImGui::Combo("Drive", &selection, driveLetters);

		if (selection != prevSelection)
		{
			TRACE(T_DEB, "Drive Selected [%d]", selection);
			prevSelection = selection;
			dirPath.clear();
		}
		
		ImGui::Text("%s%s", driveList.at(selection).c_str(), dirPath.c_str());
		
		ImGui::Separator();

		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetTextLineHeightWithSpacing() * 1.6));

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

		dl = get_directory_list(driveList.at(selection).c_str() + dirPath);

		for (dlIt = dl.begin(); dlIt != dl.end(); dlIt++)
		{
			sprintf(dirName, "%s", dlIt->c_str());

			ImGui::TextColored(imGreen, dirName);

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				TRACE(T_DEB, "Double click item [%s]", dirName);
				dirPath += "/";
				dirPath += dlIt->c_str();
			}
		}

		dl = get_file_list(driveList.at(selection).c_str() + dirPath, false, "ta");

		for (dlIt = dl.begin(); dlIt != dl.end(); dlIt++)
		{
			sprintf(fileName, "%s", dlIt->c_str());

			ImGui::Text(fileName);

			std::string tmpPath;
			tmpPath.append(driveList.at(selection).c_str());
			tmpPath.append(dirPath);
			tmpPath.append("/");
			tmpPath.append(fileName);
			tmpPath = utils::RemoveExtension(tmpPath);
			tmpPath.append(".png");
			//sprBatch = new SpriteBatch(tmpPath, "preview");

			g2dTexture *t = ServiceLocator::getTextureService()->GetTextureFromFileSystem(tmpPath);//sprBatch->m_textureFileName);
			uv0 = ImVec2(0, 0);
			uv1 = ImVec2(1, 1);

			ImGui::Image((ImTextureID*)t->id, ImVec2(192, 164), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
			//delete sprBatch;

			// on double click populate fullPath with full path of texture atlas file
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				TRACE(T_INF, "Selected file -> %s", m_selectedFile);
				fullPath.append(driveList.at(selection).c_str());
				fullPath.append(dirPath);
				fullPath.append("/");
				fullPath.append(m_selectedFile);
				TRACE(T_INF, "Full Path: [%s]", fullPath.c_str());
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
			{
				TRACE(T_DEB, "Single click file [%s]", fileName);
				strcpy(this->m_selectedFile, fileName);
			}

			if (ImGui::IsItemHovered())
			{
				//std::string tmpPath;
				//tmpPath.append(driveList.at(selection).c_str());
				//tmpPath.append(dirPath);
				//tmpPath.append("/");
				//tmpPath.append(fileName);
				//sprBatch = new SpriteBatch(tmpPath, "preview");
				//g2dTexture *t = ServiceLocator::getTextureService()->GetTexture(sprBatch->m_textureFileName);
				//uv0 = ImVec2(0, 0);
				//uv1 = ImVec2(1, 1);
				//ImGui::BeginTooltip();
				//ImGui::Image((ImTextureID*)t->id, ImVec2(192, 164), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
				//ImGui::EndTooltip();
				//delete sprBatch;
			}

		}

		scrollToBottom = false;

		ImGui::PopStyleVar();

		ImGui::EndChild();

		ImGui::Separator();

		// on load populate fullPath with full path of texture atlas file
		if (ImGui::Button("Load", ImVec2(120, 0)))
		{
			fullPath.append(driveList.at(selection).c_str());
			fullPath.append(dirPath);
			fullPath.append("/");
			fullPath.append(m_selectedFile);
			TRACE(T_INF, "Full Path: [%s]", fullPath.c_str());
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			fullPath = "Cancel";
		}

		if (!fullPath.empty())
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();

	}

	return fullPath;
}

std::string FileDialogWindow::runPNGSelect()
{
	ImVec2 uv0, uv1;
	vector<textureAtlasItem>::iterator it;
	vector<std::string> dl;
	vector<std::string>::iterator dlIt;
	char dirName[MAXPATH];
	char fileName[MAXPATH];
	ImVec4 imGreen(0.0f, 1.0f, 0.0f, 1.0f);
	bool parentDirCreated = false;
	static bool noNameEntered = false;
	std::string fullPath;

	memset(dirName, '\0', sizeof(dirName));
	memset(fileName, '\0', sizeof(fileName));
	
	ImGui::OpenPopup("PNG Image Select");

	ImGui::SetNextWindowSize(ImVec2(384, 512), ImGuiSetCond_FirstUseEver);

	static std::string dirPath;

	if (ImGui::BeginPopupModal("PNG Image Select", NULL, ImGuiWindowFlags_HorizontalScrollbar))
	{
		static int selection = 0;
		static int prevSelection = 0;

		for (it = icons.begin(); it != icons.end(); it++)
		{
			textureAtlasItem currItem = *it;
			uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
			uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);
			ImGui::PushID(currItem.id);

			ImGui::SameLine();

			if (ImGui::ImageButton(texId, ImVec2(32, 32), uv0, uv1, 0, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255)))
			{
				if (currItem.name == "folder-previous")
				{
					TRACE(T_DEB, "Previous directory: [%s%s]", driveList.at(selection).c_str(), dirPath.c_str());
					size_t found = dirPath.find_last_of("/\\");
					dirPath = dirPath.substr(0, found);
					TRACE(T_DEB, "Current directory: [%s%s]", driveList.at(selection).c_str(), dirPath.c_str());
				}

				if (currItem.name == "refresh")
				{
					this->getDriveLetters();
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

		ImGui::InputText("Filename", m_selectedFile, 128);

		ImGui::Combo("Drive", &selection, driveLetters);

		if (selection != prevSelection)
		{
			TRACE(T_DEB, "Drive Selected [%d]", selection);
			prevSelection = selection;
			dirPath.clear();
		}
		ImGui::Text("%s%s", driveList.at(selection).c_str(), dirPath.c_str());
		ImGui::Separator();

		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetTextLineHeightWithSpacing() * 1.6));

		static bool dont_ask_me_next_time = false;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

		dl = get_directory_list(driveList.at(selection).c_str() + dirPath);

		for (dlIt = dl.begin(); dlIt != dl.end(); dlIt++)
		{
			sprintf(dirName, "%s", dlIt->c_str());

			ImGui::TextColored(imGreen, dirName);

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				TRACE(T_DEB, "Double click item [%s]", dirName);
				dirPath += "/";
				dirPath += dlIt->c_str();
			}
		}

		dl = get_file_list(driveList.at(selection).c_str() + dirPath, false, "png");

		for (dlIt = dl.begin(); dlIt != dl.end(); dlIt++)
		{
			sprintf(fileName, "%s", dlIt->c_str());

			ImGui::Text(fileName);

			std::string tmpPath;
			tmpPath.append(driveList.at(selection).c_str());
			tmpPath.append(dirPath);
			tmpPath.append("/");
			tmpPath.append(fileName);
			g2dTexture *t = ServiceLocator::getTextureService()->GetTextureFromFileSystem(tmpPath);
			uv0 = ImVec2(0, 0);
			uv1 = ImVec2(1, 1);
			//ImGui::Text("%s", fileName);
			//ImGui::Text("%dx%d", t->w, t->h);
			ImGui::Image((ImTextureID*)t->id, ImVec2(192, 164), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				TRACE(T_INF, "Selected file -> %s", m_selectedFile);
				fullPath.append(driveList.at(selection).c_str());
				fullPath.append(dirPath);
				fullPath.append("/");
				fullPath.append(m_selectedFile);
				TRACE(T_INF, "Full Path: [%s]", fullPath.c_str());
				ServiceLocator::getTextureService()->GetTexture(fullPath.c_str());
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
			{
				TRACE(T_DEB, "Single click file [%s]", fileName);
				strcpy(this->m_selectedFile, fileName);
			}
			
			if (ImGui::IsItemHovered())
			{
				/*std::string tmpPath;
				tmpPath.append(driveList.at(selection).c_str());
				tmpPath.append(dirPath);
				tmpPath.append("/");
				tmpPath.append(fileName);
				g2dTexture *t = ServiceLocator::getTextureService()->GetTexture(tmpPath);
				uv0 = ImVec2(0, 0);
				uv1 = ImVec2(1, 1); 
				ImGui::BeginTooltip();
				ImGui::Text("%s", fileName);
				ImGui::Text("%dx%d", t->w, t->h);
				ImGui::Image((ImTextureID*)t->id, ImVec2(256,192	), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
				ImGui::EndTooltip();*/
			}

		}

		scrollToBottom = false;

		ImGui::PopStyleVar();

		ImGui::EndChild();

		ImGui::Separator();

		if (ImGui::Button("Load", ImVec2(120, 0)))
		{
			TRACE(T_INF, "Selected file -> %s", m_selectedFile);
			fullPath.append(driveList.at(selection).c_str());
			fullPath.append(dirPath);
			fullPath.append("/");
			fullPath.append(m_selectedFile);
			TRACE(T_INF, "Full Path: [%s]", fullPath.c_str());
			ServiceLocator::getTextureService()->GetTexture(fullPath.c_str());
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			fullPath = "Cancel";
			//ImGui::CloseCurrentPopup();
			//ServiceLocator::getImguiService()->fileDialogWindow->switchVisibility();
		}

		if (noNameEntered)
		{
			noNameEntered = this->showModalWindow("Enter a name for your game", "You need to enter a name for \nyou game in the text box above!\n\n");
		}
		
		if (!fullPath.empty())
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();

	}

	return fullPath;
}

void FileDialogWindow::run(bool* opened)
{
	ImVec2 uv0, uv1;
	vector<textureAtlasItem>::iterator it;
	vector<std::string> dl;
	vector<std::string>::iterator dlIt;
	ImGui::OpenPopup("File Operations");
	char dirName[MAXPATH];
	char fileName[MAXPATH];
	char fileToOpen[MAXPATH];
	ImVec4 imGreen(0.0f, 1.0f, 0.0f, 1.0f);
	bool parentDirCreated = false;
	static bool noNameEntered = false;

	memset(dirName, '\0', sizeof(dirName));
	memset(fileName, '\0', sizeof(fileName));
	memset(fileToOpen, '\0', sizeof(fileToOpen));

	ImGui::SetNextWindowSize(ImVec2(384, 512), ImGuiSetCond_FirstUseEver);

	static std::string dirPath;

	if (ImGui::BeginPopupModal("File Operations", NULL, ImGuiWindowFlags_HorizontalScrollbar))
	{
		static int selection = 0;
		static int prevSelection = 0;

		for (it = icons.begin(); it != icons.end(); it++)
		{
			textureAtlasItem currItem = *it;
			uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
			uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);
			ImGui::PushID(currItem.id);

			ImGui::SameLine();

			if (ImGui::ImageButton(texId, ImVec2(32, 32), uv0, uv1, 0, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255)))
			{
				if (currItem.name == "texturepacker")
				{
					if (ServiceLocator::getGameService()->gameIsCreated())
					{
						char tmp[128];
						sprintf(tmp, "media/%s", this->m_selectedFile);
						std::string fName = tmp;
						//ServiceLocator::getLevelService()->addLayer("fName", fName, 1.f, 1.f, 1.f);
					}
				}

				if (currItem.name == "folder-previous")
				{
					TRACE(T_DEB, "Previous directory: [%s%s]", driveList.at(selection).c_str(), dirPath.c_str());
					size_t found = dirPath.find_last_of("/\\");
					dirPath = dirPath.substr(0, found);
					TRACE(T_DEB, "Current directory: [%s%s]", driveList.at(selection).c_str(), dirPath.c_str());
				}

				if (currItem.name == "refresh")
				{
					this->getDriveLetters();
				}

				if (currItem.name == "savemap")
				{
					TRACE(T_DEB, "About to save [%s]", fileName);
					//ServiceLocator::getGameService()->saveGame();
				}

				if (currItem.name == "newmap")
				{
					TRACE(T_DEB, "About to create new map file");

					if (strcmp(m_selectedFile, "No File Selected"))
					{
						char dirCreatePath[1024];
						char mediaPath[1024];
						int dirCreLen = 0;
						// 1. Create game directory <game>
						// 2. Create media directory <game\media>
						// 3. Create levels directory <game\media\levels>
						// 4. Create music directory <game\media\music>
						// 5. Create sounds directory <game\media\sounds>
						// 6. Create texturepacks directory <game\media\texturepacks>
						// 7. Create textures directory <game\media\textures>
						//sprintf(dirCreatePath, "%s%s", driveList.at(selection).c_str(),m_selectedFile);
#if defined(PLATFORM_LINUX)
						sprintf(dirCreatePath, "%s", m_selectedFile);
						parentDirCreated = make_directory(dirCreatePath);
						TRACE(T_DEB, "Path -> %s%s/%s", driveList.at(selection).c_str(), dirPath.c_str(), m_selectedFile);
#else
						sprintf(dirCreatePath, "%s%s/%s", driveList.at(selection).c_str(), dirPath.c_str(), m_selectedFile);
						parentDirCreated = make_directory(dirCreatePath);
						TRACE(T_DEB, "Path -> %s", dirCreatePath);
#endif
						if (!parentDirCreated)
						{
							TRACE(T_DEB, "Failed to create new game directory");
						}
						else
						{
							TiXmlDocument doc;

							char gameFileName[128];

							TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
							doc.LinkEndChild(decl);

							sprintf(fileName, "%s.xml", this->m_name.c_str());

							TiXmlElement * root = new TiXmlElement("game");
							root->SetAttribute("name", m_selectedFile);
							doc.LinkEndChild(root);
							sprintf(gameFileName, "%s/%s.xml", dirCreatePath, m_selectedFile);
							doc.SaveFile(gameFileName);
							doc.Clear();

							TRACE(T_DEB, "About to save %s", gameFileName);

							sprintf(mediaPath, "%s/%s", dirCreatePath, "media");
							make_directory(mediaPath);

							sprintf(dirCreatePath, "%s/%s", mediaPath, "levels");
							make_directory(dirCreatePath);

							sprintf(dirCreatePath, "%s/%s", mediaPath, "music");
							make_directory(dirCreatePath);

							sprintf(dirCreatePath, "%s/%s", mediaPath, "sounds");
							make_directory(dirCreatePath);

							sprintf(dirCreatePath, "%s/%s", mediaPath, "texturepacks");
							make_directory(dirCreatePath);

							sprintf(dirCreatePath, "%s/%s", mediaPath, "textures");
							make_directory(dirCreatePath);

							//ServiceLocator::getGameService()->initGame(m_selectedFile);			

							//TRACE(T_DEB, "Create new game file [%s]", ServiceLocator::getGameService()->getGameFileName().c_str());
						}
					}
					else
					{
						TRACE(T_DEB, "Please type a filename for the new game");
						noNameEntered = true;
					}
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

		ImGui::InputText("Filename", m_selectedFile, 128);

		ImGui::Combo("Drive", &selection, driveLetters);

		if (selection != prevSelection)
		{
			TRACE(T_DEB, "Drive Selected [%d]", selection);
			prevSelection = selection;
			dirPath.clear();
		}
		ImGui::Text("%s%s", driveList.at(selection).c_str(), dirPath.c_str());
		ImGui::Separator();

		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetTextLineHeightWithSpacing() * 1.6));

		static bool dont_ask_me_next_time = false;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

		dl = get_directory_list(driveList.at(selection).c_str() + dirPath);

		for (dlIt = dl.begin(); dlIt != dl.end(); dlIt++)
		{
			sprintf(dirName, "%s", dlIt->c_str());

			ImGui::TextColored(imGreen, dirName);

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				TRACE(T_DEB, "Double click item [%s]", dirName);
				dirPath += "/";
				dirPath += dlIt->c_str();
			}
		}

		dl = get_file_list(driveList.at(selection).c_str() + dirPath, false, "xml");

		for (dlIt = dl.begin(); dlIt != dl.end(); dlIt++)
		{
			sprintf(fileName, "%s", dlIt->c_str());

			ImGui::Text(fileName);

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				TRACE(T_DEB, "Double click file [%s]", m_selectedFile);
				//ServiceLocator::getLevelService()->load(m_selectedFile);
				//ServiceLocator::getLevelService()->createPhysicalWorld();
				//ServiceLocator::getImguiService()->spriteSelector->setSpriteBatch(ServiceLocator::getLevelService()->getCurrentLayer()->getSpriteBatch());
				ServiceLocator::getImguiService()->getToolBar("SpriteSelector")->isVisible = true;
				ImGui::CloseCurrentPopup();
				ServiceLocator::getImguiService()->fileDialogWindow->switchVisibility();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
			{
				TRACE(T_DEB, "Single click file [%s]", fileName);
				strcpy(this->m_selectedFile, fileName);
			}
		}

		scrollToBottom = false;

		ImGui::PopStyleVar();

		ImGui::EndChild();

		ImGui::Separator();

		if (ImGui::Button("Load", ImVec2(120, 0)))
		{
			TRACE(T_INF, "Selected file -> %s", m_selectedFile);
			//ServiceLocator::getLevelService()->load(m_selectedFile);
			//ServiceLocator::getLevelService()->createPhysicalWorld();
			//ServiceLocator::getImguiService()->spriteSelector->setSpriteBatch(ServiceLocator::getLevelService()->getCurrentLayer()->getSpriteBatch());
			ServiceLocator::getImguiService()->spriteSelector->isVisible = true;
			ImGui::CloseCurrentPopup();
			ServiceLocator::getImguiService()->fileDialogWindow->switchVisibility();
		}

		ImGui::SameLine();

		if (ImGui::Button("Save", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			ServiceLocator::getImguiService()->fileDialogWindow->switchVisibility();
		}
		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			ServiceLocator::getImguiService()->fileDialogWindow->switchVisibility();
		}

		if (noNameEntered)
		{
			noNameEntered=this->showModalWindow("Enter a name for your game","You need to enter a name for \nyou game in the text box above!\n\n");
		}

		ImGui::EndPopup();
		
	}
}

std::string FileDialogWindow::runDirSelect()
{
	ImVec2 uv0, uv1;
	vector<textureAtlasItem>::iterator it;
	vector<std::string> dl;
	vector<std::string>::iterator dlIt;

	char dirName[MAXPATH];
	char fileName[MAXPATH];
	std::string fullPath;

	ImVec4 imGreen(0.0f, 1.0f, 0.0f, 1.0f);

	memset(dirName, '\0', sizeof(dirName));
	memset(fileName, '\0', sizeof(fileName));

	ImGui::OpenPopup("Select Game Root Directory");

	ImGui::SetNextWindowSize(ImVec2(384, 512), ImGuiSetCond_FirstUseEver);

	static std::string dirPath;

	if (ImGui::BeginPopupModal("Select Game Root Directory", NULL, ImGuiWindowFlags_HorizontalScrollbar))
	{
		static int selection = 0;
		static int prevSelection = 0;

		for (it = icons.begin(); it != icons.end(); it++)
		{
			textureAtlasItem currItem = *it;
			uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
			uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);
			ImGui::PushID(currItem.id);

			ImGui::SameLine();

			if (ImGui::ImageButton(texId, ImVec2(32, 32), uv0, uv1, 0, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255)))
			{
				if (currItem.name == "folder-previous")
				{
					TRACE(T_DEB, "Previous directory: [%s%s]", driveList.at(selection).c_str(), dirPath.c_str());
					size_t found = dirPath.find_last_of("/\\");
					dirPath = dirPath.substr(0, found);
					TRACE(T_DEB, "Current directory: [%s%s]", driveList.at(selection).c_str(), dirPath.c_str());
				}

				if (currItem.name == "refresh")
				{
					this->getDriveLetters();
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

		ImGui::InputText("Filename", m_selectedFile, 128);

		ImGui::Combo("Drive", &selection, driveLetters);

		if (selection != prevSelection)
		{
			TRACE(T_DEB, "Drive Selected [%d]", selection);
			prevSelection = selection;
			dirPath.clear();
		}

		ImGui::Text("%s%s", driveList.at(selection).c_str(), dirPath.c_str());

		ImGui::Separator();

		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetTextLineHeightWithSpacing() * 1.6));

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

		dl = get_directory_list(driveList.at(selection).c_str() + dirPath);

		for (dlIt = dl.begin(); dlIt != dl.end(); dlIt++)
		{
			sprintf(dirName, "%s", dlIt->c_str());

			ImGui::TextColored(imGreen, dirName);

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				TRACE(T_DEB, "Double click item [%s]", dirName);
				dirPath += "/";
				dirPath += dlIt->c_str();
			}
		}

		scrollToBottom = false;

		ImGui::PopStyleVar();

		ImGui::EndChild();

		ImGui::Separator();

		// on load populate fullPath with full path of texture atlas file
		if (ImGui::Button("Select", ImVec2(120, 0)))
		{
			fullPath.append(driveList.at(selection).c_str());
			fullPath.append(dirPath);
			fullPath.append("/");
			TRACE(T_INF, "Directory Selected [%s]", fullPath.c_str());
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			fullPath = "Cancel";
		}

		if (!fullPath.empty())
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();

	}

	return fullPath;
}

std::string FileDialogWindow::runSelectXmlFileName(char *dialoguName)
{
	std::string fullPath;

	ImVec2 uv0, uv1;
	vector<textureAtlasItem>::iterator it;
	vector<std::string> dl;
	vector<std::string>::iterator dlIt;
	ImGui::OpenPopup(dialoguName);
	char dirName[MAXPATH];
	char fileName[MAXPATH];
	char fileToOpen[MAXPATH];
	ImVec4 imGreen(0.0f, 1.0f, 0.0f, 1.0f);
	bool parentDirCreated = false;
	static bool noNameEntered = false;

	memset(dirName, '\0', sizeof(dirName));
	memset(fileName, '\0', sizeof(fileName));
	memset(fileToOpen, '\0', sizeof(fileToOpen));

	ImGui::SetNextWindowSize(ImVec2(384, 512), ImGuiSetCond_FirstUseEver);

	static std::string dirPath;

	if (ImGui::BeginPopupModal(dialoguName, NULL, ImGuiWindowFlags_HorizontalScrollbar))
	{
		static int selection = 0;
		static int prevSelection = 0;

		for (it = icons.begin(); it != icons.end(); it++)
		{
			textureAtlasItem currItem = *it;
			uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
			uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);
			ImGui::PushID(currItem.id);

			ImGui::SameLine();

			if (ImGui::ImageButton(texId, ImVec2(32, 32), uv0, uv1, 0, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255)))
			{
				if (currItem.name == "folder-previous")
				{
					TRACE(T_DEB, "Previous directory: [%s%s]", driveList.at(selection).c_str(), dirPath.c_str());
					size_t found = dirPath.find_last_of("/\\");
					dirPath = dirPath.substr(0, found);
					TRACE(T_DEB, "Current directory: [%s%s]", driveList.at(selection).c_str(), dirPath.c_str());
				}

				if (currItem.name == "refresh")
				{
					this->getDriveLetters();
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

		ImGui::InputText("Filename", m_selectedFile, 128);

		ImGui::Combo("Drive", &selection, driveLetters);

		if (selection != prevSelection)
		{
			TRACE(T_DEB, "Drive Selected [%d]", selection);
			prevSelection = selection;
			dirPath.clear();
		}
		ImGui::Text("%s%s", driveList.at(selection).c_str(), dirPath.c_str());
		ImGui::Separator();

		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetTextLineHeightWithSpacing() * 1.6));

		static bool dont_ask_me_next_time = false;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

		dl = get_directory_list(driveList.at(selection).c_str() + dirPath);

		for (dlIt = dl.begin(); dlIt != dl.end(); dlIt++)
		{
			sprintf(dirName, "%s", dlIt->c_str());

			ImGui::TextColored(imGreen, dirName);

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				TRACE(T_DEB, "Double click item [%s]", dirName);
				dirPath += "/";
				dirPath += dlIt->c_str();
			}
		}

		dl = get_file_list(driveList.at(selection).c_str() + dirPath, false, "xml");

		for (dlIt = dl.begin(); dlIt != dl.end(); dlIt++)
		{
			sprintf(fileName, "%s", dlIt->c_str());

			ImGui::Text(fileName);

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				TRACE(T_DEB, "Double click file [%s]", fileName);
				strcpy(this->m_selectedFile, fileName);
				fullPath = driveList.at(selection).c_str();
				fullPath += dirPath;
				fullPath += "/";
				fullPath += m_selectedFile;
				ImGui::CloseCurrentPopup();

			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
			{
				TRACE(T_DEB, "Single click file [%s]", fileName);
				strcpy(this->m_selectedFile, fileName);
			}
		}

		scrollToBottom = false;

		ImGui::PopStyleVar();

		ImGui::EndChild();

		ImGui::Separator();

		if (ImGui::Button("Load", ImVec2(120, 0)))
		{
			fullPath = driveList.at(selection).c_str();
			fullPath += dirPath;
			fullPath += "/";
			fullPath += m_selectedFile;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			fullPath = "Cancel";
			ImGui::CloseCurrentPopup();
		}

		if (noNameEntered)
		{
			noNameEntered = this->showModalWindow("Enter a name for your game", "You need to enter a name for \nyou game in the text box above!\n\n");
		}

		ImGui::EndPopup();

	}

	return fullPath;
}

