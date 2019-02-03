/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file xmlconfigurationprovider.h
*
*/
#ifndef XMLCONFIGURATIONPROVIDER_H
#define XMLCONFIGURATIONPROVIDER_H

#include <tinyxml/tinyxml.h>
#include "definitions.h"
#include "parameters.h"
#include <imgui/imgui.h>

using namespace std;

class XMLConfigurationProvider
{
private:

	TiXmlDocument m_doc; //!< Tje xml document in memmory that holds parameters and values

	string m_fileName; //!< The file name of the xml file that has the application configuration

	void loadConfigFile(string fileName); // loads xml configuration file

public:

	parameters param;

	XMLConfigurationProvider(string filename); // default constructor


	void saveConfigFile(); // saves xml configuration file

	string getParameter(string section, string parName); // gets the value of parameter from specific section
	bool getParameterBool(string section, string parName);
	float getParameterFloat(string section, string parName);
	int getParameterInt(string section, string parName);

	void setParameter(string section, string parName, string parValue); // sets the value of parameter of specific section		
	void setParameterBool(string section, string parName, bool parValue);

	void initDefaultParameters(); // initialize all parameters with default values

	void loadParametersInMemory(); // load all parameters from xml to params structure

	void destroy();

	void switchHints();

	// define all variables as public so they accesible everywere

	// log 
	string pLog_logfilename;
	bool pLog_logtofile;
	bool pLog_enabled;
	bool pLog_loginformationmsgs;
	bool pLog_logdebugingmsgs;
	bool pLog_logwarningmsgs;
	bool pLog_logerrormsgs;
	bool pLog_logproblemmsgs;
	bool pLog_logtime;
	bool pLog_logline;
	bool pLog_logfile;
	bool pLog_logfunc;
	bool pLog_logdate;
	int  pLog_alpha;
	ImVec4 pLog_color;
	// video 
	point pVideo_resolution;
	bool pVideo_fullscreen;

	// OpenGL
	int  pOpenGlVsync;

	// editor
	ImVec4 pEditor_color;
	bool pEditor_drawdebuglines;
	bool pEditor_drawbox2ddebuglines;
	bool pEditor_drawwireframe;        
	bool pEditor_drawpspbox;
	bool pEditor_hints;                
	bool pEditor_fps;     
	//DRAWINGMODE pEditor_draingmode = FREEHAND;

};

#endif