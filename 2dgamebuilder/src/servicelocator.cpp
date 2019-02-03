/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file servicelocator.cpp
*
*/

#include "servicelocator.h"

// common platform service providers

XMLConfigurationProvider *ServiceLocator::m_configurationProvider = NULL;

FileLogProvider *ServiceLocator::m_logProvider = NULL;

TextureProvider *ServiceLocator::m_textureProvider = NULL;

HelpProvider *ServiceLocator::m_helpProvider = NULL;

CameraProvider *ServiceLocator::m_cameraProvider = NULL;

StateProvider* ServiceLocator::m_stateProvider = NULL;

GameProvider* ServiceLocator::m_gameProvider = NULL;

PCAudioProvider* ServiceLocator::m_audioProvider = NULL;

PCTextProvider* ServiceLocator::m_textProvider = NULL;

GLFWInputProvider* ServiceLocator::m_inputProvider = NULL;

IMGUIProvider* ServiceLocator::m_imguiProvider = NULL;

GRIDProvider* ServiceLocator::m_gridProvider = NULL;

ImguiComponentProvider* ServiceLocator::m_imguiComponentProvider = NULL;

RenderProvider* ServiceLocator::m_renderProvider = NULL;

