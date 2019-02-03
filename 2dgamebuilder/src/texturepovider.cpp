#include "textureprovider.h"
#include "servicelocator.h"


using namespace std;

TextureProvider::TextureProvider()
{

}

TextureProvider::~TextureProvider()
{
	// Release all textures loaded so far in memmory

	this->ReleaseAllTextures();

	TRACE(T_DEB, "TextureManager - destructor finished cleaning up");
}

g2dTexture* TextureProvider::GetImage(const string& strTextName)
{
	// Look in the map if the texture is already loaded.

	string fullPath = ServiceLocator::getGameService()->m_directory;
	fullPath.append("images/");
	fullPath.append(strTextName);
	TTextureMap::const_iterator iter = m_Textures.find(fullPath);

	if (iter != m_Textures.end())
	{
		return iter->second;
	}

	// If it was not found, try to load it from file. If the load failed, delete the texture and throw an exception.

	g2dTexture* pNewText = textureLoad((char*)fullPath.c_str(), G2D_SWIZZLE);

	if (pNewText != NULL)
	{
		TRACE(T_DEB, "Loaded texture [%s] with id [%d]", strTextName.c_str(), pNewText->id);
		// Store the newly loaded texture and return it.
		m_Textures[fullPath] = pNewText;
		return pNewText;
	}
	else
	{
		return NULL;
	}
}

g2dTexture* TextureProvider::GetTexture(const string& strTextName)
{
	// Look in the map if the texture is already loaded.

	string fullPath = ServiceLocator::getGameService()->m_directory;
	fullPath.append("texturemaps/");
	fullPath.append(strTextName);
	TTextureMap::const_iterator iter = m_Textures.find(fullPath);

	if (iter != m_Textures.end())
	{
		return iter->second;
	}

	// If it was not found, try to load it from file. If the load failed, delete the texture and throw an exception.

	g2dTexture* pNewText = textureLoad((char*)fullPath.c_str(), G2D_SWIZZLE);

	if (pNewText != NULL)
	{
		TRACE(T_DEB, "Loaded texture [%s] with id [%d]", strTextName.c_str(), pNewText->id);
		// Store the newly loaded texture and return it.
		m_Textures[fullPath] = pNewText;
		return pNewText;
	}
	else
	{
		return NULL;
	}
}

g2dTexture* TextureProvider::GetTextureFromFileSystem(const string& strTextName)
{
	// Look in the map if the texture is already loaded.

	TTextureMap::const_iterator iter = m_Textures.find(strTextName);

	if (iter != m_Textures.end())
	{
		return iter->second;
	}

	// If it was not found, try to load it from file. If the load failed, delete the texture and throw an exception.

	g2dTexture* pNewText = textureLoad((char*)strTextName.c_str(), G2D_SWIZZLE);

	if (pNewText != NULL)
	{
		TRACE(T_DEB, "Loaded texture [%s] with id [%d]", strTextName.c_str(), pNewText->id);
		// Store the newly loaded texture and return it.
		m_Textures[strTextName] = pNewText;
		return pNewText;
	}
	else
	{
		return NULL;
	}
}

bool TextureProvider::ReleaseTexture(const std::string& strTextName)
{
	// Retrieve the texture from the map

	bool bFound = false;

	// append the BASE_PATH
	string fullPath = "";
	fullPath.append(BASE_PATH);
	fullPath.append(strTextName);

	// Look in the map if the texture is already loaded.
	TTextureMap::iterator iter = m_Textures.find(fullPath);

	if (iter != m_Textures.end())
	{
		// If it was found, we delete it and remove the
		// pointer from the map.
		bFound = true;
		if (iter->second)
		{
			textureFree(&iter->second);
			TRACE(T_DEB, "Free Texture = [%s]", fullPath.c_str());
			delete iter->second;
		}
		m_Textures.erase(iter);
	}

	return bFound;
}

void TextureProvider::ReleaseAllTextures()
{
	// Release all textures loaded so far in memmory

	TTextureMap::iterator it;
	string key = "";

	TRACE(T_DEB, "TextureManager - will atempt to release all loaded textures");

	for (it = m_Textures.begin(); it != m_Textures.end();it++)
	{
		key = it->first;
		TRACE(T_DEB, "Free Texture = [%s]", key.c_str());
		textureFree(&it->second);
	}

	TRACE(T_DEB, "TextureManager - finished freeing texture resources");
}

