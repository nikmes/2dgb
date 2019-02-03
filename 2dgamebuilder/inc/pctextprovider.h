/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file pspinputprovider.h
*
*/

#ifndef PCTEXTPROVIDER_H
#define PCTEXTPROVIDER_H

#include <vector>
#include <iostream>

#include "fontstash.h"
#include "definitions.h"

class PCTextProvider
{
private:

	float x, tmp_angle;

	std::vector<textItem> lines;

	std::vector<textItem>::iterator myIterator;

	int fontNormal;

	FONScontext* fs = NULL;

public:

	~PCTextProvider();

	PCTextProvider();

	void init();

	void setStyle(float size, unsigned int color, unsigned int shadowcolor, float angle, unsigned int options);
	
	void print(float x, float y, const char *message, ...);
	
	void pushLine(float x, float y, const char *message, ...);
	
	void drawLines();

};

#endif