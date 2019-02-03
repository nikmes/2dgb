/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file xmlconfigurationprovider.cpp
*
*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>
#include <time.h>

#include "servicelocator.h"
#include "filelogprovider.h"

/**
* Parameterized constructor to log in file
*
*/

FileLogProvider::FileLogProvider(string fileName)
{
	this->m_active = false;

	this->traceFlags = 0;

	this->setFileName(fileName);

	this->setActive(true);

	this->setTraceFlag(T_INF);

	this->setTraceFlag(T_DEB);

	this->setTraceFlag(T_WAR);

	this->setTraceFlag(T_ERR);

}

/**
* Default constructor
*
*/

FileLogProvider::FileLogProvider(string fileName, string section)
{
	this->m_active = false;

	this->traceFlags = 0;

	this->setFileName(fileName);

	this->setActive(true);

	this->setTraceFlag(T_INF);

	this->setTraceFlag(T_DEB);

	this->setTraceFlag(T_WAR);

	this->setTraceFlag(T_ERR);

	this->configure(section);
}

/**
* Initialize the log manager
*
* @return Void
*/

void FileLogProvider::configure(string section)
{
	/*
	* request log manager configuration from configuration manager
	*/

	XMLConfigurationProvider *confProvider = ServiceLocator::getConfigurationService();

	if (confProvider->pLog_loginformationmsgs)
	{
		this->setTraceFlag(T_INF);
	}
	else
	{
		this->clearTraceFlag(T_INF);
	}

	if (confProvider->pLog_logdebugingmsgs)
	{
		this->setTraceFlag(T_DEB);
	}
	else
	{
		this->clearTraceFlag(T_DEB);
	}

	if (confProvider->pLog_logwarningmsgs)
	{
		this->setTraceFlag(T_WAR);
	}
	else
	{
		this->clearTraceFlag(T_WAR);
	}

	if (confProvider->pLog_logerrormsgs)
	{
		this->setTraceFlag(T_ERR);
	}
	else
	{
		this->clearTraceFlag(T_ERR);
	}


	if (confProvider->pLog_enabled)
	{
		this->setActive(true);
	}
}

/**
* Logs a message in the m_logFileName file
*
* @param line		- An argument of type integer that represents the number of line in source file that LogMsg was called
* @param *file		- An argument of type character pointer that represetns the name of the source file calling LogMsg
* @param *func		- An argument of type character pointer that represents the name of function/method calling LogMsg
* @param flag		- An atgument of type int that indicates the type of trace (INFORMATION/WARNING/DEBUG/ERROR)
* @param *messaage	- An argument of type char pointer holds the incomming message to log
* @param ...	    - Am argument of type infinite arguments holds any variables that we want to substitue in *message
*
* @return Void
*/

void FileLogProvider::logMsg(int line, char *file, char *func, int flag, const char *message, ...)
{
	XMLConfigurationProvider *confProvider = ServiceLocator::getConfigurationService();

	int written = 0; // bytes written to file

	if (this->m_active)
	{
		if (traceFlags & (1 << flag))
		{
			va_list argList;

			char cbuffer[1024];

			// based on flags trace specific information

			if (confProvider->pLog_logtime || confProvider->pLog_logdate)
			{
				written += sprintf(cbuffer + written, "%s ", this->getCurrentDateTime().c_str());
			}

			/*
			* Check the kind of trace we will log
			*/

			switch (flag)
			{
			case T_INF:
				written += sprintf(cbuffer + written, "[INF] ");
				break;

			case T_ERR:
				written += sprintf(cbuffer + written, "[ERR] ");
				break;

			case T_WAR:
				written += sprintf(cbuffer + written, "[WAR] ");
				break;

			case T_DEB:
				written += sprintf(cbuffer + written, "[DEB] ");
				break;

			default:
				written += sprintf(cbuffer + written, "[INF] ");
				break;
			}

			if (confProvider->pLog_logfile)
			{
				written += sprintf(cbuffer + written, "[%s] ", file);
			}

			if (confProvider->pLog_logline)
			{
				written += sprintf(cbuffer + written, "[%04d] ", line);
			}

			if (confProvider->pLog_logfunc)
			{
				written += sprintf(cbuffer + written, "[% 48s] ", func);
			}

			va_start(argList, message);
			
			written += vsnprintf(cbuffer + written, sizeof(cbuffer) - written, message, argList);

			va_end(argList);

			sprintf(cbuffer + written, "\n");

			/*
			* Log to file is activated so log into file
			*/

			if (confProvider->pLog_logtofile)
			{
				if (m_file)
				{
					written = fprintf(m_file, cbuffer, strlen(cbuffer));

					fflush(m_file);
				}
			}
			if (ServiceLocator::getImguiService())
			{
				ServiceLocator::getImguiService()->loggerWindow->addLog(cbuffer);
			}
		}
	}
}

/**
* Sets the filename where the log file will be stored
*
* @param filename - An argument of type string that holds the file name of the log file that will be created
*
* @return Void
*/

void FileLogProvider::setFileName(string fileName)
{
	m_logFileName = fileName;

	if (this->m_active)
	{
		setActive(false);

		setActive(true);
	}

}

/**
* Sets the filename where the log file will be stored
*
* @param on - An argument of type bool that enables or disable the log manager
*
* @return Void
*/

void FileLogProvider::setActive(bool on)
{
	m_active = on;

	if (m_active)
	{
		string fullPath = "";
		fullPath.append(BASE_PATH);
		fullPath.append(m_logFileName);

		m_file = fopen(fullPath.c_str(), "w");

		if (m_file == NULL)
		{
			this->setActive(false);
		}
	}
	else
	{
		if (m_file != NULL)
		{
			fclose(m_file);
		}
	}
}

/**
* Force FileLogProvider to stop. The log file is properl closed 
*
* @return Void
*
*/

void FileLogProvider::destroy()
{
	this->setActive(false);
}

/**
* Enables specific trace flag
*
* @param flag - An integer argument that indicates the trace flag we want to enable
*
* @return Void
*
*/

void FileLogProvider::setTraceFlag(int flag)
{
	traceFlags |= 1 << flag;
}

/**
* Disable specific trace flag
*
* @param flag - An integer argument that indicates the trace flag that we want to disable
*
* @return Void
*/

void FileLogProvider::clearTraceFlag(int flag)
{
	traceFlags &= ~(1 << flag);
}

/**
* Checks if a specific trace flag is set or not 
*
* @param flag - An integer argument that indicates the trace flag that we want to check
*
* @return Bool
*/

bool FileLogProvider::checkTraceFlag(int flag)
{
	if (traceFlags & (1 << (flag)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
* gets current date and time of the system
*
* @return string - A string containing the current date and time
*/

string FileLogProvider::getCurrentDateTime()
{
	XMLConfigurationProvider *confProvider = ServiceLocator::getConfigurationService();

	time_t now = time(0);
	struct tm  tstruct;
	char buf[80];

	tstruct = *localtime(&now);

	if (confProvider->pLog_logtime && confProvider->pLog_logdate)
	{
		strftime(buf, sizeof(buf), "[%Y-%m-%d %X]", &tstruct);
	}
	else if (confProvider->pLog_logtime)
	{
		strftime(buf, sizeof(buf), "[%X]", &tstruct);
	}
	else if (confProvider->pLog_logdate)
	{
		strftime(buf, sizeof(buf), "[%Y-%m-%d]", &tstruct);
	}

	return buf;
}

