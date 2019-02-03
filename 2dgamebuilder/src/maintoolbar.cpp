#include <vector>
#include <imgui/imgui.h>
#include "maintoolbar.h"
#include "servicelocator.h"
#include "physicsstate.h"
#include "editstate.h"
#include <benchmarkstate.h>
#include <sandboxstate.h>

MainToolBar::MainToolBar(TextureAtlas *textureAtlas, string name):ImGUIComponent(textureAtlas,name)
{
	this->addIcons();
}

MainToolBar::~MainToolBar()
{
}

void MainToolBar::addIcons()
{
	// populate the toolbar with icons in the same order you want them to appear

	textureAtlasItem taItem;

	taItem = m_textureAtlas->getItem("about");
	taItem.hint = "About the author of this software";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("hint");
	taItem.hint = "Disaply hints when mouse is over icons";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("mario");
	taItem.hint = "Game related operations";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("soundeffects");
	taItem.hint = "Sound effects";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("filemanager");
	taItem.hint = "File related operations";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("layers");
	taItem.hint = "Show toolbar that allows layer operations";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("editmode");
	taItem.hint = "Enable edit mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("box2d");
	taItem.hint = "Enable Box2d physics mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("animation");
	taItem.hint = "Enable animation mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("camera");
	taItem.hint = "Camera Controls";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("fullscreen");
	taItem.hint = "Switch the editor from window to fullscreen mode or from fullscreen to window mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("fps");
	taItem.hint = "Show rendering statistics in a seperate window";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("debug");
	taItem.hint = "Draw debug lines arround objecs of current layer";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("logs");
	taItem.hint = "Show the logging window that holds level editor traces";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("gamemode");
	taItem.hint = "Switch to gamemode to test gameplay";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("gamepadconfig");
	taItem.hint = "Configure the gamepad";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("editor-config");
	taItem.hint = "Show window that allows for level editor configuration";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("wireframe");
	taItem.hint = "Show or hide the grid that helps visualize objects alignment when drawing in tilemode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("tilelayout");
	taItem.hint = "";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("psp");
	taItem.hint = "Show PSP portion of screen";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("snapshot");
	taItem.hint = "Take a snapshot of the editor";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("sandcastle");
	taItem.hint = "Sandbox Experimenting with ideas";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("gpu");
	taItem.hint = "GPU Benchmarking";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("exit");
	taItem.hint = "Exit the editor";
	icons.push_back(taItem);
}

