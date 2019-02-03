/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file filelogprovider.h
*
*/

#ifndef FILELOGPROVIDER_H
#define FILELOGPROVIDER_H

#include<string>

using namespace std;

#define T_INF 0 // Trace is for information
#define T_WAR 1 // Trace is a warning
#define T_PRO 2 // Trace is a problem
#define T_ERR 3 // Trace is a critical error
#define T_DEB 4 // Trace is for debugging purposes

class FileLogProvider 
{
private:

	string m_logFileName; //!< The file name where we should log messages

	FILE *m_file; //!< The actual file pointer

	void setActive(bool on); // activate or deactivates the logger

public:

	unsigned int traceFlags; //!< The trace flags enabled or disabled packed in an int

	bool m_active; //!< Flag that indicates if the logger is active or not

	FileLogProvider(string fileName); // default constructor

	FileLogProvider(string fileName, string section);

	void configure(string section); // initialize the log manager

	void setFileName(string fileName); // sets the filename of the logfile

	void setTraceFlag(int flag); // set specific trace flag to true
	

	void clearTraceFlag(int flag); // switch off trace flag

	bool checkTraceFlag(int flag); // checks if a trace flag is set or not


	void logMsg(int line, char *file, char *func, int flag, const char *message, ...); // logs message in log file

	string getCurrentDateTime(); // returns current time formated in string

	void destroy(); // terminate all logging activities and close the log file

};

#endif