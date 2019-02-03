#ifndef BENCHMARKINGWINDOW_H
#define BENCHMARKINGWINDOW_H

#include "imguicomponent.h"

using namespace std;

class BenchmarkingWindow : public ImGUIComponent
{

private:

	TextureAtlas *m_textureAtlas;

public:

	BenchmarkingWindow(TextureAtlas *textureAtlas, string name);

	~BenchmarkingWindow();

	void run(bool* opened);

	void addIcons();
};

#endif