void MainToolBar::run(bool* opened)
{
	ImVec2 uv0, uv1;
	
	vector<textureAtlasItem>::iterator it;

	static bool exitNow = false;
	static int row = 0;

	ImGui::Begin(m_name.c_str(), opened, ImVec2(100, 44 * icons.size()/2), 0.70f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
	{

		for (it = icons.begin(); it != icons.end(); it++)
		{
			textureAtlasItem currItem = *it;
			uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
			uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);

			ImGui::PushID(currItem.id);

			if (ImGui::ImageButton(texId, ImVec2(40, 40), uv0, uv1, 0, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255)))
			{

				// show psp screen guideline
				if (currItem.name == "psp")
				{
					if (ServiceLocator::getConfigurationService()->pEditor_drawpspbox)
					{
						TRACE(T_INF, "Enable drawing of real PSP Device and screen");
						ServiceLocator::getConfigurationService()->pEditor_drawpspbox = false;
					}
					else
					{
						TRACE(T_INF, "Disable drawing of real PSP Device and screen");
						ServiceLocator::getConfigurationService()->pEditor_drawpspbox = true;
					}
				}

				// show grid properties
				if (currItem.name == "tilelayout")
				{
					TRACE(T_INF, "Switching on/off GridProperties visibility");
					ServiceLocator::getImguiService()->gridPropertiesWindow->switchVisibility();
				}

	
				// show wireframe 
				if (currItem.name == "wireframe")
				{		
					if (ServiceLocator::getConfigurationService()->pEditor_drawwireframe)
					{
						TRACE(T_INF, "Disable Wireframe");
						ServiceLocator::getConfigurationService()->pEditor_drawwireframe = false;
					}
					else
					{
						TRACE(T_INF, "Enable Wireframe");
						ServiceLocator::getConfigurationService()->pEditor_drawwireframe = true;
					}
				}

				// draw debug lines arround objects of current layer
				if (currItem.name == "debug")
				{
					if (ServiceLocator::getConfigurationService()->pEditor_drawdebuglines)
					{
						TRACE(T_INF, "Disable debug line drawing for current layer");
						ServiceLocator::getConfigurationService()->pEditor_drawdebuglines = false;
					}
					else
					{
						TRACE(T_INF, "Enable debug line drawing for current layer");
						ServiceLocator::getConfigurationService()->pEditor_drawdebuglines= true;
					}
				}

				// enable or disable fps information

				if (currItem.name == "fps")
				{
					if (ServiceLocator::getConfigurationService()->pEditor_fps)
					{
						ServiceLocator::getConfigurationService()->pEditor_fps = false;
					}
					else
					{
						ServiceLocator::getConfigurationService()->pEditor_fps = true;
					}
				}

				// exit the level editor

				if (currItem.name == "exit")
				{
					exitNow = true;
				}

				// switch to fullscreen

				if (currItem.name == "fullscreen")
				{
					TRACE(T_INF, "Switching to/from Full Screen");
					ServiceLocator::getRenderService()->swithFullScreen();
				}

				// show hints on eache button of the toolbar

				if (currItem.name == "hint")
				{
					ServiceLocator::getConfigurationService()->switchHints();						
				}

				// show loggee window
				
				if (currItem.name == "logs")
				{
					ServiceLocator::getImguiService()->loggerWindow->switchVisibility();
				}

				// show editor preferences window

				if (currItem.name == "editor-config")
				{
					ServiceLocator::getImguiService()->preferencesWindow->switchVisibility();
				}

				// show file dialog window 

				if (currItem.name == "filemanager")
				{
					ServiceLocator::getImguiService()->fileDialogWindow->switchVisibility();					
				}

				// show editbar
				if (currItem.name == "editmode")
				{
					ServiceLocator::getImguiService()->editBar->switchVisibility();
					ServiceLocator::getStateService()->m_app->changeState(CEditState::Instance());

					TRACE(T_DEB, "Show editbar and switch from IDLE to EDIT state");
				}

				// show physicbar
				if (currItem.name == "box2d")
				{
					ServiceLocator::getStateService()->m_app->changeState(CPhysicsState::Instance());
					ServiceLocator::getImguiService()->editBar->isVisible = false;
					TRACE(T_DEB, "State set to PHYSIC State");
				}

				// show layerbar
				if (currItem.name == "layers")
				{
					ServiceLocator::getImguiService()->layerBar->switchVisibility();
				}

				if (currItem.name == "mario")
				{
					ServiceLocator::getImguiService()->gameBar->switchVisibility();
				}

				if (currItem.name == "snapshot")
				{
					//ServiceLocator::getImguiService()->screenShotWindow->switchVisibility();
					//TRACE(T_DEB, "Enable screenshot tools window");
					ServiceLocator::getRenderService()->m_takeSnapShot = true;
				}

				if (currItem.name == "camera")
				{
					ServiceLocator::getImguiService()->cameraPropertiesWindow->switchVisibility();
					TRACE(T_DEB, "Bring up camera properties");
				}

				// show benchmark state
				if (currItem.name == "gpu")
				{
					ServiceLocator::getStateService()->m_app->changeState(CBenchMarkState::Instance());

					TRACE(T_DEB, "Switch to BenchMark State");
				}

				// show sandbox state

				if (currItem.name == "sandcastle")
				{
					ServiceLocator::getStateService()->m_app->changeState(CSandBoxState::Instance());

					TRACE(T_DEB, "Switch to SandBox State");
				}

			}

			row++;

			if ((row%2)!=0) ImGui::SameLine();

			// for every icon display hint if hint is set and hints are enabled

			if (ServiceLocator::getConfigurationService()->pEditor_hints==true)
			{
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip(currItem.hint.c_str());
				}
			}
	
			ImGui::PopID();
		}
	}

	ImGui::End();

	if (exitNow)
	{
		ImGui::OpenPopup("Are you sure you want to exit?");
	
		if (ImGui::BeginPopupModal("Are you sure you want to exit?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			textureAtlasItem currItem = m_textureAtlas->getItem("warning");
			
			uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
			uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);

			ImGui::Image(texId, ImVec2(46, 46), uv0, uv1, ImColor(255, 255, 255, 255),ImColor(100, 100, 100, 100));
			ImGui::SameLine();
			ImGui::Text("Your beautiful level will be deleted.\n This operation cannot be undone!\n\n");

			ImGui::Separator();

			static bool dont_ask_me_next_time = false;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
			ImGui::PopStyleVar();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				exitNow = false;
				ServiceLocator::getConfigurationService()->saveConfigFile();
				exit(0);
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				exitNow = false;
			}
			ImGui::EndPopup();
		}
	}
	

}

