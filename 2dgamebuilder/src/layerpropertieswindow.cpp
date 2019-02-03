#include "layerpropertieswindow.h"
#include <imgui/imgui.h>
#include "servicelocator.h"

LayerPropertiesWindow::LayerPropertiesWindow(TextureAtlas *textureAtlas, string name) :ImGUIComponent(textureAtlas, name)
{
	addIcons();
}

LayerPropertiesWindow::~LayerPropertiesWindow()
{

}

void LayerPropertiesWindow::addIcons()
{

}

void LayerPropertiesWindow::run(bool* opened)
{
	char layerName[64];

	strcpy(layerName, ServiceLocator::getGameService()->getCurrentLayer()->name.c_str());

	if (!ImGui::Begin("Layer Properties", opened, ImVec2(0, 0), 0.90f, ImGuiWindowFlags_NoScrollbar)) 
	{
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(150);

	if (ImGui::InputText("Layer Name", layerName, 64))
	{
		ServiceLocator::getGameService()->getCurrentLayer()->name = layerName;
	}
	
	ImGui::DragFloat("Horizontal Speed", &ServiceLocator::getGameService()->getCurrentLayer()->horizSpeed, 0.01);

	ImGui::DragFloat("Vertical Speed", &ServiceLocator::getGameService()->getCurrentLayer()->vertSpeed, 0.01);

	ImGui::DragFloat("Depth Level", &ServiceLocator::getGameService()->getCurrentLayer()->zDepth, 1.0);

	ImGui::Checkbox("Active", &ServiceLocator::getGameService()->getCurrentLayer()->m_active);

	ImGui::PopItemWidth();

	ImGui::End();

}