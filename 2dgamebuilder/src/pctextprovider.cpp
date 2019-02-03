#include "servicelocator.h"

#include <stdio.h>
#include <string.h>
#define FONTSTASH_IMPLEMENTATION
#include "fontstash.h"
#include <GLFW/glfw3.h>
#define GLFONTSTASH_IMPLEMENTATION
#include "glfontstash.h"
#include <math.h>
#include <stdarg.h>

PCTextProvider::PCTextProvider()
{
	this->init();
}

void PCTextProvider::init()
{
	fontNormal = FONS_INVALID;
	fs = NULL;
	
	fs = glfonsCreate(1024, 1024, FONS_ZERO_TOPLEFT);

	if (fs == NULL) 
	{
		TRACE(T_ERR,"Could not create stash.\n");
	}
	else
	{
		TRACE(T_INF, "Fonts for PCTextProvider loaded [OK]");
	}

	fontNormal = fonsAddFont(fs, "sans", "extra_fonts/Inconsolata-Regular.ttf");

	if (fontNormal == FONS_INVALID) 
	{
		TRACE(T_ERR, "Could not add normal font.\n");
	}
	{
		TRACE(T_INF, "Inconsolata-Regular.ttf added to the stash [OK]");
	}
}

void PCTextProvider::print(float x, float y, const char *message, ...)
{
	textItem tItem;

	tItem.x = x;
	tItem.y = y;


	unsigned int green = glfonsRGBA(0, 255, 0, 255);

	fonsClearState(fs);
	fonsSetSize(fs, 12.0f);
	fonsSetFont(fs, fontNormal);
	fonsSetColor(fs, green);

	va_list argList;
	va_start(argList, message);
	vsnprintf(tItem.cbuffer, 1024, message, argList);
	va_end(argList);

	//glDisable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);
	fonsDrawText(fs, x, y, message, NULL);
	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);

}

void PCTextProvider::setStyle(float size, unsigned int color, unsigned int shadowcolor, float angle, unsigned int options)
{
	
}

void PCTextProvider::pushLine(float x, float y, const char *message, ...)
{
	textItem tItem;

	tItem.x = x;
	tItem.y = y;


	va_list argList;
	va_start(argList, message);
	vsnprintf(tItem.cbuffer, 1024, message, argList);
	va_end(argList);

	this->lines.push_back(tItem);
}

void PCTextProvider::drawLines()
{
	for (myIterator = lines.begin(); myIterator != lines.end(); myIterator++)
	{
		print(myIterator->x, myIterator->y,myIterator->cbuffer);
	}
	lines.clear();
}