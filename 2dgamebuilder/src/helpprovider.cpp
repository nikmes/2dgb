#include "servicelocator.h"
#include "helpprovider.h"
#include <stdio.h>
#include <string>

using namespace std;

int HelpProvider::id = 0;

HelpProvider::HelpProvider()
{
	timeLastAsk = glfwGetTime() / 0.8f;
}

HelpProvider::~HelpProvider()
{

}

string HelpProvider::RemoveExtension(const string str)
{
	string path;
	string fileName;
	size_t found = str.find_last_of(".");
	path = str.substr(0, found);
	return path;
}

string HelpProvider::SplitFilename(const string str)
{
	string fileName;
	size_t found = str.find_last_of("/\\");
	fileName = str.substr(found + 1);
	return fileName;
}

string HelpProvider::SplitPath(const string str)
{
	string path;
	string fileName;
	size_t found = str.find_last_of("/\\");
	path = str.substr(0, found);
	return path;
}

unsigned int HelpProvider::convertHEXtoAlpha(unsigned char a)
{
	char buf[50];

	sprintf(buf, "0x%2.2X", a);

	string hex(buf);

	unsigned int alpha;

	sscanf(hex.c_str(), "%x", &alpha);

	return alpha;
}

unsigned int HelpProvider::convertRGBtoColor(unsigned int r, unsigned int g, unsigned int b)
{
	char buf[50];

	sprintf(buf, "0xFF%2.2X%2.2X%2.2X", r, g, b);

	string hex(buf);

	unsigned int decimalValue;

	sscanf(hex.c_str(), "%x", &decimalValue);

	return decimalValue;
}
g2dColor HelpProvider::convertToPSPColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	g2dColor color = r | (g << 8) | (b << 16) | (a << 24);

	return color;
}

int HelpProvider::generateId()
{
	id++;
	return id;
}

void HelpProvider::flipHorizontal(unsigned char* image, int w, int h, int stride)
{
	int i = 0, j = h - 1, k;
	while (i < j) {
		unsigned char* ri = &image[i * stride];
		unsigned char* rj = &image[j * stride];
		for (k = 0; k < w * 4; k++) {
			unsigned char t = ri[k];
			ri[k] = rj[k];
			rj[k] = t;
		}
		i++;
		j--;
	}
}

double HelpProvider::GetDeltaTime(void)
{
	timeNow = glfwGetTime() / 0.8f;
	double dt = (timeNow - timeLastAsk); 
	timeLastAsk = timeNow;
	return dt;
}
