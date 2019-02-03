#ifndef RENDERPROVIDER_H
#define RENDERPROVIDER_H

#include "servicelocator.h"

class RenderProvider
{
private:
	static int			 *dlist;
	static RenderContext  m_rctx;
	static Transform	  m_tstack[TSTACK_MAX];
	static unsigned int	  m_tstack_size;
	static bool			  m_init;
	static bool			  m_start;
	static bool			  m_gbegin;
	static bool			  m_zclear;
	static bool			  m_scissor;
	static float		  m_global_scale;
	string				  m_title;
	int					  m_scrWidth;
	int					  m_scrHeight;
	bool				  m_fullScreen;

	float* g2dSetVertex(float *vp, int i, float vx, float vy);
	void   g2dSetRectVertices(Object currObj);
	void   g2dSetLineVertices(Object currObj);
	void   g2dSetQuadVertices(Object currObj);
	void   g2dSetPointVertices(Object currObj);

	void   g2dStart();
	void   g2dBeginCommon(Obj_Type type, g2dTexture *tex);
	
	void   g2dEndRects_DARR();
	void   g2dEndRects_VBO();
	void   g2dEndRects_PMVBO();

	void   g2dEndLines();
	void   g2dEndLines_DARR();
	void   g2dEndLines_VBO();
	void   g2dEndLines_PMVBO();

	void   g2dEndQuads();
	void   g2dEndQuads_DARR();
	void   g2dEndQuads_VBO();
	void   g2dEndQuads_PMVBO();
	
	void   g2dEndPoints();
	void   g2dEndPoints_DARR();
	void   g2dEndPoints_VBO();
	void   g2dEndPoints_PMVBO();

	GLuint posVBO;
	GLuint texVBO;
	GLuint colVBO;
	
	float *pVertexPosBufferData=nullptr;
	float *pVertexColBufferData=nullptr;
	float *pVertexTexBufferData=nullptr;

	float *pVertexPosCurrent=nullptr;
	float *pVertexTexCurrent=nullptr;
	float *pVertexColCurrent=nullptr;

	RenderMode m_renderMode = RM_VBO;

public:

	int maxRectangles	= 0;
	int bufferDataIndex = 0;
	int maxVertices		= 0;

	unsigned int maxTexVertices = 0;
	unsigned int maxColVertices = 0;
	unsigned int maxPosVertices = 0;

	unsigned int totalVertices = 0;

	bool m_takeSnapShot;


	RenderProvider();

	RenderProvider(const char* title);
	
	RenderProvider(const char* title, int width, int height);


	void generateVBOs();


	void takeSnapShot();

	void swithFullScreen();

	void init();

	void destroy();

	void g2dSwap();

	void getWindowSize(int *width, int *height);

	void setRenderMode(RenderMode renderMode);

	/**
	* \brief Clears screen & depth buffer.
	* @param color Screen clear color
	*
	* This function clears the screen, and clears the zbuffer if depth coordinate
	* is used in the loop. Will automatically init the GU if needed.
	*/
	void g2dClear(ImVec4 color);

	/**
	* \brief Clears depth buffer.
	*
	* This function clears the zbuffer to zero (z range 0-65535).
	* Will automatically init the GU if needed.
	*/
	void g2dClearZ();

	/**
	* \brief Begins rectangles rendering.
	* @param tex Pointer to a texture, pass NULL to get a colored rectangle.
	*
	* This function begins object rendering. Resets all properties.
	* One g2dAdd() call per object.
	* Only one texture can be used, but multiple objects can be rendered at a time.
	* g2dBegin*() / g2dEnd() couple can be called multiple times in the loop,
	* to render multiple textures.
	*/
	void g2dBeginRects(g2dTexture *tex);

	/**
	* \brief Begins lines rendering.
	* @param line_mode A g2dLine_Mode constant.
	*
	* This function begins object rendering. Calls g2dReset().
	* Two g2dAdd() calls per object.
	* Pass G2D_LINE_STRIP to make a line strip (two calls, then one per object).
	*/
	void g2dBeginLines(g2dLine_Mode mode);

	/**
	* \brief Begins quads rendering.
	* @param tex Pointer to a texture, pass NULL to get a colored quad.
	*
	* This function begins object rendering. Resets all properties.
	* Four g2dAdd() calls per object, first for the up left corner, then clockwise.
	* Only one texture can be used, but multiple objects can be rendered at a time.
	* g2dBegin*() / g2dEnd() couple can be called multiple times in the loop,
	* to render multiple textures.
	*/
	void g2dBeginQuads(g2dTexture *tex);

