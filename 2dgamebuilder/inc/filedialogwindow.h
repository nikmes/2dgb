/**
* \date    14/02/2015
* \author  Nicholas Messaritis
*
* \file    loggerwindow.h
*
* Implementation of the logger window
*
*/

#ifndef FILEDIALOGWINDOW_H
#define FILEDIALOGWINDOW_H

#include "imguicomponent.h"

class FileDialogWindow : public ImGUIComponent
{
private:

	void getDriveLetters();
	void refreshDriveLetters();
	bool showModalWindow(string name, string caption);

	FileType fileType = FileType::FT_PNG;
	bool scrollToBottom;

	char m_selectedFile[128];
	vector<std::string> driveList;
	char driveLetters[128];

public:

	FileDialogWindow(TextureAtlas *textureAtlas, string name);

	~FileDialogWindow();

	// Main loop and additiona windows

	void run(bool* opened);

	void addIcons();

	void setFileType(FileType ft);


	std::string runPNGSelect();

	std::string runTAtlasSelect();

	std::string runDirSelect();

	std::string runSelectXmlFileName(char *dialoguName);


};

#endif
