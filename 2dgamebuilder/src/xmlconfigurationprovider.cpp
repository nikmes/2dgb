/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file xmlconfigurationprovider.cpp
*
*/

#include "servicelocator.h"
#include "xmlconfigurationprovider.h"

/**
* Default constructor 
*
*/

XMLConfigurationProvider::XMLConfigurationProvider(string fileName)
{
	string fullPath = "";
	fullPath.append("resources/");
	fullPath.append(fileName);

	TRACE(T_DEB, "XMLConfigurationProvider - Contructor");
	pVideo_resolution.x = 1024;
	pVideo_resolution.y = 720;
	pLog_logfilename = "";
	pLog_logfilename.append(BASE_PATH);
	pLog_logfilename.append("application.log");
	pLog_logtofile = true;
	pLog_enabled = true;
	pLog_loginformationmsgs = true;
	pLog_logdebugingmsgs = true;
	pLog_logwarningmsgs = true;
	pLog_logerrormsgs = true;
	pLog_logproblemmsgs = true;
	pLog_logtime = true;
	pLog_logline = true;
	pLog_logfile = true;
	pLog_logfunc = true;
	pLog_logdate = true;
	pLog_alpha = 0;
	pLog_color = { 0.0, 0.0, 0.0, 255.0 };

	// video 
	pVideo_fullscreen = false;
	// opengl
	pOpenGlVsync = 1;

	// editor
	pEditor_color = { 0.0, 0.0, 0.0, 255.0 };

	pEditor_drawdebuglines = true;
	pEditor_drawbox2ddebuglines = true;
	pEditor_drawwireframe = true;
	pEditor_drawpspbox = true;
	pEditor_hints = true;
	pEditor_fps = true;
	//DRAWINGMODE pEditor_draingmode = FREEHAND;
	this->loadConfigFile(fullPath);
	m_fileName = fullPath;

}

/**
* Loads the application configuration from a given xml file
*
* @param fileName - A string argument that holds the file name of the xml configuration file
*
* @return Void
*/

void XMLConfigurationProvider::loadConfigFile(string fileName)
{
	TRACE(T_DEB, "Load application configuration file [%s]", fileName.c_str());

	m_doc.Clear();

	if (!m_doc.LoadFile(fileName.c_str()))
	{
		TRACE(T_DEB, "Unable to load [%s] ", fileName.c_str());
		TRACE(T_DEB, "Creating parameters file [config.xml] using default values");
		exit(0);
		/*!!! todo: Do not exit just create the xml file with default parameter values !!!*/
	}

	m_fileName = fileName;

	this->loadParametersInMemory();
}

/**
* Load all parameters from xml doc that is in memmory to parameters 
*
* @return Void
*/

void XMLConfigurationProvider::loadParametersInMemory() 
{
	string paramValue;
	
	// load log related parameters
		
	this->pLog_logfilename = getParameter("log", "logfilename");
	this->pLog_logtofile = getParameterBool("log", "logtofile");
	this->pLog_enabled = getParameterBool("log", "enabled");
	this->pLog_loginformationmsgs = getParameterBool("log", "loginformationmsgs");
	this->pLog_logdebugingmsgs = getParameterBool("log", "logdebugingmsgs");
	this->pLog_logwarningmsgs = getParameterBool("log", "logwarningmsgs");
	this->pLog_logerrormsgs = getParameterBool("log", "logerrormsgs");
	this->pLog_logproblemmsgs = getParameterBool("log", "logproblemmsgs");
	this->pLog_logtime = getParameterBool("log", "logtime");
	this->pLog_logline = getParameterBool("log", "logline");
	this->pLog_logfile = getParameterBool("log", "logfile");
	this->pLog_logfunc = getParameterBool("log", "logfunc");
	this->pLog_logdate = getParameterBool("log", "logdate");

	paramValue = getParameter("log", "color");
	sscanf(paramValue.c_str(), "%fx%fx%fx%f", &this->pLog_color.x, &this->pLog_color.y, &this->pLog_color.z, &this->pLog_color.w);
	
	// load all video related parameters

	this->pVideo_fullscreen = getParameterBool("video", "fullscreen");

	paramValue = getParameter("video", "resolution");
	sscanf(paramValue.c_str(),"%fx%f", &this->pVideo_resolution.x, &this->pVideo_resolution.y);

	// load all editor related parameters

	paramValue = getParameter("editor", "color");
	sscanf(paramValue.c_str(), "%fx%fx%fx%f", &this->pEditor_color.x, &this->pEditor_color.y, &this->pEditor_color.z, &this->pEditor_color.w);

	this->pEditor_drawdebuglines = getParameterBool("editor", "drawdebuglines");
	this->pEditor_drawbox2ddebuglines = getParameterBool("editor", "drawbox2ddebuglines");
	this->pEditor_drawwireframe = getParameterBool("editor", "drawwireframe");
	this->pEditor_drawpspbox = getParameterBool("editor", "drawpspbox");
	this->pEditor_hints = getParameterBool("editor", "hints");
	this->pEditor_fps = getParameterBool("editor", "fps");

}