	/**
	* \brief Begins points rendering.
	*
	* This function begins object rendering. Resets all properties.
	* One g2dAdd() call per object.
	*/
	void g2dBeginPoints();

	/**
	* \brief Ends object rendering.
	*
	* This function ends object rendering. Must be called after g2dBegin*() to add
	* objects to the display list. Automatically adapts pspgu functionnalities
	* to get the best performance possible.
	*/
	void g2dEnd();

	/**
	* \brief Resets current transformation and attribution.
	*
	* This function must be called during object rendering.
	* Calls g2dResetCoord(), g2dResetRotation(), g2dResetScale(),
	* g2dResetColor(), g2dResetAlpha(), g2dResetCrop() and g2dResetTex().
	*/
	void g2dReset();

	/**
	* \brief Pushes the current transformation & attribution to a new object.
	*
	* This function must be called during object rendering.
	*/
	void g2dAdd();

	/**
	* \brief Saves the current transformation to stack.
	*
	* This function must be called during object rendering.
	* The stack is 64 saves high.
	* Use it like the OpenGL one.
	*/
	void g2dPush();

	/**
	* \brief Restore the current transformation from stack.
	*
	* This function must be called during object rendering.
	* The stack is 64 saves high.
	* Use it like the OpenGL one.
	*/
	void g2dPop();

	/**
	* \brief Resets the current coordinates.
	*
	* This function must be called during object rendering.
	* Sets g2dSetCoordMode() to G2D_UP_LEFT and g2dSetCoordXYZ() to (0,0,0).
	*/
	void g2dResetCoord();

	/**
	* \brief Set coordinate mode.
	* @param coord_mode A gCoord_Mode.
	*
	* This function must be called during object rendering.
	* Defines where the coordinates correspond in the object.
	*/
	void g2dSetCoordMode(g2dCoord_Mode mode);

	/**
	* \brief Gets the current position.
	* @param x Pointer to save the current x (in pixels).
	* @param y Pointer to save the current y (in pixels).
	* @param z Pointer to save the current z (in pixels).
	*
	* This function must be called during object rendering.
	* Parameters are pointers to float, not int !
	* Pass NULL if not needed.
	*/
	void g2dGetCoordXYZ(float *x, float *y, float *z);

	/**
	* \brief Sets the new position.
	* @param x New x, in pixels.
	* @param y New y, in pixels.
	*
	* This function must be called during object rendering.
	*/
	void g2dSetCoordXY(float x, float y);

	/**
	* \brief Sets the new position, with depth support.
	* @param x New x, in pixels.
	* @param y New y, in pixels.
	* @param z New z, in pixels. (front 0-65535 back)
	*
	* This function must be called during object rendering.
	*/
	void g2dSetCoordXYZ(float x, float y, float z);

	/**
	* \brief Sets the new position, relative to the current.
	* @param x New x increment, in pixels.
	* @param y New y increment, in pixels.
	*
	* This function must be called during object rendering.
	*/
	void g2dSetCoordXYRelative(float x, float y);

	/**
	* \brief Sets the new position, with depth support, relative to the current.
	* @param x New x increment, in pixels.
	* @param y New y increment, in pixels.
	* @param z New z increment, in pixels.
	*
	* This function must be called during object rendering.
	*/
	void g2dSetCoordXYZRelative(float x, float y, float z);

	/**
	* \brief Use integer coordinates.
	* @param use false to desactivate (better look, by default),
	true to activate (can be useful when you have glitches).
	*
	* This function must be called during object rendering.
	*/
	void g2dSetCoordInteger(bool use);

	/**
	* \brief Resets the global scale.
	*
	* This function resets the global scale to 1.f.
	* Translations and scales are multiplied by this factor.
	*/
	void g2dResetGlobalScale();

	/**
	* \brief Resets the current scale.
	*
	* This function must be called during object rendering.
	* Sets the scale to the current texture size or (10,10).
	*/
	void g2dResetScale();

	/**
	* \brief Gets the global scale.
	* @param scale Pointer to save the global scale (factor).
	*
	* Pass NULL if not needed.
	*/
	void g2dGetGlobalScale(float *scale);

