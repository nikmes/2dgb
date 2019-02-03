#include "box2dworldwindow.h"
#include <imgui/imgui.h>
#include "servicelocator.h"

Box2dWorldWindow::Box2dWorldWindow(TextureAtlas *textureAtlas, string name) :ImGUIComponent(textureAtlas, name)
{
	addIcons();
}

Box2dWorldWindow::~Box2dWorldWindow()
{

}

void Box2dWorldWindow::addIcons()
{

}

void Box2dWorldWindow::run(bool* opened)
{

}