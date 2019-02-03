#include "renderingstatswindow.h"
#include <imgui/imgui.h>
#include "servicelocator.h"

RenderingStatsWindow::RenderingStatsWindow(TextureAtlas *textureAtlas, string name) :ImGUIComponent(textureAtlas, name)
{
	addIcons();
}

RenderingStatsWindow::~RenderingStatsWindow()
{

}

void RenderingStatsWindow::addIcons()
{

}

void RenderingStatsWindow::run(bool* opened)
{
	ImVec4 imGreen(0.0f, 1.0f, 0.0f, 1.0f);

	if (!ImGui::Begin("FPS Overlay", opened, ImVec2(0, 0), 50.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::End();
		return;
	}

	ImGui::TextColored(imGreen, "%4.3f MSPF %4.0f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Separator();

	ImGui::TextColored(imGreen, "Mouse Position: %6.1f,%6.1f", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);

	ImGui::Separator();

	ImGui::TextColored(imGreen, "Rendered Sprites: %d/%d", totalRenderedSprites, ServiceLocator::getGameService()->getCurrentLevel()->getTotalSprites());

	ImGui::TextColored(imGreen, "Delta Time %f", ImGui::GetIO().DeltaTime);

	ImGui::TextColored(imGreen, "Camera Pos [%.0fx%.0f]", camX, camY);

	ImGui::TextColored(imGreen, "Total Sprites [%d]", ServiceLocator::getRenderService()->maxRectangles);

	ImGui::TextColored(imGreen, "CPU2GPU Floats/Second [%d]", ServiceLocator::getRenderService()->maxVertices);

	ImGui::Separator();

	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse)
	{
		ImGui::TextColored(imGreen, "Mouse in IMGUI window");
	}
	else
	{
		ImGui::TextColored(imGreen, "Mouse NOT in IMGUI window");
	}


	if (ServiceLocator::getGameService()->gameIsCreated())
	{
		ImGui::Separator();

		ImGui::TextColored(imGreen, "Current Level: %s", ServiceLocator::getGameService()->getCurrentLevel()->m_name.c_str());

		ImGui::Separator();

		ImGui::TextColored(imGreen, "Current Layer: %s", ServiceLocator::getGameService()->getCurrentLayer()->name.c_str());

		ImGui::Separator();

		ImGui::TextColored(imGreen, "Current SpriteBatch: %s", ServiceLocator::getGameService()->getCurrentLevel()->getCurrentSpriteBatch()->m_name.c_str());
	}

	ImGui::End();

}