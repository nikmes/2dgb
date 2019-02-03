#include "spriteselector.h"
#include <imgui/imgui.h>
#include "servicelocator.h"
#include <map>

SpriteSelector::SpriteSelector()
{
	TRACE(T_INF, "SpriteSelector initialized");

}

SpriteSelector::SpriteSelector(TextureAtlas *textureAtlas, string name) :ImGUIComponent(textureAtlas, name)
{
	m_spriteBatch = NULL; 

	this->addIcons();
}

SpriteSelector::~SpriteSelector()
{
}

Sprite SpriteSelector::getCurrentSprite()
{
	return m_selectedSprite;
}

SpriteBatch *SpriteSelector::getSpriteBatch()
{
	return m_spriteBatch;
}

void SpriteSelector::run(bool* opened)
{
	ImVec2 uv0;
	ImVec2 uv1;

	vector<textureAtlasItem>::iterator it;

	if (!ImGui::Begin(m_name.c_str(), opened, ImVec2(0, 0), 0.70f, ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
		return;
	}

	it = icons.begin();

	textureAtlasItem currItem = *it;

	uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
	uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);

	ImGui::Image(texId, ImVec2(20, 20), uv0, uv1, ImColor(255, 255, 255, 255));

	for (it = icons.begin() + 1; it != icons.end(); it++)
	{
		textureAtlasItem currItem = *it;

		uv0 = ImVec2(currItem.x / (float)tex->w, currItem.y / (float)tex->h);
		uv1 = ImVec2((currItem.x + currItem.w) / (float)tex->w, (currItem.y + currItem.h) / (float)tex->h);

		ImGui::PushID(currItem.id);

		ImGui::SameLine();

		if (ImGui::ImageButton(texId, ImVec2(40, 40), uv0, uv1, 0, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255)))
		{
			/*
			if (currItem.name == "next")
			{
				this->setSpriteBatch(ServiceLocator::getGameService()->getCurrentLevel()->getNextSpriteBatch());
				TRACE(T_INF, "Set spriteselector->spritebatch to point at next layer spritebatch [%s]", this->m_spriteBatch->m_name.c_str());
			}

			if (currItem.name == "previous")
			{
				this->setSpriteBatch(ServiceLocator::getGameService()->getCurrentLevel()->getPreviousSpriteBatch());
				TRACE(T_INF, "Set spriteselector->spritebatch to point at previous layer spritebatch [%s]", this->m_spriteBatch->m_name.c_str());
			}
			*/
			if (currItem.name == "properties")
			{
				TRACE(T_INF, "Bring up layer properties window");
				ServiceLocator::getImguiService()->layerPropertiesWindow->switchVisibility();
			}

			if (currItem.name == "preview")
			{
				TRACE(T_INF, "Enable preview of current sprite");
				ServiceLocator::getImguiService()->layerPropertiesWindow->switchVisibility();
			}

			if (currItem.name == "texturepacker")
			{
				TRACE(T_INF, "Add a texturepacker layer to the level");
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

	ImGui::Separator();

	ImGui::BeginChild("SpriteBatchScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	// 1. Check if game is loaded
	// 2. Get all SpriteBatches of Level
	// 3. Create a tree
	// 4. Add Parent Node as "Level Objects"
	// 5. Add Child Node as "All"
	// 6. Add children Nodes to All of each and every sprite from SpriteBatches
	
	vector<SpriteBatch*> *sprBatches = ServiceLocator::getGameService()->getCurrentLevel()->getSpriteBatches();

	vector<SpriteBatch*>::iterator sprIt;

	int curSprBatch = 0;

	for (sprIt = sprBatches->begin(); sprIt != sprBatches->end(); sprIt++)
	{
		g2dTexture* sbTex = ServiceLocator::getTextureService()->GetTexture((*sprIt)->m_textureFileName);

		ImTextureID texSprId = (ImTextureID*)sbTex->id;

		map<string, Sprite> *allSprites = (*sprIt)->getAllSprites();

		map<string, Sprite>::iterator itp;

		for (itp = allSprites->begin(); itp != allSprites->end(); itp++)
		{

			uv0 = ImVec2(itp->second.cX / (float)sbTex->w, itp->second.cY / (float)sbTex->h);
			uv1 = ImVec2((itp->second.cX + itp->second.cW) / (float)sbTex->w, (itp->second.cY + itp->second.cH) / (float)sbTex->h);

			ImGui::PushID(itp->second.id);

			if (ImGui::ImageButton(texSprId, ImVec2(64, 64), uv0, uv1, 0, ImColor(0, 0, 0, 0), ImColor(255, 255, 255, 255)))
			{
				ServiceLocator::getGameService()->getCurrentLevel()->m_currentSpriteBatch = curSprBatch;
				TRACE(T_INF, "The image selected is [%s]", itp->second.name.c_str());
				
				this->setSpriteBatch((*sprIt));

				m_selectedSprite = (*sprIt)->getSprite(itp->second.name.c_str());
				m_selectedSprite.sprBatch = (*sprIt);
				ServiceLocator::getGameService()->getCurrentSpriteBatch()->m_lastSelectedSprite = m_selectedSprite.name;

				// maintain scales rotation of what u were doing
				m_selectedSprite.scale = ServiceLocator::getGridService()->m_item->scale;
				m_selectedSprite.rotation = ServiceLocator::getGridService()->m_item->rotation;

				// set item based on which grid will be drawn
				ServiceLocator::getGridService()->setItem(&m_selectedSprite);
				//!!! show properties window disable for now since we can not rotate image in windw
				//ServiceLocator::getImguiService()->spritePropertiesWindow->switchVisibility();
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();

				ImGui::Text("%s", itp->second.name.c_str());

				ImGui::Text("%gx%g", itp->second.cW, itp->second.cH);

				ImGui::Image(texSprId, ImVec2(itp->second.cW, itp->second.cH), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

				ImGui::EndTooltip();
			}

			ImGui::SameLine();

			ImGui::PopID();
		}
		curSprBatch++;
	}
	ImGui::EndChild();
	ImGui::End();
}

void SpriteSelector::addIcons()
{
	// populate the toolbar with icons in the same order you want them to appear
	textureAtlasItem taItem;

	taItem = m_textureAtlas->getItem("layers");
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("previous");
	taItem.hint = "Select sprites from previous texture atlas";
	icons.push_back(taItem);

	taItem = m_textureAtlas->getItem("next");
	taItem.hint = "Select sprite from next texture atlas";
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

void SpriteSelector::setSpriteBatch(SpriteBatch *spriteBatch) 
{
	this->m_spriteBatch = spriteBatch;
	
	// check if is the first time we using this texture atlas and if yes then set default selected sprite the first one
	// if is not the first time then set default sprite the one last time used by user

	if (m_spriteBatch->m_lastSelectedSprite.empty())
	{
		m_spriteBatch->spriteMap.begin()->second.sprBatch = spriteBatch;
		ServiceLocator::getGridService()->setItem(&m_spriteBatch->spriteMap.begin()->second);
		TRACE(T_INF, "SpriteBatch first use. Currently selected sprite [%s]", this->m_selectedSprite.name.c_str());
	}
	else
	{
		m_spriteBatch->spriteMap.at(m_spriteBatch->m_lastSelectedSprite).sprBatch = spriteBatch;
		ServiceLocator::getGridService()->setItem(&m_spriteBatch->spriteMap.at(m_spriteBatch->m_lastSelectedSprite));
		TRACE(T_INF, "SpriteBatch was used again. Currently selected sprite [%s]", this->m_selectedSprite.name.c_str());
	}

	TRACE(T_INF, "setSpriteBatch END");
}