/**
* Initialize all parameters with default values
*
* @return Void
*/

void XMLConfigurationProvider::initDefaultParameters()
{

}

/**
* Saves the application configuration file
*
* @return Void
*/

void XMLConfigurationProvider::saveConfigFile()
{
	TRACE(T_DEB, "Save application configuration file [%s]", m_fileName.c_str());

	// before saving xml document to disk set values of all parameters as of now to xml doc in memmory
	string paramValue;
	char paramValueBuf[64];

	// save all log related parameters

	setParameter("log", "logfilename",this->pLog_logfilename);

	setParameterBool("log", "enabled", pLog_enabled);
	setParameterBool("log", "logdate", pLog_logdate);
	setParameterBool("log", "logdebugingmsgs", pLog_logdebugingmsgs);
	setParameterBool("log", "logwarningmsgs", pLog_logwarningmsgs);
	setParameterBool("log", "logerrormsgs", pLog_logerrormsgs);
	setParameterBool("log", "logfile", pLog_logfile);
	setParameterBool("log", "logfunc", pLog_logfunc);
	setParameterBool("log", "loginformationmsgs", pLog_loginformationmsgs);
	setParameterBool("log", "logline", pLog_logline);
	setParameterBool("log", "logproblemmsgs", pLog_logproblemmsgs);
	setParameterBool("log", "logtime", pLog_logtime);
	setParameterBool("log", "logtofile", pLog_logtofile);
	
	sprintf(paramValueBuf, "%fx%fx%fx%f", this->pLog_color.x, this->pLog_color.y, this->pLog_color.z, this->pLog_color.w);
	paramValue = paramValueBuf;
	setParameter("log", "color", paramValue);

	// save all video related parameters
	setParameterBool("video", "fullscreen", pVideo_fullscreen);

	sprintf(paramValueBuf, "%.0fx%.0f", this->pVideo_resolution.x, this->pVideo_resolution.y);
	paramValue = paramValueBuf;
	setParameter("video", "resolution", paramValue);


	// save all editor related parameters
	
	sprintf(paramValueBuf, "%fx%fx%fx%f", this->pEditor_color.x, this->pEditor_color.y, this->pEditor_color.z, this->pEditor_color.w);
	paramValue = paramValueBuf;
	setParameter("editor", "color", paramValue);

	setParameterBool("editor", "drawwireframe", pEditor_drawwireframe);
	setParameterBool("editor", "drawbox2ddebuglines", pEditor_drawbox2ddebuglines);
	setParameterBool("editor", "drawdebuglines", pEditor_drawdebuglines);
	setParameterBool("editor", "drawpspbox", pEditor_drawpspbox);
	setParameterBool("editor", "hints", pEditor_hints);
	setParameterBool("editor", "fps", pEditor_fps);

	// save the actual xml doc to disk
	m_doc.SaveFile(m_fileName.c_str());

}

