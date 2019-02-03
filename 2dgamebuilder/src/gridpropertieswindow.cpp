#include <imgui/imgui.h>
#include "gridpropertieswindow.h"
#include "servicelocator.h"

GridPropertiesWindow::GridPropertiesWindow(TextureAtlas *textureAtlas, string name) : ImGUIComponent(textureAtlas, name)
{
	this->addIcons();
}

GridPropertiesWindow::~GridPropertiesWindow()
{

}

void GridPropertiesWindow::addIcons()
{

}

void GridPropertiesWindow::run(bool* opened)
{
	int width, height;

	ServiceLocator::getRenderService()->getWindowSize(&width, &height);
	static float itemRotation = 0.0f;
	static float itemScale = 1.0f;

	//static float gridRotation = 0.0f;
	//static int gridOriginX = width / 2;
	//static int gridOriginY = height / 2;

	if (!ImGui::Begin("Grid Properties", opened, ImVec2(0, 0), 0.90f, ImGuiWindowFlags_NoScrollbar)) //, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
	{
	ImGui::End();
	return;
	}

	//ImGui::DragInt("Grid Origin X", &gridOriginX, 1.0);	// set grid origin x

	//ImGui::DragInt("Grid Origin Y", &gridOriginY, 1.0);	// set grid origin y
	ImGui::PushItemWidth(100);

	ImGui::DragFloat("Grid Origin X", &ServiceLocator::getGridService()->m_rotationOrigin.x, 1.0);	// set grid origin x
	ImGui::DragFloat("Grid Origin Y", &ServiceLocator::getGridService()->m_rotationOrigin.y, 1.0);	// set grid origin x


	//ServiceLocator::getGridService()->setGridRotationOrigin(gridOriginX, gridOriginY);

	ImGui::DragFloat("Grid Rotation", &ServiceLocator::getGridService()->m_rotation, .01);	// set grid rotation
	//ImGui::DragFloat("Grid Rotation", &gridRotation, .01);	// set grid rotation
	//	ServiceLocator::getGridService()->setGridRotation(gridRotation);

	//ImGui::DragFloat("Item Rotation", &itemRotation, .01); //&ServiceLocator::getGridService()->m_item->rotation, .01);	// set item rotation
	ImGui::DragFloat("Item Rotation", &ServiceLocator::getGridService()->m_item->rotation, .01);	// set item rotation
														   //ImGui::DragFloat("Item Rotation", &itemRotation, .01);	// set item rotation
	//ServiceLocator::getGridService()->setGridItemRotation(itemRotation);

	ImGui::DragFloat("Item Scale", &ServiceLocator::getGridService()->m_item->scale, .01,0.1,50);
	ServiceLocator::getGridService()->m_item->setScale(ServiceLocator::getGridService()->m_item->scale);
	//ImGui::DragFloat("Item Scale", &itemScale, .01);
	//ServiceLocator::getGridService()->setGridItemScale(itemScale);

	ImGui::Text("Scaled Width : %f", ServiceLocator::getGridService()->m_item->sW);
	ImGui::Text("Scaled Height: %f", ServiceLocator::getGridService()->m_item->sH);
	ImGui::Text("X: %f", ServiceLocator::getGridService()->m_item->x);
	ImGui::Text("Y: %f", ServiceLocator::getGridService()->m_item->y);


	ImGui::PopItemWidth();
	ImGui::End();
	
}