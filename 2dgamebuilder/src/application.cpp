/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file application.cpp
*
*/

#include <stdio.h>
#include <random>
#include "application.h"
#include "applicationstate.h"
#include "globals.h"

/**
* Default destructor
*
*/

Application::~Application()
{
}

/**
* Default constructor
*
*/

Application::Application()
{

}

/**
* Initialize the application
*
* @return Void
*/

void Application::init()
{
	m_running = true;

	// Initialize Configuration Provider
	confProvider = new XMLConfigurationProvider("config.xml");
	ServiceLocator::RegisterServiceLocator(confProvider);

	// Initialize Log Provider
	logProvider = new FileLogProvider(confProvider->pLog_logfilename);
	ServiceLocator::RegisterServiceLocator(logProvider);
	ServiceLocator::getLogService()->configure("log");

	// Initialize Texture Provider
	textureProvider = new TextureProvider();
	ServiceLocator::RegisterServiceLocator(textureProvider);

	// HELP provider
	helpProvider = new HelpProvider();
	ServiceLocator::RegisterServiceLocator(helpProvider);

	// Initialize the Camera provider
	cameraProvider = new CameraProvider(0, 0);
	ServiceLocator::RegisterServiceLocator(cameraProvider);

	// Initialize State Machine Provider
	stateProvider = new StateProvider(this);
	ServiceLocator::RegisterServiceLocator(stateProvider);

	// Initialize Game provider 
	gameProvider = new GameProvider();
	ServiceLocator::RegisterServiceLocator(gameProvider);

	// Initialize Video provider
	// Initialize Render provider (Maybe we should consolidate together with the Video Provider)

	if (confProvider->pVideo_fullscreen)
	{
		//videoProvider = new GLFWVideoProvider("Template Game Engine Application");
		renderProvider = new RenderProvider("Template Game Engine Application");
	}
	else
	{
		//videoProvider = new GLFWVideoProvider("Template Game Engine Application", confProvider->pVideo_resolution.x, confProvider->pVideo_resolution.y);
		renderProvider = new RenderProvider("Template Game Engine Application", confProvider->pVideo_resolution.x, confProvider->pVideo_resolution.y);
	}

	//ServiceLocator::RegisterServiceLocator(videoProvider);
	ServiceLocator::RegisterServiceLocator(renderProvider);

	// PC audio provider
	//PCAudioProvider pcAudioProvider;
	//ServiceLocator::RegisterServiceLocator(&pcAudioProvider);

	// Initialize OpenGL Text Provider
	textProvider = new PCTextProvider();
	ServiceLocator::RegisterServiceLocator(textProvider);

	// Initialize GLFW Input Provider
	inputProvider = new GLFWInputProvider();
	ServiceLocator::RegisterServiceLocator(inputProvider);

	// Initialize IMGUI Component Provider
	imguiComponentProvider = new ImguiComponentProvider();
	ServiceLocator::RegisterServiceLocator(imguiComponentProvider);

	// IMGUI input provider
	imGuiProvider = new IMGUIProvider();
	ServiceLocator::RegisterServiceLocator(imGuiProvider);

	// Initialize Grid provider
	gridProvider = new GRIDProvider();
	ServiceLocator::RegisterServiceLocator(gridProvider);

	this->logProvider = ServiceLocator::getLogService();

	this->confProvider = ServiceLocator::getConfigurationService();

	//this->videoProvider = ServiceLocator::getVideoService();

	this->inputProvider = ServiceLocator::getInputService();

	this->helpProvider = ServiceLocator::getHelpService();

	this->cameraProvider = ServiceLocator::getCameraService();

	this->gameProvider = ServiceLocator::getGameService();

	this->textProvider = ServiceLocator::getTextService();

	this->imguiComponentProvider = ServiceLocator::getImguiComponentService();

	this->imGuiProvider = ServiceLocator::getImguiService();

	this->gridProvider = ServiceLocator::getGridService();

	this->renderProvider = ServiceLocator::getRenderService();

	clearColor = ServiceLocator::getConfigurationService()->pEditor_color;
}

