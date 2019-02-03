#include "layerbar.h"
#include <imgui/imgui.h>
#include "servicelocator.h"

LayerBar::LayerBar(TextureAtlas *textureAtlas, string name) :ImGUIComponent(textureAtlas, name)
{
	this->addIcons();
}

LayerBar::~LayerBar()
{

}

void LayerBar::run(bool *opened)
{
	static bool showLayerProperties = false;
	ImVec2 uv0, uv1;

	vector<textureAtlasItem>::iterator it;

	ImGui::Begin(m_name.c_str(), opened, ImVec2(44 * icons.size()+16, 46), 0.70f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
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
				if (currItem.name == "next")
				{
					TRACE(T_INF, "Set spriteselector->spritebatch to point at next layer spritebatch");
					//Layer *l = ServiceLocator::getGameService()->getCurrentLevel()->getCurrentLayer();
					//ServiceLocator::getImguiService()->spriteSelector->setSpriteBatch(l->getSpriteBatch());
				}

				if (currItem.name == "previous")
				{
					TRACE(T_INF, "Set spriteselector->spritebatch to point at previous layer spritebatch");
					//Layer *l = ServiceLocator::getGameService()->getCurrentLevel()->getPreviousLayer();
					//ServiceLocator::getImguiService()->spriteSelector->setSpriteBatch(l->getSpriteBatch());
				}

				if (currItem.name == "properties")
				{
					showLayerProperties = true;
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

	if (showLayerProperties) this->layerProperties(&showLayerProperties);
}

void LayerBar::addIcons()
{
	// populate the toolbar with icons in the same order you want them to appear
	textureAtlasItem taItem;
	
	taItem = m_textureAtlas->getItem("layers");
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("previous");
	taItem.hint = "Go to previous layer";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("next");
	taItem.hint = "Go to next layer";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("properties");
	taItem.hint = "Edit current layer properties";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("texturepacker");
	taItem.hint = "Add Texture Packer Layer";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("shoebox");
	taItem.hint = "Add Shoebox Layer";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("delete");
	taItem.hint = "Delete a layer from the level";
	icons.push_back(taItem);


}

void LayerBar::layerProperties(bool *opened)
{
	/*
	if (opened)
	{
		char layerName[128];
		int i = 0;
		vector<Layer*>::iterator it;

		vector<Layer*> layers = ServiceLocator::getLevelService()->getLayers();

		LevelProvider *lvlProvider = ServiceLocator::getLevelService();

		if (!ImGui::Begin("Layers Properties", opened, ImVec2(0, 0), 0.90f))
		{
			ImGui::End();
			return;
		}

		ImGui::PushItemWidth(150);

		if (ImGui::TreeNode("Game"))
		{
			if (ImGui::TreeNode("Level"))
			{
				ImGui::Checkbox("Show Background", &ServiceLocator::getLevelService()->m_backgroundVisible);

				for (it = layers.begin(); it != layers.end(); it++)
				{
					i++;

					strcpy(layerName, (*it)->name.c_str());

					if (ImGui::TreeNode((void*)(intptr_t)i, "[%02d]-[%s]", (*it)->id,layerName))
					{
						if (ImGui::InputText("Layer Name", layerName, 128))
						{
							(*it)->name = layerName;
						}

						ImGui::DragFloat("Horizontal Speed", &(*it)->horizSpeed, 0.01);

						ImGui::DragFloat("Vertical Speed", &(*it)->vertSpeed, 0.001);

						ImGui::DragFloat("Depth", &(*it)->zDepth, 1.0);

						ImGui::Checkbox("Active", &(*it)->m_active);

						ImGui::LabelText("Layer ID", "%d", (*it)->id);

						if (ImGui::Button("Move Up"))
						{

						}

						ImGui::SameLine();

						if (ImGui::Button("Move Down"))
						{

						}


						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::PopItemWidth();
		ImGui::End();
	}
	*/
}