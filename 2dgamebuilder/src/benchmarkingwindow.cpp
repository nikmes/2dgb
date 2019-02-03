#include "benchmarkingwindow.h"
#include <imgui/imgui.h>
#include "servicelocator.h"

BenchmarkingWindow::BenchmarkingWindow(TextureAtlas *textureAtlas, string name) :ImGUIComponent(textureAtlas, name)
{
	addIcons();
}

BenchmarkingWindow::~BenchmarkingWindow()
{

}

void BenchmarkingWindow::addIcons()
{

}

void BenchmarkingWindow::run(bool* opened)
{

}