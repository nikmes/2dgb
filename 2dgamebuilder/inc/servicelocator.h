/**
* \date    14/02/2015
* \author  Nicholas Messaritis
*
* \file    servicelocator.h
*
* Service Locator Definition 
*
*/

#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include "platform.h"

/*
* common platform service providers
*/

#include "filelogprovider.h"
#include "xmlconfigurationprovider.h"
#include "textureprovider.h"
#include "helpprovider.h"
#include "cameraprovider.h"
#include "gameprovider.h"
#include "stateprovider.h"

/*
* platform specific services and definitions
*/

#define __func__ __FUNCTION__
#define __BASE_FILE__ __FILE__

#include "utils_filesystem.h"
#include "utils_string.h"

#include "glfwvideoprovider.h"
#include "glfwinputprovider.h"
#include "imguiprovider.h"
#include "pcaudioprovider.h"
#include "pctextprovider.h"
#include "gridprovider.h"
#include "imguicomponentprovider.h"
#include "renderprovider.h"

class ServiceLocator
{
public:

	// Common platform services

	static void RegisterServiceLocator(FileLogProvider *provider)
	{
		m_logProvider = provider;
	}

	static FileLogProvider* getLogService()
	{
		return m_logProvider;
	}

	static void RegisterServiceLocator(XMLConfigurationProvider *provider)
	{
		m_configurationProvider = provider;
	}

	static XMLConfigurationProvider* getConfigurationService()
	{
		return m_configurationProvider;
	}

	static void RegisterServiceLocator(TextureProvider *provider)
	{
		m_textureProvider = provider;
	}

	static TextureProvider* getTextureService()
	{
		return m_textureProvider;
	}

	static void RegisterServiceLocator(HelpProvider *provider)
	{
		m_helpProvider = provider;
	}

	static HelpProvider* getHelpService()
	{
		return m_helpProvider;
	}

	static void RegisterServiceLocator(CameraProvider *provider)
	{
		m_cameraProvider = provider;
	}

	static CameraProvider* getCameraService()
	{
		return m_cameraProvider;
	}

	static void RegisterServiceLocator(StateProvider *provider)
	{
		m_stateProvider = provider;
	}

	static StateProvider* getStateService()
	{
		return m_stateProvider;
	}

	static void RegisterServiceLocator(GameProvider *provider)
	{
		m_gameProvider = provider;
	}

	static GameProvider* getGameService()
	{
		return m_gameProvider;
	}

	//audio service provider

	static void RegisterServiceLocator(PCAudioProvider *provider)
	{
		m_audioProvider = provider;
	}

	static PCAudioProvider* getAudioService()
	{
		return m_audioProvider;
	}

	// video service provider
	 
	//static void RegisterServiceLocator(GLFWVideoProvider *provider)
	//{
	//	m_videoProvider = provider;
	//}

	//static GLFWVideoProvider* getVideoService()
	//{
	//	return m_videoProvider;
	//}

	// text service provider
	 
	static void RegisterServiceLocator(PCTextProvider *provider)
	{
		m_textProvider = provider;
	}

	static PCTextProvider* getTextService()
	{
		return m_textProvider;
	}

	// input service provider

	static void RegisterServiceLocator(GLFWInputProvider *provider)
	{
		m_inputProvider = provider;
	}

	static GLFWInputProvider* getInputService()
	{
		return m_inputProvider;
	}

	// imgui service provider

	static void RegisterServiceLocator(IMGUIProvider *provider)
	{
		m_imguiProvider = provider;
	}

	static IMGUIProvider* getImguiService()
	{
		return m_imguiProvider;
	}

	// GRID service provider

	static void RegisterServiceLocator(GRIDProvider *provider)
	{
		m_gridProvider = provider;
	}

	static GRIDProvider* getGridService()
	{
		return m_gridProvider;
	}

	// TOOLBAR service provider

	static void RegisterServiceLocator(ImguiComponentProvider *provider)
	{
		m_imguiComponentProvider = provider;
	}

	static ImguiComponentProvider* getImguiComponentService()
	{
		return m_imguiComponentProvider;
	}

	static void RegisterServiceLocator(RenderProvider *provider)
	{
		m_renderProvider = provider;
	}

	static RenderProvider* getRenderService()
	{
		return m_renderProvider;
	}

private:

	static GLFWVideoProvider *m_videoProvider;

	static PCAudioProvider *m_audioProvider;

	static PCTextProvider *m_textProvider;

	static GLFWInputProvider *m_inputProvider;

	static IMGUIProvider *m_imguiProvider;

	static GRIDProvider *m_gridProvider;

	static ImguiComponentProvider *m_imguiComponentProvider;

	static RenderProvider *m_renderProvider;

	static StateProvider *m_stateProvider;

	static FileLogProvider *m_logProvider;

	static XMLConfigurationProvider *m_configurationProvider;

	static TextureProvider *m_textureProvider;

	static HelpProvider *m_helpProvider;

	static CameraProvider *m_cameraProvider;

	static GameProvider *m_gameProvider;

};

#ifdef DEBUG
#define TRACE(TTYPE,TMSG,...) { if ( ServiceLocator::getLogService()!=NULL && (ServiceLocator::getLogService()->traceFlags & (1<<TTYPE)) ) ServiceLocator::getLogService()->logMsg(__LINE__, __BASE_FILE__, (char *)__func__, TTYPE, TMSG, ## __VA_ARGS__);}
#else
#define TRACE(TTYPE,TMSG,...) { }
#endif

#endif