/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file globals.cpp
*
*/

#include "platform.h"

#include <GLFW/glfw3.h>
GLFWwindow *gWin;
bool g_MouseWasPressed[3] = { false,false,false };
int	totalRenderedSprites = 0;
float camX=1920/2;
float camY=1200/2;
