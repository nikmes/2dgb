/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file textureprovider.h
*
*/

#ifndef TEXTUREPROVIDER_H
#define TEXTUREPROVIDER_H

#include <string>
#include <map>
#include <png.h>
#include "definitions.h"

using namespace std;

class TextureProvider
{
private:
	typedef map<string, g2dTexture*> TTextureMap;	// The map of already loaded textures. There are indexed using their filename.
	
	TTextureMap m_Textures;

public:

	TextureProvider();

	~TextureProvider();

	g2dTexture* GetImage(const string& strTextName);

	g2dTexture* GetTexture(const string& strTextName);

	g2dTexture* GetTextureFromFileSystem(const string& strTextName);

	bool ReleaseTexture(const string& strTextName);

	void ReleaseAllTextures();

	void ReloadAllTextures();

	g2dTexture* textureLoad(char path[], g2dTex_Mode mode);

	g2dTexture* textureLoadPNG(FILE *fp);

	unsigned int getNextPower2(unsigned int n);

	g2dTexture* textureCreate(int w, int h);

	void textureFree(g2dTexture **tex);

	void swizzle(unsigned char *dest, unsigned char *source, int width, int height);

	g2dTexture* copy2texture(int x, int y, int w, int h, string name, g2dTexture *srcTexture, g2dTex_Mode mode);

};

#endif