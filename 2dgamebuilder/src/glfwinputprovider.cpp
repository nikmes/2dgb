#include "globals.h"
#include "glfwinputprovider.h"

GLFWInputProvider::GLFWInputProvider()
{
	init();
}

GLFWInputProvider::~GLFWInputProvider()
{

}

void GLFWInputProvider::init()
{
	glfwSetMouseButtonCallback(gWin, this->mouse_button_callback); // Directly redirect GLFW mouse button events to AntTweakBar

	glfwSetCursorPosCallback(gWin, this->cursor_position_callback); 	// Directly redirect GLFW mouse position events to AntTweakBar

	glfwSetScrollCallback(gWin, this->scroll_callback); // Directly redirect GLFW mouse wheel events to AntTweakBar

	glfwSetKeyCallback(gWin, this->key_callback); 	// Directly redirect GLFW key events to AntTweakBar

	glfwSetCharCallback(gWin, this->char_callback); 	// Directly redirect GLFW char events to AntTweakBar
	
}

void GLFWInputProvider::getCursorPos(double *mouseX, double *mouseY)
{
	glfwGetCursorPos(gWin, mouseX, mouseY);
}