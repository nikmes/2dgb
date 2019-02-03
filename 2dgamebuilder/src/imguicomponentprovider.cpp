#define IMGUI_DEFINE_MATH_OPERATORS
#include "imguicomponentprovider.h"
#include <imgui_internal.h> // ImSaturate

ImguiComponentProvider::ImguiComponentProvider()
{

}

ImguiComponentProvider::~ImguiComponentProvider()
{

}

void ImguiComponentProvider::addImguiComponent(ImGUIComponent *imguiComponent)
{
	// add toolbar to the list of toolbars
	this->imguiComponents.insert((std::pair<string, ImGUIComponent*>(imguiComponent->getName(), imguiComponent)));
}

void ImguiComponentProvider::render()
{
	// render all toolbars on screen if they are visible

	map<string, ImGUIComponent*>::iterator it;

	for (it = imguiComponents.begin(); it != imguiComponents.end(); it++)
	{
		if ((it)->second->isVisible)
		{
			(it)->second->run(&(it)->second->isVisible);
		}
	}
}

ImGUIComponent* ImguiComponentProvider::getImguiComponent(string imguiComponentName)
{
	return this->imguiComponents.at(imguiComponentName);
}

bool ImguiComponentProvider::colorPicker4(float* col, bool show_alpha)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	// Setup
	ImVec2 picker_pos = ImGui::GetCursorScreenPos();
	ImVec2 sv_picker_size = ImVec2(256.0f, 256.0f);                             // Saturation/Value picking box
	float bars_width = ImGui::GetWindowFontSize() + style.FramePadding.y*2.0f;  // Width of Hue/Alpha picking bars (using Framepadding.y to match the ColorButton sides)
	float bar0_pos_x = picker_pos.x + sv_picker_size.x + style.ItemInnerSpacing.x;
	float bar1_pos_x = bar0_pos_x + bars_width + style.ItemInnerSpacing.x;

	float H, S, V;
	ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], H, S, V);

	// Color matrix logic
	bool value_changed = false, hsv_changed = false;
	ImGui::BeginGroup();
	ImGui::InvisibleButton("sv", sv_picker_size);
	if (ImGui::IsItemActive())
	{
		S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size.x - 1));
		V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size.y - 1));
		value_changed = hsv_changed = true;
	}

	// Hue bar logic
	ImGui::SetCursorScreenPos(ImVec2(bar0_pos_x, picker_pos.y));
	ImGui::InvisibleButton("hue", ImVec2(bars_width, sv_picker_size.y));
	if (ImGui::IsItemActive())
	{
		H = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size.y - 1));
		value_changed = hsv_changed = true;
	}

	// Alpha bar logic
	if (show_alpha)
	{
		ImGui::SetCursorScreenPos(ImVec2(bar1_pos_x, picker_pos.y));
		ImGui::InvisibleButton("alpha", ImVec2(bars_width, sv_picker_size.y));
		if (ImGui::IsItemActive())
		{
			col[3] = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size.y - 1));
			value_changed = true;
		}
	}

	// Convert back to RGB
	if (hsv_changed)
		ImGui::ColorConvertHSVtoRGB(H >= 1.0f ? H - 10 * 1e-6f : H, S > 0.0f ? S : 10 * 1e-6f, V > 0.0f ? V : 1e-6f, col[0], col[1], col[2]);

	// R,G,B or H,S,V color editor
	ImGui::PushItemWidth((show_alpha ? bar1_pos_x : bar0_pos_x) + bars_width - picker_pos.x);
	value_changed |= show_alpha ? ImGui::ColorEdit4("##edit", col) : ImGui::ColorEdit3("##edit", col);
	ImGui::PopItemWidth();

	// Try to cancel hue wrap (after ColorEdit), if any
	if (value_changed)
	{
		float new_H, new_S, new_V;
		ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
		if (new_H <= 0 && H > 0)
		{
			if (new_V <= 0 && V != new_V)
				ImGui::ColorConvertHSVtoRGB(H, S, new_V <= 0 ? V * 0.5f : new_V, col[0], col[1], col[2]);
			else if (new_S <= 0)
				ImGui::ColorConvertHSVtoRGB(H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[0], col[1], col[2]);
		}
	}

	// Render hue bar
	ImU32 hue_colors[] = { IM_COL32(255,0,0,255), IM_COL32(255,255,0,255), IM_COL32(0,255,0,255), IM_COL32(0,255,255,255), IM_COL32(0,0,255,255), IM_COL32(255,0,255,255), IM_COL32(255,0,0,255) };
	for (int i = 0; i < 6; ++i)
	{
		draw_list->AddRectFilledMultiColor(
			ImVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size.y / 6)),
			ImVec2(bar0_pos_x + bars_width, picker_pos.y + (i + 1) * (sv_picker_size.y / 6)),
			hue_colors[i], hue_colors[i], hue_colors[i + 1], hue_colors[i + 1]);
	}
	float bar0_line_y = (float)(int)(picker_pos.y + H * sv_picker_size.y + 0.5f);
	draw_list->AddLine(ImVec2(bar0_pos_x - 1, bar0_line_y), ImVec2(bar0_pos_x + bars_width + 1, bar0_line_y), IM_COL32_WHITE);

	// Render alpha bar
	if (show_alpha)
	{
		float alpha = ImSaturate(col[3]);
		float bar1_line_y = (float)(int)(picker_pos.y + (1.0f - alpha) * sv_picker_size.y + 0.5f);
		draw_list->AddRectFilledMultiColor(ImVec2(bar1_pos_x, picker_pos.y), ImVec2(bar1_pos_x + bars_width, picker_pos.y + sv_picker_size.y), IM_COL32_WHITE, IM_COL32_WHITE, IM_COL32_BLACK, IM_COL32_BLACK);
		draw_list->AddLine(ImVec2(bar1_pos_x - 1, bar1_line_y), ImVec2(bar1_pos_x + bars_width + 1, bar1_line_y), IM_COL32_WHITE);
	}

	// Render color matrix
	ImVec4 hue_color_f(1, 1, 1, 1);
	ImGui::ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
	ImU32 hue_color32 = ImGui::ColorConvertFloat4ToU32(hue_color_f);
	draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + sv_picker_size, IM_COL32_WHITE, hue_color32, hue_color32, IM_COL32_WHITE);
	draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + sv_picker_size, IM_COL32_BLACK_TRANS, IM_COL32_BLACK_TRANS, IM_COL32_BLACK, IM_COL32_BLACK);

	// Render cross-hair
	const float CROSSHAIR_SIZE = 7.0f;
	ImVec2 p((float)(int)(picker_pos.x + S * sv_picker_size.x + 0.5f), (float)(int)(picker_pos.y + (1 - V) * sv_picker_size.y + 0.5f));
	draw_list->AddLine(ImVec2(p.x - CROSSHAIR_SIZE, p.y), ImVec2(p.x - 2, p.y), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x + CROSSHAIR_SIZE, p.y), ImVec2(p.x + 2, p.y), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x, p.y + CROSSHAIR_SIZE), ImVec2(p.x, p.y + 2), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x, p.y - CROSSHAIR_SIZE), ImVec2(p.x, p.y - 2), IM_COL32_WHITE);
	ImGui::EndGroup();

	return value_changed;
}

