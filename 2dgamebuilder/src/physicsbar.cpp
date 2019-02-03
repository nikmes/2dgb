#include "physicsbar.h"
#include <imgui/imgui.h>
#include "servicelocator.h"

PhysicsBar::PhysicsBar(TextureAtlas *textureAtlas, string name) :ImGUIComponent(textureAtlas, name)
{
	this->addIcons();
}

PhysicsBar::~PhysicsBar()
{

}

void PhysicsBar::run(bool *opened)
{
	ImVec2 uv0, uv1;

	vector<textureAtlasItem>::iterator it;

	ImGui::Begin(m_name.c_str(), opened, ImVec2(44 * icons.size(), 46), 0.70f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
	{

		for (it = icons.begin(); it != icons.end(); it++)
		{
			textureAtlasItem currItem = *it;
			uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
			uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);
			
			ImGui::PushID(currItem.id);

			ImGui::SameLine();

			if (ImGui::ImageButton(texId, ImVec2(40, 40), uv0, uv1, 0, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255)))
			{
				if (currItem.name == "tilemode")
				{
					//ServiceLocator::getConfigurationService()->pEditor_draingmode = TILED;
				}

				if (currItem.name == "freehandmode")
				{
					//ServiceLocator::getConfigurationService()->pEditor_draingmode = FREEHAND;
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

	ImGui::End();
}

void PhysicsBar::addIcons()
{
	// populate the toolbar with icons in the same order you want them to appear
	textureAtlasItem taItem;

	taItem = m_textureAtlas->getItem("tilemode");
	taItem.hint = "Set drawing mode to tile mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("freehandmode");
	taItem.hint = "Set drawing mode to free hand mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("fliphorizontal");
	taItem.hint = "Flip selected sprite horizontaly";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("flipvertical");
	taItem.hint = "Flip selected sprite verticaly";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("rotate");
	taItem.hint = "Enable rotation of the selected sprite";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("scale");
	taItem.hint = "Enable scaling of the selected sprite";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("hand");
	taItem.hint = "Enable dragging mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("move");
	taItem.hint = "Enable moving mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("delete");
	taItem.hint = "Enable selecte and delete sprites mode";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("transparency");
	taItem.hint = "Set transparency of the selected sprite";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("properties");
	taItem.hint = "Show window with current sprite properties";
	icons.push_back(taItem);

}