/**
* Gets the value of a parameter from a specific section of the xml configuration file
*
* @param section   - A string argument that indicates the section name in the XML configuration file
* @param paramName - A string argument that indicates the parameter name for which we want to get its value
*
* @return string - The value of the parameter in string format or the string "NULL"
*/

string XMLConfigurationProvider::getParameter(string section, string parName)
{
	TRACE(T_DEB, "Attemp to retrieve parameter [%s] from section [%s]", parName.c_str(), section.c_str());

	// returns the string value of a parameter at section section with name parName

	TiXmlElement* l_section = m_doc.FirstChildElement("globalconfig")->FirstChildElement(section.c_str());

	if (l_section)
	{
		TiXmlElement* element = m_doc.FirstChildElement("globalconfig")->FirstChildElement(section.c_str())->FirstChildElement("param");

		if (element)
		{
			for (TiXmlElement* value = element; value != NULL; element = element->NextSiblingElement())
			{
				string elemName = element->Value();

				string paramName = element->Attribute("name");

				if ((!paramName.empty()) && (paramName == parName))
				{
					string val = element->Attribute("value");

					if (!val.empty())
					{
						TRACE(T_DEB, "Load parameter [%s] with value [%s]", paramName.c_str(), val.c_str());

						return val;
					}
				}
			}
		}
	}

	TRACE(T_WAR, "Fail to find parameter [%s] in section [%s] ", parName.c_str(), section.c_str());

	return "NULL";
}

bool XMLConfigurationProvider::getParameterBool(string section, string parName)
{
	TRACE(T_DEB, "Attemp to retrieve parameter [%s] from section [%s]", parName.c_str(), section.c_str());

	// returns the string value of a parameter at section section with name parName

	TiXmlElement* l_section = m_doc.FirstChildElement("globalconfig")->FirstChildElement(section.c_str());

	if (l_section)
	{
		TiXmlElement* element = m_doc.FirstChildElement("globalconfig")->FirstChildElement(section.c_str())->FirstChildElement("param");

		if (element)
		{
			for (TiXmlElement* value = element; value != NULL; element = element->NextSiblingElement())
			{
				string elemName = element->Value();

				string paramName = element->Attribute("name");

				if ((!paramName.empty()) && (paramName == parName))
				{
					string val = element->Attribute("value");

					if (!val.empty())
					{
						TRACE(T_DEB, "Load parameter [%s] with value [%s]", paramName.c_str(), val.c_str());

						// convert to bool before returning
						if (val == "Yes" || val == "yes")
						{
							return true;
						}
						else
						{
							return false;
						}
					}
				}
			}
		}
	}

	TRACE(T_WAR, "Fail to find parameter [%s] in section [%s] ", parName.c_str(), section.c_str());

	return false;
}

int XMLConfigurationProvider::getParameterInt(string section, string parName)
{
	TRACE(T_DEB, "Attemp to retrieve parameter [%s] from section [%s]", parName.c_str(), section.c_str());

	// returns the string value of a parameter at section section with name parName

	TiXmlElement* l_section = m_doc.FirstChildElement("globalconfig")->FirstChildElement(section.c_str());

	if (l_section)
	{
		TiXmlElement* element = m_doc.FirstChildElement("globalconfig")->FirstChildElement(section.c_str())->FirstChildElement("param");

		if (element)
		{
			for (TiXmlElement* value = element; value != NULL; element = element->NextSiblingElement())
			{
				string elemName = element->Value();

				string paramName = element->Attribute("name");

				if ((!paramName.empty()) && (paramName == parName))
				{
					string val = element->Attribute("value");

					if (!val.empty())
					{
						TRACE(T_DEB, "Load parameter [%s] with value [%s]", paramName.c_str(), val.c_str());
						//convert to int before returning
						return atoi(val.c_str());
					}
				}
			}
		}
	}

	TRACE(T_WAR, "Fail to find parameter [%s] in section [%s] ", parName.c_str(), section.c_str());

	return 0;
}

