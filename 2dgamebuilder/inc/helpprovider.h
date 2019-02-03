#ifndef HELPPROVIDER_H
#define HELPPROVIDER_H

#ifdef PLATFORM_PSP
#include <psptypes.h>
#endif

class HelpProvider
{
private:
	double dt;
#if defined(PLATFORM_PSP)
	u64 timeNow;
	u64 timeLastAsk;
	u32 tickResolution;
#else
	double timeNow;
	double timeLastAsk;
	double tickResolution;
#endif

public:

	HelpProvider();

	~HelpProvider();

public:

	double GetDeltaTime(void);	// Returns the delta time

	static int id;

	unsigned int convertHEXtoAlpha(unsigned char a);

	unsigned int convertRGBtoColor(unsigned int r, unsigned int g, unsigned int b);

	int generateId();

	void flipHorizontal(unsigned char* image, int w, int h, int stride);

	g2dColor convertToPSPColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);

	string RemoveExtension(const string str);

	string SplitFilename(const string str);

	string SplitPath(const string str);

#ifdef PLATFORM_PSP
	u32 ramAvailable(void);

	u32 ramAvailableLineareMax(void);

	void getCurrentTick(u64 *now);

	void getCurrentRes(u32* res);

#endif

};

#endif
