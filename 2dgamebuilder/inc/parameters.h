#ifndef PARAMETERS_H
#define PARAMETERS_H

typedef struct
{
	string logfilename; 
	bool logtofile; 
	bool enabled;
	bool loginformationmsgs;
	bool logdebugingmsgs;
	bool logwarningmsgs;
	bool logerrormsgs;
	bool logproblemmsgs;
	bool logtime;
	bool logline;
	bool logfile;
	bool logfunc;
	bool logdate;

} loggerParams;

typedef struct
{
	point resolution;
	bool fullscreen;
} videoParams;

typedef struct
{
	bool wireframe;
	bool drawdebuglines;
	bool drawbox2ddebuglines;
	bool drawwireframe;
	bool drawpspbox;
	bool hints;
	bool fps;
} editorParams;

typedef struct
{
	loggerParams logger;
	videoParams video;
	editorParams editor;
} parameters;

#endif