float XMLConfigurationProvider::getParameterFloat(string section, string parName)
{
	TRACE(T_DEB, "Attemp to retrieve parameter [%s] from section [%s]", parName.c_str(), section.c_str());

	// returns the string value of a parameter at section section with name parName

	TiXmlElement* l_section = m_doc.FirstChildElement("globalconfig")->FirstChildElement(section.c_str());

	if (l_section)
	{
		TiXmlElement* element = m_doc.FirstChildElement("globalconfig")->FirstChildElement(section.c_str())->FirstChildElement("param");

		if (element)
		{
			for (TiXmlElement* value = element; value != NULL; element = element->NextSiblingElement())
			{
				string elemName = element->Value();

				string paramName = element->Attribute("name");

				if ((!paramName.empty()) && (paramName == parName))
				{
					string val = element->Attribute("value");

					if (!val.empty())
					{
						TRACE(T_DEB, "Load parameter [%s] with value [%s]", paramName.c_str(), val.c_str());
						//convert to float before return
						return stof(val.c_str());
					}
				}
			}
		}
	}

	TRACE(T_WAR, "Fail to find parameter [%s] in section [%s] ", parName.c_str(), section.c_str());

	return 0.0;
}

/**
* Sets the value of a parameter from a specific section of the xml configuration file
*
* @param section   - A string argument that indicates the section name in the XML configuration file
* @param paramName - A string argument that indicates the parameter name for which we want to set its value
*
* @return void
*/

void XMLConfigurationProvider::setParameter(string section, string parName, string parValue)
{
	TiXmlElement* sectionElement = m_doc.FirstChildElement("globalconfig")->FirstChildElement(section.c_str());

	if (sectionElement)
	{
		sectionElement = m_doc.FirstChildElement("globalconfig")->FirstChildElement(section.c_str())->FirstChildElement("param");

		if (sectionElement)
		{
			for (TiXmlElement* elem = sectionElement; elem != NULL; elem = elem->NextSiblingElement())
			{
				string elemName = elem->Value();

				string paramName = elem->Attribute("name");

				if ((!paramName.empty()) && (paramName == parName))
				{
					elem->SetAttribute("value", parValue.c_str());

					string value = elem->Attribute("value");

					if (!value.empty())
					{
						TRACE(T_DEB, "Save Parameter [%s][%s][%s]", section.c_str(),paramName.c_str(), value.c_str());
					}
					else
					{
						TRACE(T_WAR, "Parameter [%s] has empty value!", paramName.c_str());
					}				
				}
			}
		}
	}
}

void XMLConfigurationProvider::setParameterBool(string section, string parName, bool parValue)
{
	string pValue;

	TiXmlElement* sectionElement = m_doc.FirstChildElement("globalconfig")->FirstChildElement(section.c_str());

	if (sectionElement)
	{
		sectionElement = m_doc.FirstChildElement("globalconfig")->FirstChildElement(section.c_str())->FirstChildElement("param");

		if (sectionElement)
		{
			for (TiXmlElement* elem = sectionElement; elem != NULL; elem = elem->NextSiblingElement())
			{
				string elemName = elem->Value();

				string paramName = elem->Attribute("name");

				if ((!paramName.empty()) && (paramName == parName))
				{
					if (parValue == true)
					{
						pValue = "Yes";
					}
					else
					{
						pValue = "No";
					}

					elem->SetAttribute("value", pValue.c_str());

					string value = elem->Attribute("value");

					if (!value.empty())
					{
						TRACE(T_DEB, "Save Parameter [%s][%s][%s]", section.c_str(), paramName.c_str(), value.c_str());
					}
					else
					{
						TRACE(T_WAR, "Parameter [%s] has empty value!", paramName.c_str());
					}
				}
			}
		}
	}
}


void XMLConfigurationProvider::destroy()
{
	this->saveConfigFile();
}

void XMLConfigurationProvider::switchHints()
{
	if (this->pEditor_hints)
	{
		pEditor_hints = false;
	}
	else
	{
		pEditor_hints = true;
	}
}