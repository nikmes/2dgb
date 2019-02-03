/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file glfwvideoprovider.cpp
*
*/

#include "servicelocator.h"
#include "globals.h"
#include <glib2d/glib2d.h>
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"
//#include <fstream>

/**
* Default constructor
*
*/

GLFWVideoProvider::GLFWVideoProvider(const char* title)
{
	m_takeSnapShot = false;

	m_fullScreen = true;

	this->init();

	m_title = title;

	m_scrWidth = ServiceLocator::getConfigurationService()->pVideo_resolution.x;

	m_scrHeight = ServiceLocator::getConfigurationService()->pVideo_resolution.y;
}

GLFWVideoProvider::GLFWVideoProvider(const char* title, int width, int height)
{
	m_title = title;

	m_scrWidth = width;

	m_scrHeight = height;
	
	m_fullScreen = false;

	this->init();
}

void GLFWVideoProvider::swithFullScreen()
{
	int vSyncMode = 0;

	if (!m_fullScreen)
	{
		const GLFWvidmode *vidmode;

		int count,w,h;

		glfwGetWindowSize(gWin, &w, &h);

		ServiceLocator::getConfigurationService()->pVideo_resolution.x = w;

		ServiceLocator::getConfigurationService()->pVideo_resolution.y = h;

		GLFWmonitor** monitors = glfwGetMonitors(&count);

		vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		m_scrWidth = vidmode->width;

		m_scrHeight = vidmode->height;

		GLFWwindow *newWindow = glfwCreateWindow(vidmode->width, vidmode->height, m_title.c_str(), glfwGetPrimaryMonitor(), gWin);

		glfwDestroyWindow(gWin);

		gWin = newWindow;

		m_fullScreen = true;

		ServiceLocator::getConfigurationService()->pVideo_fullscreen = true;
	}
	else 
	{
		m_scrWidth = ServiceLocator::getConfigurationService()->pVideo_resolution.x;

		m_scrHeight = ServiceLocator::getConfigurationService()->pVideo_resolution.y;

		GLFWwindow *newWindow = glfwCreateWindow(m_scrWidth, m_scrHeight, m_title.c_str(), NULL, gWin);

		glfwDestroyWindow(gWin);

		gWin = newWindow;

		m_fullScreen = false;

		ServiceLocator::getConfigurationService()->pVideo_fullscreen = false;

	}

	if (!gWin)
	{
		TRACE(T_DEB, "Error window not created");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	int width, height;

	glfwSetFramebufferSizeCallback(gWin, framebuffer_size_callback);

	glfwMakeContextCurrent(gWin);

	vSyncMode = ServiceLocator::getConfigurationService()->pOpenGlVsync;

	if (vSyncMode == 1)
	{
		glfwSwapInterval(1); // 1 - enable vsync 60
	}

	if (vSyncMode == 2)
	{
		glfwSwapInterval(2); // 2 - enable vsync 30 - disable vsync
	}

	if (vSyncMode == 0)
	{
		glfwSwapInterval(0); // 2 - disable vsync
	}

	glfwSetWindowSizeCallback(gWin, windowSizeCB); // - Redirect window size changes to the callback function windowSizeCB

	glfwSetWindowCloseCallback(gWin, windowclose_callback);

	glfwSetErrorCallback(error_callback);

	glfwGetWindowSize(gWin, &width, &height);

	glewInit();

	// Setup OpenGL

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glOrtho(0.0f, width, height, 0, -1000.0f, +1000.0f);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_MODELVIEW);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glDisable(GL_BLEND);

	glDisable(GL_DITHER);

	glDisable(GL_LIGHTING);
	
	glAlphaFunc(GL_GREATER, 0.1);

	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);

	glDepthRangef(0, 65535);

	glClearDepthf(65535);

	g2dSetGlobalScale(1.0);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	ServiceLocator::getInputService()->init();
}

void GLFWVideoProvider::init()
{
	int vSyncMode = 0;

	const GLFWvidmode *vidmode;

	int width, height;

	if (!glfwInit())
	{
		TRACE(T_DEB, "Fatal error during GLFW3 intialization");

		exit(EXIT_FAILURE);
	}

	if (m_fullScreen)
	{
		int count;

		GLFWmonitor** monitors = glfwGetMonitors(&count);

		vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		GLFWwindow *newWindow = glfwCreateWindow(vidmode->width, vidmode->height, "", glfwGetPrimaryMonitor(), gWin);
		
		glfwDestroyWindow(gWin);

		gWin = newWindow;
	}
	else
	{
		GLFWwindow *newWindow = glfwCreateWindow(m_scrWidth, m_scrHeight, m_title.c_str(), NULL, gWin);
		
		glfwDestroyWindow(gWin);

		gWin = newWindow;
	}

	if (!gWin)
	{
		TRACE(T_DEB, "Error window not created"); 
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetFramebufferSizeCallback(gWin, framebuffer_size_callback);

	glfwMakeContextCurrent(gWin);

	vSyncMode = ServiceLocator::getConfigurationService()->pOpenGlVsync;

	if (vSyncMode == 1)
	{
		glfwSwapInterval(1); // 1 - enable vsync 60
	}

	if (vSyncMode == 2)
	{
		glfwSwapInterval(2); // 2 - enable vsync 30 - disable vsync
	}

	if (vSyncMode == 0)
	{
		glfwSwapInterval(0); // 2 - disable vsync
	}

	glfwSetWindowSizeCallback(gWin, windowSizeCB); // - Redirect window size changes to the callback function windowSizeCB

	glfwSetWindowCloseCallback(gWin, windowclose_callback);

	glfwSetErrorCallback(error_callback);
	
	glfwGetWindowSize(gWin, &width, &height);
	
	glewInit();

	// Setup OpenGL

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glOrtho(0.0f, width, height, 0, -1000.0f, +1000.0f);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_MODELVIEW);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//glDisable(GL_BLEND);

	glDisable(GL_DITHER);

	glDisable(GL_LIGHTING);

	glAlphaFunc(GL_GREATER, 0.1);
	
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);

	glDepthRangef(0, 65535);

	glClearDepthf(65535);

	g2dSetGlobalScale(1.0);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void GLFWVideoProvider::destroy()
{

}

void GLFWVideoProvider::getWindowSize(int *width, int *height)
{
	glfwGetWindowSize(gWin, width, height);
}

void GLFWVideoProvider::takeSnapShot()
{
	/*
	* makes a screenshot of the editor and saves it in file
	*
	static int id = 0;
	int width, height;
	char outName[256];
	sprintf(outName, "out_%d.png", id);
	getWindowSize(&width, &height);

	unsigned char* pixels = new unsigned char[3 * width * height];
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	unsigned char* line_tmp = new unsigned char[3 * width];
	unsigned char* line_a = pixels;
	unsigned char* line_b = pixels + (3 * width * (height - 1));
	while (line_a < line_b)
	{
		memcpy(line_tmp, line_a, width * 3);
		memcpy(line_a, line_b, width * 3);
		memcpy(line_b, line_tmp, width * 3);
		line_a += width * 3;
		line_b -= width * 3;
	}

	stbi_write_png(outName, width, height, 3, pixels, width * 3);
	id++;
	delete[] pixels;
	delete[] line_tmp;
	*/

}
