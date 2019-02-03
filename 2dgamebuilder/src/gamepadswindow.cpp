#include "gamepadswindow.h"
#include <imgui/imgui.h>
#include "servicelocator.h"

GamePadsWindow::GamePadsWindow(TextureAtlas *textureAtlas, string name) :ImGUIComponent(textureAtlas, name)
{
	addIcons();
}

GamePadsWindow::~GamePadsWindow()
{

}

void GamePadsWindow::addIcons()
{

}

void GamePadsWindow::run(bool* opened)
{

}