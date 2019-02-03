/**
* \date    14/02/2015
* \author  Nicholas Messaritis
*
* \file    glfwvideoprovider.cpp
*
* Implementation of the glfw3 video service provider
*
*/

#ifndef GLFWVIDEOPROVIDER_H
#define GLFWVIDEOPROVIDER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifdef _MSC_VER
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif
#include <string>
#include <imgui/imgui.h>
#include "globals.h"
#include <glib2d/glib2d.h>


using namespace std;

class GLFWVideoProvider
{
private:

	string m_title;
	
	int m_scrWidth;

	int m_scrHeight;

	bool m_fullScreen;

public:

	bool m_takeSnapShot;

	GLFWVideoProvider(const char* title);

	GLFWVideoProvider(const char* title, int width, int height);

	void swithFullScreen();

	void init();

	void destroy();

	void getWindowSize(int *width, int *height);
	
	// static methods GLFW Callbacks
	
	void static error_callback(int error, const char* description)
	{
		printf("%s", description);
		fputs(description, stderr);
	}

	void static framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void static windowSizeCB(GLFWwindow* window, int width, int height)
	{
		// Callback function called by GLFW when window size changes

		//printf("Window resized. New resolution [%dx%d] \n", width, height);
		
		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();
	
		glOrtho(0.0f, width, height, 0.0f, -1000.0f, +1000.0f);

		glViewport(0, 0, width, height);
		
	}

	void static windowclose_callback(GLFWwindow* window)
	{
		exit(0);
	}

	void takeSnapShot();

};

#endif
