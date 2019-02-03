/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file application.h
*
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
#include "servicelocator.h"

class CApplicationState;

class Application 
{
private:

	// PSP FPS Reporting
	char fps[100];
	int frames;	 // for calculating the frames per second
	uint32_t res;	// resolution
	uint64_t now;	 // the ticks at current point
	uint64_t previous; // the ticks at the previous point
	uint32_t ramAvail;
	float curr_ms;
	uint64_t last_tick;
	uint32_t tick_frequency;
	int frame_count;

	// PS3 FPS Reporting
	float FPS;
	float LastTime;
	float REPORT_TIME;
	int Frames;
	int FramesLastReport;
	double TimeElapsed;
	double TimeLastReport;
	double TimeReport;
	unsigned int Counter;		//frame counter

	bool VSYNC_ON;

	g2dColor m_clearColor;

public:
	
	State m_currentState;

	ImVec4 clearColor;

	vector<CApplicationState*> states;	// stack of states


	~Application();

	Application();


	void init(); // Intialize our application

	void draw(); // Draw current state

	void update(float tpf); // Update current state 

	void handleEvents(); // Handles input from mouse, joystick and mouse

	void cleanup();	// Clean up any resource allocation of state

	void changeState(CApplicationState* state); // switch from one state to another

	void pushState(CApplicationState* state); // push current state to teh stack	

	void popState(); // pop las state from stack

	bool running(); // Return true if this application is running

	void quit(); // Quit application

	void updateFPS(); // Calculates FPS 

	CApplicationState* getCurrentState(); // returns a pointer to the current state

	XMLConfigurationProvider *confProvider; // configuration provider service

	FileLogProvider *logProvider; // logprovider service

	TextureProvider *textureProvider; // TEXTURE provider service

	HelpProvider *helpProvider; // HELP provider

	CameraProvider *cameraProvider; // CAMERA provider

	StateProvider *stateProvider; // States provider

	GameProvider *gameProvider; // Game provider

	GLFWInputProvider *inputProvider; // input provider

	IMGUIProvider *imGuiProvider; // gui provider

	PCTextProvider *textProvider; // text provider

	PCAudioProvider *audioProvider; // audio provider

	GRIDProvider *gridProvider; // GRID provider

	ImguiComponentProvider *imguiComponentProvider; // Toolbar provider

	RenderProvider *renderProvider; // Render Provider

private:

	bool m_running; // application is running
};

#endif