	/**
	* \brief Gets the current scale.
	* @param w Pointer to save the current width (in pixels).
	* @param h Pointer to save the current height (in pixels).
	*
	* This function must be called during object rendering.
	* Parameters are pointers to float, not int !
	* Pass NULL if not needed.
	*/
	void g2dGetScaleWH(float *w, float *h);

	/**
	* \brief Sets the global scale.
	*
	* Translations and scales are multiplied by this factor.
	*/
	void g2dSetGlobalScale(float scale);

	/**
	* \brief Sets the new scale.
	* @param w Width scale factor.
	* @param h Height scale factor.
	*
	* This function must be called during object rendering.
	* g2dResetScale() is called, then width & height scale are
	* multiplied by these values.
	* Negative values can be passed to invert the texture.
	*/
	void g2dSetScale(float w, float h);

	/**
	* \brief Sets the new scale, in pixels.
	* @param w New width, in pixels.
	* @param h New height, in pixels.
	*
	* This function must be called during object rendering.
	* Negative values can be passed to invert the texture.
	*/
	void g2dSetScaleWH(float w, float h);

	/**
	* \brief Sets the new scale, relative to the current.
	* @param w Width scale factor.
	* @param h Height scale factor.
	*
	* This function must be called during object rendering.
	* Current width & height scale are multiplied by these values.
	* Negative values can be passed to invert the texture.
	*/
	void g2dSetScaleRelative(float w, float h);

	/**
	* \brief Sets the new scale, in pixels, relative to the current.
	* @param w New width to increment, in pixels.
	* @param h New height to increment, in pixels.
	*
	* This function must be called during object rendering.
	* Negative values can be passed to invert the texture.
	*/
	void g2dSetScaleWHRelative(float w, float h);

	/**
	* \brief Resets the current color.
	*
	* This function must be called during object rendering.
	* Sets g2dSetColor() to WHITE.
	*/
	void g2dResetColor();

	/**
	* \brief Resets the current alpha.
	*
	* This function must be called during object rendering.
	* Sets g2dSetAlpha() to 255.
	*/
	void g2dResetAlpha();

	/**
	* \brief Gets the current alpha.
	* @param alpha Pointer to save the current alpha (0-255).
	*
	* This function must be called during object rendering.
	* Pass NULL if not needed.
	*/
	void g2dGetAlpha(g2dAlpha *alpha);

	/**
	* \brief Sets the new color.
	* @param color The new color.
	*
	* This function must be called during object rendering.
	* Can be used to colorize any object.
	*/
	void g2dSetColor(g2dColor color);

	/**
	* \brief Sets the new alpha.
	* @param alpha The new alpha (0-255).
	*
	* This function must be called during object rendering.
	* Can be used to make any object transparent.
	*/
	void g2dSetAlpha(g2dAlpha alpha);

	/**
	* \brief Sets the new alpha, relative to the current alpha.
	* @param alpha The new alpha increment.
	*
	* This function must be called during object rendering.
	* Can be used to make any object transparent.
	*/
	void g2dSetAlphaRelative(int alpha);

	/**
	* \brief Resets the current rotation.
	*
	* This function must be called during object rendering.
	* Sets g2dSetRotation() to 0?a.
	*/
	void g2dResetRotation();

	/**
	* \brief Gets the current rotation, in radians.
	* @param radians Pointer to save the current rotation.
	*
	* This function must be called during object rendering.
	* Pass NULL if not needed.
	*/
	void g2dGetRotationRad(float *radians);

	/**
	* \brief Gets the current rotation, in degrees.
	* @param degrees Pointer to save the current rotation.
	*
	* This function must be called during object rendering.
	* Pass NULL if not needed.
	*/
	void g2dGetRotation(float *degrees);

	/**
	* \brief Sets the new rotation, in radians.
	* @param radians The new angle.
	*
	* This function must be called during object rendering.
	* The rotation center is the actual coordinates.
	*/
	void g2dSetRotationRad(float radians);

	/**
	* \brief Sets the new rotation, in degrees.
	* @param degrees The new angle.
	*
	* This function must be called during object rendering.
	* The rotation center is the actual coordinates.
	*/
	void g2dSetRotation(float degrees);

	/**
	* \brief Sets the new rotation, relative to the current, in radians.
	* @param radians The new angle increment.
	*
	* This function must be called during object rendering.
	* The rotation center is the actual coordinates.
	*/
	void g2dSetRotationRadRelative(float radians);

