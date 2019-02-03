#include "spritepropertieswindow.h"
#include <imgui/imgui.h>
#include "servicelocator.h"

SpritePropertiesWindow::SpritePropertiesWindow(TextureAtlas *textureAtlas, string name) :ImGUIComponent(textureAtlas, name)
{
	addIcons();
}

SpritePropertiesWindow::~SpritePropertiesWindow()
{

}

void SpritePropertiesWindow::addIcons()
{

}

void SpritePropertiesWindow::run(bool* opened)
{
	ImVec2 uv0;
	ImVec2 uv1;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	Sprite *currentSprite = ServiceLocator::getGridService()->getCurrentSprite();

	g2dTexture *tex = ServiceLocator::getTextureService()->GetTexture(ServiceLocator::getGameService()->getCurrentLayer()->getSpriteBatch()->m_textureFileName);

	uv0 = ImVec2(currentSprite->cX / (float)tex->w, currentSprite->cY / (float)tex->h);
	uv1 = ImVec2((currentSprite->cX + currentSprite->cW) / (float)tex->w, (currentSprite->cY + currentSprite->cH) / (float)tex->h);

	ImGui::Begin(this->m_name.c_str(), opened, ImVec2(0, 0), 1.0f, ImGuiWindowFlags_AlwaysAutoResize);
	{
		
	//	draw_list->PushTextureID((ImTextureID*)tex->id);
	//	draw_list->PrimReserve(6, 4);
//		draw_list->PrimQuadUV(a, b, c, d, uv_a, uv_b, uv_c, uv_d, col);
		//draw_list->PopTextureID();

		ImGui::Image((ImTextureID*)tex->id, ImVec2(currentSprite->sW, currentSprite->sH), uv0, uv1, ImColor(255, 255, 255, 255));

	}

	ImGui::End();

}