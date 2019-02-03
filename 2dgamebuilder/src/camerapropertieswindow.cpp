#include "camerapropertieswindow.h"
#include <imgui/imgui.h>
#include "servicelocator.h"

CameraPropertiesWindow::CameraPropertiesWindow(TextureAtlas *textureAtlas, string name) :ImGUIComponent(textureAtlas, name)
{
	addIcons();
}

CameraPropertiesWindow::~CameraPropertiesWindow()
{

}

void CameraPropertiesWindow::addIcons()
{

}

void CameraPropertiesWindow::run(bool* opened)
{
	int width, height;

	ServiceLocator::getRenderService()->getWindowSize(&width, &height);

	if (!ImGui::Begin("Camera Properties", opened, ImVec2(0, 0), 0.90f))
	{
		ImGui::End();
		return;
	}
	ImGui::PushItemWidth(150);

	ImGui::Checkbox("Active", &ServiceLocator::getCameraService()->active);
	ImGui::DragFloat("Step X", &ServiceLocator::getCameraService()->CAM_STEP_X, 1.0);	
	ImGui::DragFloat("Step Y", &ServiceLocator::getCameraService()->CAM_STEP_Y, 1.0);	
	
	ImGui::DragFloat("Position Speed", &ServiceLocator::getCameraService()->CAM_POS_SPEED, .001);
	ImGui::DragFloat("Rotation Speed", &ServiceLocator::getCameraService()->CAM_ROT_SPEED, .001);
	ImGui::DragFloat("Zoom Speed", &ServiceLocator::getCameraService()->CAM_ZOOM_SPEED, .0001);

	ImGui::DragFloat("X Target", &ServiceLocator::getCameraService()->x_target, 10.0);
	camX = ServiceLocator::getCameraService()->x_target;
	
	ImGui::DragFloat("Y Target", &ServiceLocator::getCameraService()->y_target, 10.0);
	camY = ServiceLocator::getCameraService()->y_target;
	
	ImGui::DragFloat("X ", &ServiceLocator::getCameraService()->x, 1.0);
	ImGui::DragFloat("Y ", &ServiceLocator::getCameraService()->y, 1.0);
	ImGui::DragFloat("Zoom Target", &ServiceLocator::getCameraService()->zoom_target,0.1);
	ImGui::DragFloat("Rotation Target", &ServiceLocator::getCameraService()->rot_target, 0.1);

	ImGui::PopItemWidth();

	ImGui::End();

}