	/**
	* \brief Sets the new rotation, relative to the current, in degrees.
	* @param degrees The new angle increment.
	*
	* This function must be called during object rendering.
	* The rotation center is the actual coordinates.
	*/
	void g2dSetRotationRelative(float degrees);

	/**
	* \brief Resets the current crop.
	*
	* This function must be called during object rendering.
	* Sets g2dSetCropXY() to (0;0) and g2dSetCropWH() to (tex->w,tex->h).
	*/
	void g2dResetCrop();

	/**
	* \brief Gets the current crop position.
	* @param x Pointer to save the current crop x.
	* @param y Pointer to save the current crop y.
	*
	* This function must be called during object rendering.
	* Pass NULL if not needed.
	*/
	void g2dGetCropXY(int *x, int *y);

	/**
	* \brief Gets the current crop scale.
	* @param w Pointer to save the current crop width.
	* @param h Pointer to save the current crop height.
	*
	* This function must be called during object rendering.
	* Pass NULL if not needed.
	*/
	void g2dGetCropWH(int *w, int *h);

	/**
	* \brief Sets the new crop position.
	* @param x New x, in pixels.
	* @param y New y, in pixels.
	*
	* This function must be called during object rendering. Defines crop position.
	* If the rectangle is larger or next to the texture, it will be repeated
	* when g2dSetTexRepeat is enabled. Useful for a tileset.
	*/
	void g2dSetCropXY(int x, int y);

	/**
	* \brief Sets the new crop size.
	* @param w New width, in pixels.
	* @param h New height, in pixels.
	*
	* This function must be called during object rendering. Defines crop size.
	* If the rectangle is larger or next to the texture, it will be repeated
	* when g2dSetTexRepeat is enabled. Useful for a tileset.
	*/
	void g2dSetCropWH(int w, int h);

	/**
	* \brief Sets the new crop position, relative to the current.
	* @param x New x increment, in pixels.
	* @param y New y increment, in pixels.
	*
	* This function must be called during object rendering. Defines crop position.
	* If the rectangle is larger or next to the texture, texture will be repeated
	* when g2dSetTexRepeat is enabled. Useful for a tileset.
	*/
	void g2dSetCropXYRelative(int x, int y);

	/**
	* \brief Sets the new crop size, relative to the current.
	* @param w New width increment, in pixels.
	* @param h New height increment, in pixels.
	*
	* This function must be called during object rendering. Defines crop size.
	* If the rectangle is larger or next to the texture, texture will be repeated
	* when g2dSetTexRepeat is enabled. Useful for a tileset.
	*/
	void g2dSetCropWHRelative(int w, int h);

	/**
	* \brief Resets texture properties.
	*
	* This function must be called during object rendering.
	*/
	void g2dResetTex();

	/**
	* \brief Set texture wrap.
	* @param use true to repeat, false to clamp (by default).
	*
	* This function must be called during object rendering.
	*/
	void g2dSetTexRepeat(bool use);

	/**
	* \brief Use the bilinear filter with the texture.
	* @param use true to activate (better look, by default).
	false to desactivate (better performance).
	*
	* This function must be called during object rendering.
	* Only useful when scaling.
	*/
	void g2dSetTexLinear(bool use);

	/**
	* \brief Resets the draw zone to the entire screen.
	*
	* This function can be called everywhere in the loop.
	*/
	void g2dResetScissor();

	/**
	* \brief Sets the draw zone.
	* @param x New x position.
	* @param y New y position.
	* @param w New width.
	* @param h New height.
	*
	* This function can be called everywhere in the loop.
	* Pixel draw will be skipped outside this rectangle.
	*/

	void g2dSetScissor(int x, int y, int w, int h);

	void static error_callback(int error, const char* description)
	{
		printf("%s", description);
		fputs(description, stderr);
	}

	void static framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void static windowSizeCB(GLFWwindow* window, int width, int height)
	{
		// Callback function called by GLFW when window size changes

		printf("Window resized. New resolution [%dx%d] \n", width, height);

		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();

		glOrtho(0.0f, width, height, 0.0f, -1000.0f, +1000.0f);

		glViewport(0, 0, width, height);

	}

	void static windowclose_callback(GLFWwindow* window)
	{
		exit(0);
	}

};

#endif
