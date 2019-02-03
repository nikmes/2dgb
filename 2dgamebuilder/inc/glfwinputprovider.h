/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file glfwinputprovider.h
*
*/

#ifndef GLFWINPUTPROVIDER_H
#define GLFWINPUTPROVIDER_H

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <imgui/imgui.h>

class GLFWInputProvider
{
private:
	
public:

	~GLFWInputProvider();

	GLFWInputProvider();

	void init();

	void resetInput()
	{
		g_MouseWasPressed[0] = false;
		g_MouseWasPressed[1] = false;
		g_MouseWasPressed[2] = false;
	}

	void static cursor_position_callback(GLFWwindow* window, double x, double y)
	{

	}

	void static mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS && button >= 0 && button < 3)
		{
			g_MouseWasPressed[button] = true;
		}

		if (action == GLFW_RELEASE && button >= 0 && button < 3)
		{
//			g_MouseWasPressed[button] = false;
		}
	}

	void static scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.MouseWheel += (float)yoffset; // Use fractional mouse wheel, 1.0 unit 5 lines.
	}

	void static key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		// Callback function called by GLFW when window size changes

		ImGuiIO& io = ImGui::GetIO();
		
		if (action == GLFW_PRESS)
		{
			io.KeysDown[key] = true;
		}
			
		if (action == GLFW_RELEASE)
		{
			io.KeysDown[key] = false;
		}
		
		(void)mods; // Modifiers are not reliable across systems

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];

	}

	void static char_callback(GLFWwindow* window, unsigned int c)
	{
		ImGuiIO& io = ImGui::GetIO();

		if (c > 0 && c < 0x10000)
		{
			io.AddInputCharacter((unsigned short)c);
		}
	}

	void getCursorPos(double *mouseX, double *mouseY);
};

#endif