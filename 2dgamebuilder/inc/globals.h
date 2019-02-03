/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file globals.h
*
*/

#ifndef GLOBALS_H
#define GLOBALS_H

#include "platform.h"

#include <GLFW/glfw3.h>
#define IMCOL32(R,G,B,A)    (((ImU32)(A)<<24) | ((ImU32)(B)<<16) | ((ImU32)(G)<<8) | ((ImU32)(R)))
#define IMCOL32_WHITE       (0xFFFFFFFF)
#define IMCOL32_BLACK       (0xFF000000)
#define IMCOL32_BLACK_TRANS (0x00000000)    // Transparent black
extern GLFWwindow *gWin;
extern bool g_MouseWasPressed[3];
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

extern int totalRenderedSprites;
extern float camX;
extern float camY;


#endif