void ImguiComponentProvider::runPNGSelect(bool* opened)
{
	/*
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
					if (ServiceLocator::getLevelService()->m_levelIsLoaded)
					{
						char tmp[128];
						sprintf(tmp, "media/%s", this->m_selectedFile);
						std::string fName = tmp;
						ServiceLocator::getLevelService()->addLayer("fName", fName, 1.f, 1.f, 1.f);
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
					ServiceLocator::getLevelService()->save();
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

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				TRACE(T_INF, "Selected file -> %s", m_selectedFile);
				std::string fullPath;
				fullPath.append(driveList.at(selection).c_str());
				fullPath.append(dirPath);
				fullPath.append("/");
				fullPath.append(m_selectedFile);
				TRACE(T_INF, "Full Path: [%s]", fullPath.c_str());
				ServiceLocator::getTextureService()->GetTexture(fullPath.c_str());
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
			std::string fullPath;
			fullPath.append(driveList.at(selection).c_str());
			fullPath.append(dirPath);
			fullPath.append("/");
			fullPath.append(m_selectedFile);
			TRACE(T_INF, "Full Path: [%s]", fullPath.c_str());
			ServiceLocator::getTextureService()->GetTexture(fullPath.c_str());
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
			noNameEntered = this->showModalWindow("Enter a name for your game", "You need to enter a name for \nyou game in the text box above!\n\n");
		}

		ImGui::EndPopup();

	}*/
}