void TextureProvider::ReloadAllTextures()
{
	// Release all textures loaded so far in memmory

	TTextureMap::iterator it;
	string key = "";

	TRACE(T_DEB, "TextureManager - will attempt to reload all currently loaded textures");

	for (it = m_Textures.begin(); it != m_Textures.end();it++)
	{
		key = it->first;
		it->second = textureLoad((char*)key.c_str(), G2D_SWIZZLE);
		TRACE(T_DEB, "Reload texture = [%s] with opengl ID = [%d]", key.c_str(), it->second->id);
	}

	TRACE(T_DEB, "TextureManager - reloaded all currently loaded textures");
}

g2dTexture* TextureProvider::textureLoad(char path[], g2dTex_Mode mode)
{
#ifndef PLATFORM_PSP
	g2dTexture *tex = NULL;

	FILE *fp = NULL;

	if (path == NULL)
	{
		return NULL;
	}

	if ((fp = fopen(path, "rb")) == NULL)
	{
		return NULL;
	}

	if (strstr(path, ".png"))
	{
		tex = textureLoadPNG(fp);
	}

	if (tex == NULL)
		goto error;

	fclose(fp);
	fp = NULL;

	// The PSP can't draw 512*512+ textures.
	//if (tex->w > 512 || tex->h > 512)
	//    goto error;

	glGenTextures(1, &tex->id);
	
	TRACE(T_DEB, "Texture ID generated for texture file [%s]=[%d]", path, tex->id);
	
	glBindTexture(GL_TEXTURE_2D, tex->id);
	
	glTexImage2D(GL_TEXTURE_2D, 0, 4, tex->tw, tex->th, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;

	// Load failure... abort
error:
	if (fp != NULL)
	{
		fclose(fp);
	}

	TRACE(T_ERR, "Error loading texture file");

	textureFree(&tex);

	return NULL;
#else
	g2dTexture *tex = NULL;
	FILE *fp = NULL;

	if (path == NULL)
		return NULL;
	if ((fp = fopen(path, "rb")) == NULL)
		return NULL;

	if (strstr(path, ".png"))
	{
		tex = textureLoadPNG(fp);
	}

	if (tex == NULL)
		goto error;

	fclose(fp);
	fp = NULL;

	// The PSP can't draw 512*512+ textures.
	if (tex->w > 512 || tex->h > 512)
		goto error;

	// Swizzling is useless with small textures.
	if ((mode & G2D_SWIZZLE) && (tex->w >= 16 || tex->h >= 16))
	{
		u8 *tmp = (u8*)malloc(tex->tw*tex->th*PIXEL_SIZE);
		swizzle(tmp, (u8*)tex->data, tex->tw*PIXEL_SIZE, tex->th);
		free(tex->data);
		tex->data = (g2dColor*)tmp;
		tex->swizzled = true;
	}
	else
		tex->swizzled = false;

	sceKernelDcacheWritebackRange(tex->data, tex->tw*tex->th*PIXEL_SIZE);

	tex->id=ServiceLocator::getHelpService()->generateId();

	return tex;

	// Load failure... abort
error:
	if (fp != NULL)
		fclose(fp);

	g2dTexFree(&tex);

	return NULL;
#endif
}

g2dTexture* TextureProvider::textureLoadPNG(FILE *fp)
{
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	unsigned int x, y;
	g2dColor *line;
	g2dTexture *tex;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_set_error_fn(png_ptr, NULL, NULL, NULL);
	info_ptr = png_create_info_struct(png_ptr);

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
	png_set_strip_16(png_ptr);
	png_set_packing(png_ptr);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_palette_to_rgb(png_ptr);
	}
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png_ptr);
	}

	png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

	tex = textureCreate(width, height);
	
	line = (unsigned int *)malloc(width * 4);

	for (y = 0; y < height; y++)
	{
		png_read_row(png_ptr, (unsigned char*)line, NULL);

		for (x = 0; x < width; x++)
		{
			tex->data[x + y*tex->tw] = line[x];
		}
	}

	free(line);
	png_read_end(png_ptr, info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	return tex;
}

unsigned int TextureProvider::getNextPower2(unsigned int n)
{
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;

	return n + 1;
}

