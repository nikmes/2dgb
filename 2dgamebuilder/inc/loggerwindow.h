/**
* \date    14/02/2015
* \author  Nicholas Messaritis
*
* \file    loggerwindow.h
*
* Implementation of the logger window
*
*/

#ifndef LOGGERWINDOW_H
#define LOGGERWINDOW_H

#include "imguicomponent.h"
#include "textureatlas.h"

class LoggerWindow : public ImGUIComponent
{
private:
	
	ImVector<char*> items;
	bool scrollToBottom;

public:

	LoggerWindow(TextureAtlas *textureAtlas, string name);
	
	~LoggerWindow();
	
	// Operations on logger window

	void addLog(const char* fmt, ...);

	void clearLog();

	// Main loop and additiona windows

	void run(bool* opened);

	void showOptions(bool *opened);

	void addIcons();

};

#endif