void Application::handleEvents()
{
	this->inputProvider->resetInput();

	glfwPollEvents();

	states.back()->handleEvents();

	ServiceLocator::getRenderService()->m_takeSnapShot |= (ImGui::GetIO().KeyAlt && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_C)));
}

void Application::updateFPS()
{

}

void Application::update(float tpf)
{
	states.back()->update(tpf);
}

void Application::draw()
{
	g2dColor col;

	int height, width;

	static int pos = 1;

	RenderProvider *rp = ServiceLocator::getRenderService();
	
	static float rot = 0;

	if (m_currentState != INTRO)
	{
		clearColor = confProvider->pEditor_color;
	}
	else
	{
		clearColor.x = 0;
		clearColor.y = 0;
		clearColor.z = 0;
		clearColor.w = 0;
	}

	rp->g2dClear(clearColor);

	if (m_currentState == INTRO)
	{
		states.back()->draw(); // draw the state related stuff
	}

	if (m_currentState != INTRO)
	{
		rp->g2dSetGlobalScale(1.f);

		rp->g2dResetScale();
	
		ServiceLocator::getImguiService()->NewFrame();
		
		states.back()->draw(); // draw the state related stuff

		this->gridProvider->draw();

		g2dSetGlobalScale(1.f);
		
		g2dResetScale();

		ServiceLocator::getTextService()->drawLines();

		if (ServiceLocator::getConfigurationService()->pEditor_drawpspbox)
		{
			float cameraZoom = ServiceLocator::getCameraService()->zoom;

			ServiceLocator::getRenderService()->getWindowSize(&width, &height);
			
			g2dBeginRects(ServiceLocator::getTextureService()->GetTextureFromFileSystem("resources/consoleimages/psp-real.png"));
			{
				g2dSetCoordMode(G2D_CENTER);
				g2dSetCoordXY((width / 2)-2*cameraZoom, (height / 2)+13*cameraZoom);//, 100);
				//!!! FIX: hardcode, need to define parameter to choose if psp or psvita
				g2dSetScaleWH(839*cameraZoom,360*cameraZoom); // psp
				//g2dSetScaleWH(1610 * cameraZoom, 727 * cameraZoom); // ps vita
				g2dAdd();
				// ** UNCOMENT BELOW 2 lines TO TEST Z BUFFERING **
				//g2dSetCoordXYZ(width / 4, height / 2, -98);
				//g2dAdd();
				g2dEnd(); 
			}	
		}
		ServiceLocator::getImguiService()->draw(); // draw gui on top of everything
	}

	rp->g2dSwap();

	if (ServiceLocator::getRenderService()->m_takeSnapShot)
	{
		ServiceLocator::getRenderService()->takeSnapShot();
		ServiceLocator::getRenderService()->m_takeSnapShot = false;
	}

	rot += 3.0;
}

void Application::changeState(CApplicationState* state)
{
	// cleanup the current state

	if (!states.empty())
	{
		states.back()->cleanup();

		states.pop_back();
	}

	// store and init the new state

	states.push_back(state);

	states.back()->init(this);
}

void Application::pushState(CApplicationState* state)
{
	// pause current state

	if (!states.empty())
	{
		states.back()->pause();
	}

	// store and init the new state

	states.push_back(state);

	states.back()->init(this);
}

void Application::popState()
{
	// cleanup the current state

	if (!states.empty())
	{
		states.back()->cleanup();

		states.pop_back();
	}

	// resume previous state

	if (!states.empty())
	{
		states.back()->resume();
	}
}

bool Application::running()
{
	return m_running;
}

void Application::quit()
{
	// Free all resource shutdown services and quit out application.

	ServiceLocator::getLogService()->destroy();

	ServiceLocator::getConfigurationService()->destroy();

	ServiceLocator::getRenderService()->destroy();

	m_running = false;
}

CApplicationState* Application::getCurrentState()
{
	return states.back();
}