g2dTexture* TextureProvider::textureCreate(int w, int h)
{
	g2dTexture *tex = (g2dTexture *)malloc(sizeof(g2dTexture));

	if (tex == NULL)
	{
		return NULL;
	}

	tex->tw = getNextPower2(w);
	tex->th = getNextPower2(h);
	tex->w = w;
	tex->h = h;
	tex->ratio = (float)w / (float)h;
	tex->swizzled = false;

	tex->data = (unsigned int *)malloc(tex->tw * tex->th * sizeof(g2dColor));

	if (tex->data == NULL)
	{
		free(tex);
		return NULL;
	}

	memset(tex->data, 0, tex->tw * tex->th * sizeof(g2dColor));

	return tex;
}

void  TextureProvider::textureFree(g2dTexture **tex)
{
	if (tex == NULL)
	{
		return;
	}

	if (*tex == NULL)
	{
		return;
	}

#ifndef PSP
	unsigned int uiGLID = (unsigned int)(*tex)->id;

	TRACE(T_DEB, "Delete texture [%d]", (*tex)->id);

	glDeleteTextures(1, &uiGLID);
#endif
	free((*tex)->data);

	free((*tex));

	*tex = NULL;
}

void TextureProvider::swizzle(unsigned char *dest, unsigned char *source, int width, int height)
{
	int i, j;
	int rowblocks = (width / 16);
	int rowblocks_add = (rowblocks - 1) * 128;
	unsigned int block_address = 0;
	unsigned int *img = (unsigned int*)source;

	for (j = 0; j<height; j++)
	{
		unsigned int *block = (unsigned int*)(dest + block_address);

		for (i = 0; i < rowblocks; i++)
		{
			*block++ = *img++;
			*block++ = *img++;
			*block++ = *img++;
			*block++ = *img++;

			block += 28;
		}

		if ((j & 0x7) == 0x7)
			block_address += rowblocks_add;

		block_address += 16;
	}
}

g2dTexture* TextureProvider::copy2texture(int x, int y, int w, int h, string name, g2dTexture *srcTexture, g2dTex_Mode mode)
{

	g2dTexture *tex = NULL;

	int f_x, f_y;

	// if texture with same name already exists just return it
	TTextureMap::const_iterator iter = m_Textures.find(name);

	if (iter != m_Textures.end())
	{
		return iter->second;
	}

	// create the new texture 
	g2dTexture *destTexture = textureCreate(w, h);
	string fName = ServiceLocator::getGameService()->m_directory;
	fName.append("texturemaps/");
	fName.append(name);

	strcpy(destTexture->name,fName.c_str());

	int posX = 0;
	destTexture->w = w;
	destTexture->h = h;

	TRACE(T_DEB, "Texture: Name[%s] Real[%dx%d] Shown[%dx%d] Ratio[%f]", destTexture->name, destTexture->tw, destTexture->th, destTexture->w, destTexture->h, destTexture->ratio);
	
	for (f_y = y; f_y < y+h; f_y++)
	{
		for (f_x = x; f_x < x+w; f_x++)
		{
			destTexture->data[posX] = srcTexture->data[f_y*srcTexture->tw + f_x];
			posX++;	
		}
		posX += destTexture->tw - w;
	}

#ifndef PLATFORM_PSP
	glGenTextures(1, &destTexture->id);

	TRACE(T_DEB, "Texture ID generated for texture file [%s]=[%d]", name.c_str() , destTexture->id);

	glBindTexture(GL_TEXTURE_2D, destTexture->id);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, destTexture->tw, destTexture->th, 0, GL_RGBA, GL_UNSIGNED_BYTE, destTexture->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#endif

#ifdef PLATFORM_PSP

	// Swizzling is useless with small textures.
	if ((mode & G2D_SWIZZLE) && (destTexture->w >= 16 || destTexture->h >= 16))
	{
		u8 *tmp = (u8*)malloc(destTexture->tw*destTexture->th*PIXEL_SIZE);
		swizzle(tmp, (u8*)destTexture->data, destTexture->tw*PIXEL_SIZE, destTexture->th);
		free(destTexture->data);
		destTexture->data = (g2dColor*)tmp;
		destTexture->swizzled = true;
	}
	else
		destTexture->swizzled = false;

	sceKernelDcacheWritebackRange(destTexture->data, destTexture->tw*destTexture->th*PIXEL_SIZE);

	destTexture->id = ServiceLocator::getHelpService()->generateId();
#endif
	
	m_Textures[fName] = destTexture;

	// return the new texture
	return destTexture;
}