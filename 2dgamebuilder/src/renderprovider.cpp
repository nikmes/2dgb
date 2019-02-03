#include "servicelocator.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <fstream>
#include <stdlib.h>
#include <immintrin.h>

#define SPRITE_COUNT 20000

bool		  RenderProvider::m_init	= true;
bool		  RenderProvider::m_start	= false;
bool		  RenderProvider::m_gbegin	= false;
bool		  RenderProvider::m_zclear	= true;
bool		  RenderProvider::m_scissor	= false;

float		  RenderProvider::m_global_scale = 1.0;
unsigned int  RenderProvider::m_tstack_size = 0;
Transform	  RenderProvider::m_tstack[TSTACK_MAX];
RenderContext RenderProvider::m_rctx;


RenderProvider::RenderProvider()
{
	// generate all necessary VBOs

	switch (m_renderMode)
	{
	case RM_DARRAY:
		TRACE(T_DEB, "Do not Initialize VBOs since we using Drawing Arrays");
		break;
	case RM_VBO:
		generateVBOs();
		break;
	case RM_PMVBO:
		generateVBOs();
		break;
	default:
		generateVBOs();
		break;
	}
}

RenderProvider::RenderProvider(const char* title)
{
	/*
	* Initialize the Vertex Buffer Objects
	*/

	m_takeSnapShot = false;

	m_fullScreen = true;

	this->init();

	m_title = title;

	m_scrWidth = ServiceLocator::getConfigurationService()->pVideo_resolution.x;

	m_scrHeight = ServiceLocator::getConfigurationService()->pVideo_resolution.y;

	switch (m_renderMode)
	{
	case RM_DARRAY:
		TRACE(T_DEB, "Do not Initialize VBOs since we using Drawing Arrays");
		break;
	case RM_VBO:
		generateVBOs();
		break;
	case RM_PMVBO:
		generateVBOs();
		break;
	default:
		generateVBOs();
		break;
	}
}

RenderProvider::RenderProvider(const char* title, int width, int height)
{
	/*
	* Initialize the Vertex Buffer Objects
	*/

	m_title = title;

	m_scrWidth = width;

	m_scrHeight = height;

	m_fullScreen = false;

	this->init();

	switch (m_renderMode)
	{
	case RM_DARRAY:
		TRACE(T_DEB,"Do not Initialize VBOs since we using Drawing Arrays");
		break;
	case RM_VBO:
		generateVBOs();
		break;
	case RM_PMVBO:
		generateVBOs();
		break;
	default:
		generateVBOs();
		break;
	}
	
}

void RenderProvider::setRenderMode(RenderMode renderMode)
{
	m_renderMode = renderMode;
}

void RenderProvider::generateVBOs()
{
	size_t vpSize = SPRITE_COUNT * (sizeof(float) * (6 * 3) );	// calculate the size needed to store position floats ( 2 triangles x 3 points x 3 floats each point x,y,z)
	size_t vtSize = SPRITE_COUNT * (sizeof(float) * (6 * 2) );  // calculate the size needed to store texture floats (2 triangles x 3 points x 2 floats each uv1,uv2)
	size_t vcSize = SPRITE_COUNT * (sizeof(float) * (6 * 4) );  // calculate the size needed to store color floats (2 triangles x 3 points x 4 floats each point r,g,b,a)

	pVertexPosBufferData = (float*)malloc(vpSize);				// allocate enough memmory in RAM to store the Vertices that hold position information
	pVertexPosCurrent	 = pVertexPosBufferData;					
																
	pVertexTexBufferData = (float*)malloc(vtSize);				// allocate enough memmory in RAM to store the vertices that hold texture mapping information
	pVertexTexCurrent	 = pVertexTexBufferData;					
																
	pVertexColBufferData = (float*)malloc(vcSize);				// allocate enough memmory in RAM to store the vertices that hold color information
	pVertexColCurrent	 = pVertexColBufferData;					
																
	glGenBuffers(1, &posVBO);									// to store position vertices (x,y,z)

	glGenBuffers(1, &texVBO);									// to store textre UV coordinates (x,y)
	
	glGenBuffers(1, &colVBO);									// to store color information (r,g,b,a)

	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, vpSize, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, vtSize, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colVBO);
	glBufferData(GL_ARRAY_BUFFER, vcSize, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderProvider::destroy()
{
	free(pVertexPosBufferData);
	free(pVertexTexBufferData);
	free(pVertexColBufferData);

	glDeleteBuffers(1, &posVBO);
	glDeleteBuffers(1, &texVBO);
	glDeleteBuffers(1, &colVBO);
}

void RenderProvider::swithFullScreen()
{
	int vSyncMode = 0;

	if (!m_fullScreen)
	{
		const GLFWvidmode *vidmode;

		int count, w, h;

		glfwGetWindowSize(gWin, &w, &h);

		ServiceLocator::getConfigurationService()->pVideo_resolution.x = w;

		ServiceLocator::getConfigurationService()->pVideo_resolution.y = h;

		GLFWmonitor** monitors = glfwGetMonitors(&count);

		vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		m_scrWidth = vidmode->width;

		m_scrHeight = vidmode->height;

		GLFWwindow *newWindow = glfwCreateWindow(vidmode->width, vidmode->height, m_title.c_str(), glfwGetPrimaryMonitor(), gWin);

		glfwDestroyWindow(gWin);

		gWin = newWindow;

		m_fullScreen = true;

		ServiceLocator::getConfigurationService()->pVideo_fullscreen = true;
	}
	else
	{
		m_scrWidth = ServiceLocator::getConfigurationService()->pVideo_resolution.x;

		m_scrHeight = ServiceLocator::getConfigurationService()->pVideo_resolution.y;

		GLFWwindow *newWindow = glfwCreateWindow(m_scrWidth, m_scrHeight, m_title.c_str(), NULL, gWin);

		glfwDestroyWindow(gWin);

		gWin = newWindow;

		m_fullScreen = false;

		ServiceLocator::getConfigurationService()->pVideo_fullscreen = false;

	}

	if (!gWin)
	{
		TRACE(T_DEB, "Error window not created");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	int width, height;

	glfwSetFramebufferSizeCallback(gWin, framebuffer_size_callback);

	glfwMakeContextCurrent(gWin);

	vSyncMode = ServiceLocator::getConfigurationService()->pOpenGlVsync;

	if (vSyncMode == 1)
	{
		glfwSwapInterval(1); // 1 - enable vsync 60
	}

	if (vSyncMode == 2)
	{
		glfwSwapInterval(2); // 2 - enable vsync 30 - disable vsync
	}

	if (vSyncMode == 0)
	{
		glfwSwapInterval(0); // 2 - disable vsync
	}

	glfwSetWindowSizeCallback(gWin, windowSizeCB); // - Redirect window size changes to the callback function windowSizeCB

	glfwSetWindowCloseCallback(gWin, windowclose_callback);

	glfwSetErrorCallback(error_callback);

	glfwGetWindowSize(gWin, &width, &height);

	glewInit();

	// Setup OpenGL

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glOrtho(0.0f, width, height, 0, -1000.0f, +1000.0f);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_MODELVIEW);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_BLEND);

	glEnable(GL_SCISSOR_TEST);

	glDisable(GL_DITHER);

	glDisable(GL_LIGHTING);

	glAlphaFunc(GL_GREATER, 0.1);

	glEnable(GL_ALPHA_TEST);

	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);

	glDepthRangef(0, 65535);

	glClearDepthf(65535);

	g2dSetGlobalScale(1.0);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	ServiceLocator::getInputService()->init();
}

void RenderProvider::getWindowSize(int *width, int *height)
{
	glfwGetWindowSize(gWin, width, height);
}

void RenderProvider::takeSnapShot()
{
	/*
	* makes a screenshot of the editor and saves it in file
	*/
	static int id = 0;
	int width, height;
	char outName[256];
	sprintf(outName, "wonderboy_%d.png", id);
	getWindowSize(&width, &height);

	unsigned char* pixels = new unsigned char[3 * width * height];
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	unsigned char* line_tmp = new unsigned char[3 * width];
	unsigned char* line_a = pixels;
	unsigned char* line_b = pixels + (3 * width * (height - 1));
	while (line_a < line_b)
	{
		memcpy(line_tmp, line_a, width * 3);
		memcpy(line_a, line_b, width * 3);
		memcpy(line_b, line_tmp, width * 3);
		line_a += width * 3;
		line_b -= width * 3;
	}

	stbi_write_png(outName, width, height, 3, pixels, width * 3);
	id++;
	delete[] pixels;
	delete[] line_tmp;


}

void RenderProvider::init()
{
	int vSyncMode = 0;

	const GLFWvidmode *vidmode;

	int width, height;

	if (!glfwInit())
	{
		TRACE(T_DEB, "Fatal error during GLFW3 intialization");

		exit(EXIT_FAILURE);
	}

	if (m_fullScreen)
	{
		int count;

		GLFWmonitor** monitors = glfwGetMonitors(&count);

		vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		GLFWwindow *newWindow = glfwCreateWindow(vidmode->width, vidmode->height, "", glfwGetPrimaryMonitor(), gWin);

		glfwDestroyWindow(gWin);

		gWin = newWindow;
	}
	else
	{
		GLFWwindow *newWindow = glfwCreateWindow(m_scrWidth, m_scrHeight, m_title.c_str(), NULL, gWin);

		glfwDestroyWindow(gWin);

		gWin = newWindow;
	}

	if (!gWin)
	{
		TRACE(T_DEB, "Error window not created");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetFramebufferSizeCallback(gWin, framebuffer_size_callback);

	glfwMakeContextCurrent(gWin);

	vSyncMode = ServiceLocator::getConfigurationService()->pOpenGlVsync;

	if (vSyncMode == 1)
	{
		glfwSwapInterval(1); // 1 - enable vsync 60
	}

	if (vSyncMode == 2)
	{
		glfwSwapInterval(2); // 2 - enable vsync 30 - disable vsync
	}

	if (vSyncMode == 0)
	{
		glfwSwapInterval(0); // 2 - disable vsync
	}

	glfwSetWindowSizeCallback(gWin, windowSizeCB); // - Redirect window size changes to the callback function windowSizeCB

	glfwSetWindowCloseCallback(gWin, windowclose_callback);

	glfwSetErrorCallback(error_callback);

	glfwGetWindowSize(gWin, &width, &height);

	glewInit();

	// Setup OpenGL

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glOrtho(0.0f, width, height, 0, -1000.0f, +1000.0f);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_MODELVIEW);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_BLEND);

	glEnable(GL_SCISSOR_TEST);

	glDisable(GL_DITHER);

	glDisable(GL_LIGHTING);

	glAlphaFunc(GL_GREATER, 0.1);

	glEnable(GL_ALPHA_TEST);

	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);

	glDepthRangef(0, 65535);

	glClearDepthf(65535);

	g2dSetGlobalScale(1.0);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void RenderProvider::g2dStart()
{
	m_start = true;
}


float* RenderProvider::g2dSetVertex(float *vp, int i, float vx, float vy)
{

	// Vertex order: [texture uv] [color] [coord]

	// A Prepare the texture vertices information
	if (m_rctx.tex != NULL)
	{
		vp[0] = (float)(m_rctx.obj[i].crop_x + vx*m_rctx.obj[i].crop_w) / m_rctx.tex->tw;
		vp[1] = (float)(m_rctx.obj[i].crop_y + vy*m_rctx.obj[i].crop_h) / m_rctx.tex->th;
	}
	else
	{
		vp[0] = 0;
		vp[1] = 0;
	}

	// A Prepare the color vertices information
	vp[2] = G2D_GET_R(m_rctx.obj[i].color) / 255.;
	vp[3] = G2D_GET_G(m_rctx.obj[i].color) / 255.;
	vp[4] = G2D_GET_B(m_rctx.obj[i].color) / 255.;
	vp[5] = G2D_GET_A(m_rctx.obj[i].color) / 255.;

	// A Prepare the position vertices information
	vp[6] = m_rctx.obj[i].x;
	vp[7] = m_rctx.obj[i].y;
	vp[8] = m_rctx.obj[i].z;

	if (m_rctx.type == RECTS)
	{
		vp[6] += vx * m_rctx.obj[i].scale_w;
		vp[7] += vy * m_rctx.obj[i].scale_h;

		if (m_rctx.use_rot) // Apply a rotation
		{
			float tx = vp[6] - m_rctx.obj[i].rot_x;
			float ty = vp[7] - m_rctx.obj[i].rot_y;

			vp[6] = m_rctx.obj[i].rot_x - m_rctx.obj[i].rot_sin*ty + m_rctx.obj[i].rot_cos*tx,
			vp[7] = m_rctx.obj[i].rot_y + m_rctx.obj[i].rot_cos*ty + m_rctx.obj[i].rot_sin*tx;
		}
	}

	if (m_rctx.use_int) // Pixel perfect
	{
		vp[6] = floorf(vp[6]);
		vp[7] = floorf(vp[7]);
	}

	return (vp + 9);	
}

void RenderProvider::g2dSetRectVertices(Object currObj)
{
	if (m_rctx.tex != NULL)
	{
		pVertexTexCurrent[0]  = (float)(currObj.crop_x + 1.0*currObj.crop_w) / m_rctx.tex->tw;
		pVertexTexCurrent[1]  = (float)(currObj.crop_y + 0.0*currObj.crop_h) / m_rctx.tex->th;
		pVertexTexCurrent[2]  = (float)(currObj.crop_x + 0.0*currObj.crop_w) / m_rctx.tex->tw;
		pVertexTexCurrent[3]  = (float)(currObj.crop_y + 1.0*currObj.crop_h) / m_rctx.tex->th;
		pVertexTexCurrent[4]  = (float)(currObj.crop_x + 0.0*currObj.crop_w) / m_rctx.tex->tw;
		pVertexTexCurrent[5]  = (float)(currObj.crop_y + 0.0*currObj.crop_h) / m_rctx.tex->th;

		pVertexTexCurrent[6]  = (float)(currObj.crop_x + 0.0*currObj.crop_w) / m_rctx.tex->tw;
		pVertexTexCurrent[7]  = (float)(currObj.crop_y + 1.0*currObj.crop_h) / m_rctx.tex->th;
		pVertexTexCurrent[8]  = (float)(currObj.crop_x + 1.0*currObj.crop_w) / m_rctx.tex->tw;
		pVertexTexCurrent[9]  = (float)(currObj.crop_y + 0.0*currObj.crop_h) / m_rctx.tex->th;
		pVertexTexCurrent[10] = (float)(currObj.crop_x + 1.0*currObj.crop_w) / m_rctx.tex->tw;
		pVertexTexCurrent[11] = (float)(currObj.crop_y + 1.0*currObj.crop_h) / m_rctx.tex->th;
	}
	else
	{
		pVertexTexCurrent[0]  = 0.0;
		pVertexTexCurrent[1]  = 0.0;
		pVertexTexCurrent[2]  = 0.0;
		pVertexTexCurrent[3]  = 0.0;
		pVertexTexCurrent[4]  = 0.0;
		pVertexTexCurrent[5]  = 0.0;

		pVertexTexCurrent[6]  = 0.0;
		pVertexTexCurrent[7]  = 0.0;
		pVertexTexCurrent[8]  = 0.0;
		pVertexTexCurrent[9]  = 0.0;
		pVertexTexCurrent[10] = 0.0;
		pVertexTexCurrent[11] = 0.0;
	}
	maxVertices += 12;

	// A Prepare the color vertices information in color buffer
	pVertexColCurrent[0] = G2D_GET_R(currObj.color) / 255.;
	pVertexColCurrent[1] = G2D_GET_G(currObj.color) / 255.;
	pVertexColCurrent[2] = G2D_GET_B(currObj.color) / 255.;
	pVertexColCurrent[3] = G2D_GET_A(currObj.color) / 255.;
	pVertexColCurrent[4] = G2D_GET_R(currObj.color) / 255.;
	pVertexColCurrent[5] = G2D_GET_G(currObj.color) / 255.;
	pVertexColCurrent[6] = G2D_GET_B(currObj.color) / 255.;
	pVertexColCurrent[7] = G2D_GET_A(currObj.color) / 255.;
	pVertexColCurrent[8] = G2D_GET_R(currObj.color) / 255.;
	pVertexColCurrent[9] = G2D_GET_G(currObj.color) / 255.;
	pVertexColCurrent[10] = G2D_GET_B(currObj.color) / 255.;
	pVertexColCurrent[11] = G2D_GET_A(currObj.color) / 255.;

	pVertexColCurrent[12] = G2D_GET_R(currObj.color) / 255.;
	pVertexColCurrent[13] = G2D_GET_G(currObj.color) / 255.;
	pVertexColCurrent[14] = G2D_GET_B(currObj.color) / 255.;
	pVertexColCurrent[15] = G2D_GET_A(currObj.color) / 255.;
	pVertexColCurrent[16] = G2D_GET_R(currObj.color) / 255.;
	pVertexColCurrent[17] = G2D_GET_G(currObj.color) / 255.;
	pVertexColCurrent[18] = G2D_GET_B(currObj.color) / 255.;
	pVertexColCurrent[19] = G2D_GET_A(currObj.color) / 255.;
	pVertexColCurrent[20] = G2D_GET_R(currObj.color) / 255.;
	pVertexColCurrent[21] = G2D_GET_G(currObj.color) / 255.;
	pVertexColCurrent[22] = G2D_GET_B(currObj.color) / 255.;
	pVertexColCurrent[23] = G2D_GET_A(currObj.color) / 255.;

	maxVertices += 24;

	// A Prepare the position vertices information in position buffer
	pVertexPosCurrent[0]  = currObj.x;
	pVertexPosCurrent[1]  = currObj.y;
	pVertexPosCurrent[2]  = currObj.z;
	pVertexPosCurrent[3]  = currObj.x;
	pVertexPosCurrent[4]  = currObj.y;
	pVertexPosCurrent[5]  = currObj.z;
	pVertexPosCurrent[6]  = currObj.x;
	pVertexPosCurrent[7]  = currObj.y;
	pVertexPosCurrent[8]  = currObj.z;

	pVertexPosCurrent[9]  = currObj.x;
	pVertexPosCurrent[10] = currObj.y;
	pVertexPosCurrent[11] = currObj.z;
	pVertexPosCurrent[12] = currObj.x;
	pVertexPosCurrent[13] = currObj.y;
	pVertexPosCurrent[14] = currObj.z;
	pVertexPosCurrent[15] = currObj.x;
	pVertexPosCurrent[16] = currObj.y;
	pVertexPosCurrent[17] = currObj.z;

	maxVertices += 18;

	if (m_rctx.type == RECTS)
	{
		pVertexPosCurrent[0]  += 1.0 * currObj.scale_w;
		pVertexPosCurrent[1]  += 0.0 * currObj.scale_h;
							  
		pVertexPosCurrent[3]  += 0.0 * currObj.scale_w;
		pVertexPosCurrent[4]  += 1.0 * currObj.scale_h;
							  
		pVertexPosCurrent[6]  += 0.0 * currObj.scale_w;
		pVertexPosCurrent[7]  += 0.0 * currObj.scale_h;


		pVertexPosCurrent[9]  += 0.0 * currObj.scale_w;
		pVertexPosCurrent[10] += 1.0 * currObj.scale_h;

		pVertexPosCurrent[12] += 1.0 * currObj.scale_w;
		pVertexPosCurrent[13] += 0.0 * currObj.scale_h;

		pVertexPosCurrent[15] += 1.0 * currObj.scale_w;
		pVertexPosCurrent[16] += 1.0 * currObj.scale_h;

		if (m_rctx.use_rot) // Apply a rotation
		{
			float tx1 = (pVertexPosCurrent[0]  - currObj.rot_x);
			float ty1 = (pVertexPosCurrent[1]  - currObj.rot_y);
			float tx2 = (pVertexPosCurrent[3]  - currObj.rot_x);
			float ty2 = (pVertexPosCurrent[4]  - currObj.rot_y);
			float tx3 = (pVertexPosCurrent[6]  - currObj.rot_x);
			float ty3 = (pVertexPosCurrent[7]  - currObj.rot_y);

			float tx4 = (pVertexPosCurrent[9]  - currObj.rot_x);
			float ty4 = (pVertexPosCurrent[10] - currObj.rot_y);
			float tx5 = (pVertexPosCurrent[12] - currObj.rot_x);
			float ty5 = (pVertexPosCurrent[13] - currObj.rot_y);
			float tx6 = (pVertexPosCurrent[15] - currObj.rot_x);
			float ty6 = (pVertexPosCurrent[16] - currObj.rot_y);


			pVertexPosCurrent[0]  = currObj.rot_x - currObj.rot_sin*ty1 + currObj.rot_cos*tx1;
			pVertexPosCurrent[1]  = currObj.rot_y + currObj.rot_cos*ty1 + currObj.rot_sin*tx1;
								  
			pVertexPosCurrent[3]  = currObj.rot_x - currObj.rot_sin*ty2 + currObj.rot_cos*tx2;
			pVertexPosCurrent[4]  = currObj.rot_y + currObj.rot_cos*ty2 + currObj.rot_sin*tx2;
								  
			pVertexPosCurrent[6]  = currObj.rot_x - currObj.rot_sin*ty3 + currObj.rot_cos*tx3;
			pVertexPosCurrent[7]  = currObj.rot_y + currObj.rot_cos*ty3 + currObj.rot_sin*tx3;


			pVertexPosCurrent[9]  = currObj.rot_x - currObj.rot_sin*ty4 + currObj.rot_cos*tx4;
			pVertexPosCurrent[10] = currObj.rot_y + currObj.rot_cos*ty4 + currObj.rot_sin*tx4;

			pVertexPosCurrent[12] = currObj.rot_x - currObj.rot_sin*ty5 + currObj.rot_cos*tx5;
			pVertexPosCurrent[13] = currObj.rot_y + currObj.rot_cos*ty5 + currObj.rot_sin*tx5;

			pVertexPosCurrent[15] = currObj.rot_x - currObj.rot_sin*ty6 + currObj.rot_cos*tx6;
			pVertexPosCurrent[16] = currObj.rot_y + currObj.rot_cos*ty6 + currObj.rot_sin*tx6;
		}
	}

	if (m_rctx.use_int) // Pixel perfect
	{

		int x = floorf(pVertexPosCurrent[0]);
		int y = floorf(pVertexPosCurrent[1]);

		pVertexPosCurrent[0] = x;
		pVertexPosCurrent[1] = y;
		pVertexPosCurrent[3] = x;
		pVertexPosCurrent[4] = y;
		pVertexPosCurrent[6] = x;
		pVertexPosCurrent[7] = y;

		pVertexPosCurrent[9] = x;
		pVertexPosCurrent[10] = y;
		pVertexPosCurrent[12] = x;
		pVertexPosCurrent[13] = y;
		pVertexPosCurrent[15] = x;
		pVertexPosCurrent[16] = y;
	}

	pVertexPosCurrent = (float*)((char*)pVertexPosCurrent + (sizeof(float) * 18));
	pVertexTexCurrent = (float*)((char*)pVertexTexCurrent + (sizeof(float) * 12));
	pVertexColCurrent = (float*)((char*)pVertexColCurrent + (sizeof(float) * 24));

	++bufferDataIndex;
}

void RenderProvider::g2dSetLineVertices(Object currObj)
{

}

void RenderProvider::g2dSetQuadVertices(Object currObj)
{

}

void RenderProvider::g2dSetPointVertices(Object currObj)
{

}


void RenderProvider::g2dClear(ImVec4 color)
{
	glClearColor(color.x, color.y, color.z, color.w); 

	glClear(GL_COLOR_BUFFER_BIT); // | (m_zclear ? GL_DEPTH_BUFFER_BIT : 0));

	glClear(GL_DEPTH_BUFFER_BIT);

	m_zclear = true;
}

void RenderProvider::g2dClearZ()
{
	if (!m_start)
	{
		g2dStart();
	}

	glClear(GL_DEPTH_BUFFER_BIT);

	m_zclear = true;
}


void RenderProvider::g2dBeginCommon(Obj_Type type, g2dTexture *tex)
{
	if (m_gbegin)
		return;

	if (!m_start)
		g2dStart();
	
	// Reset render context
	m_rctx.obj = (Object *)realloc(m_rctx.obj, MALLOC_STEP * sizeof(Object));
	m_rctx.n = 0;
	m_rctx.type = type;
	m_rctx.tex = tex;
	m_rctx.use_strip = false;
	m_rctx.use_z = false;
	m_rctx.use_vert_color = false;
	m_rctx.use_rot = false;
	m_rctx.use_tex_linear = true;
	m_rctx.use_tex_repeat = false;
	m_rctx.use_int = false;
	m_rctx.color_count = 0;
	m_rctx.coord_mode = DEFAULT_COORD_MODE;

	// Reset current object
	g2dReset();
	m_gbegin = true;

	maxRectangles = 0;
	maxVertices = 0;
}

void RenderProvider::g2dBeginRects(g2dTexture *tex)
{
	g2dBeginCommon(RECTS, tex);
}

void RenderProvider::g2dBeginLines(g2dLine_Mode mode)
{
	g2dBeginCommon(LINES, NULL);

	m_rctx.use_strip = (mode & G2D_STRIP);
}

void RenderProvider::g2dBeginQuads(g2dTexture *tex)
{
	g2dBeginCommon(QUADS, tex);
}

void RenderProvider::g2dBeginPoints()
{
	g2dBeginCommon(POINTSS, NULL);
}


void RenderProvider::g2dEndRects_DARR()
{
	int v_prim = GL_TRIANGLES;
	int v_obj_nbr = 6;
	int v_nbr = v_obj_nbr * m_rctx.n;
	int v_size = 9 * sizeof(float);

	// Allocate vertex list memory
	float *v = (float*)malloc(v_nbr * v_size);
	float *vi = v;

	// Add each object	
	for (int i = 0; i<m_rctx.n; i += 1)
	{
		vi = g2dSetVertex(vi, i, 0.f, 0.f); //
		vi = g2dSetVertex(vi, i, 1.f, 0.f); // 1st triangle
		vi = g2dSetVertex(vi, i, 0.f, 1.f); //

		vi = g2dSetVertex(vi, i, 0.f, 1.f); //
		vi = g2dSetVertex(vi, i, 1.f, 0.f); // 2nd triangle
		vi = g2dSetVertex(vi, i, 1.f, 1.f); // 
	}

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, v_size, v);
	glColorPointer(4, GL_FLOAT, v_size, v + 2);
	glVertexPointer(3, GL_FLOAT, v_size, v + 6);

	glDrawArrays(v_prim, 0, v_nbr);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	free(v);
}

void RenderProvider::g2dEndRects_VBO()
{
	int v_prim = GL_TRIANGLES;
	int v_obj_nbr = 6;
	int v_nbr = v_obj_nbr * m_rctx.n;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * m_rctx.n * 6 * 3, pVertexPosBufferData);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * m_rctx.n * 6 * 2, pVertexTexBufferData);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, colVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * m_rctx.n * 6 * 4, pVertexColBufferData);
	glColorPointer(4, GL_FLOAT, 0, 0);

	glDrawArrays(v_prim, 0, (GLsizei)(bufferDataIndex * 6)); 

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	pVertexPosCurrent = pVertexPosBufferData;
	pVertexTexCurrent = pVertexTexBufferData;
	pVertexColCurrent = pVertexColBufferData;

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void RenderProvider::g2dEndRects_PMVBO()
{

}


void RenderProvider::g2dEndLines_DARR()
{
	int		v_prim = (m_rctx.use_strip ? GL_LINE_STRIP : GL_LINES);
	int		v_obj_nbr = (m_rctx.use_strip ? 1 : 2);
	int		v_nbr = v_obj_nbr * (m_rctx.use_strip ? m_rctx.n : m_rctx.n / 2);
	int		v_size = 9 * sizeof(float);
	float  *v = (float*)malloc(v_nbr * v_size);
	float  *vi = v;

	// Add each object
	int i;
	if (m_rctx.use_strip)
	{
		vi = g2dSetVertex(vi, 0, 0., 0.);
		for (i = 1; i<m_rctx.n; i += 1)
		{
			vi = g2dSetVertex(vi, i, 0., 0.);
		}
	}
	else
	{
		for (i = 0; i + 1<m_rctx.n; i += 2)
		{
			vi = g2dSetVertex(vi, i, 0., 0.);
			vi = g2dSetVertex(vi, i + 1, 0., 0.);
		}
	}

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColorPointer(4, GL_FLOAT, v_size, v + 2);
	glVertexPointer(3, GL_FLOAT, v_size, v + 6);

	glDrawArrays(v_prim, 0, v_nbr);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	free(v);
}

void RenderProvider::g2dEndLines_VBO()
{

}

void RenderProvider::g2dEndLines_PMVBO()
{

}


void RenderProvider::g2dEndQuads_DARR()
{
	// Define vertices properties
	int v_prim = GL_TRIANGLES;
	int v_obj_nbr = 6;
	int v_nbr = v_obj_nbr * (m_rctx.n / 4);
	int v_size = 9 * sizeof(float);

	float *v = (float*)malloc(v_nbr*v_size);
	float *vi = v;

	// Add each object

	for (int i = 0; i + 3<m_rctx.n; i += 4)
	{
		vi = g2dSetVertex(vi, i, 0.f, 0.f);
		vi = g2dSetVertex(vi, i + 1, 1.f, 0.f);
		vi = g2dSetVertex(vi, i + 3, 0.f, 1.f);
		vi = g2dSetVertex(vi, i + 3, 0.f, 1.f);
		vi = g2dSetVertex(vi, i + 1, 1.f, 0.f);
		vi = g2dSetVertex(vi, i + 2, 1.f, 1.f);
	}

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, v_size, v);
	glColorPointer(4, GL_FLOAT, v_size, v + 2);
	glVertexPointer(3, GL_FLOAT, v_size, v + 6);

	// Then put it in the display list.
	glDrawArrays(v_prim, 0, v_nbr);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	free(v);
}

void RenderProvider::g2dEndQuads_VBO()
{

}

void RenderProvider::g2dEndQuads_PMVBO()
{

}


void RenderProvider::g2dEndPoints_DARR()
{
	// Define vertices properties
	int v_prim = GL_POINTS;
	int v_obj_nbr = 1;
	int v_nbr = v_obj_nbr * m_rctx.n;
	int v_size = 9 * sizeof(float);

	// Allocate vertex list memory
	float *v = (float*)malloc(v_nbr*v_size);
	float *vi = v;
	int i;

	// Add each object
	for (i = 0; i<m_rctx.n; i += 1)
	{
		vi = g2dSetVertex(vi, i, 0.f, 0.f);
	}

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColorPointer(4, GL_FLOAT, v_size, v + 2);
	glVertexPointer(3, GL_FLOAT, v_size, v + 6);

	// Then put it in the display list.
	glDrawArrays(v_prim, 0, v_nbr);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	free(v);
}

void RenderProvider::g2dEndPoints_VBO()
{

}

void RenderProvider::g2dEndPoints_PMVBO()
{

}


void RenderProvider::g2dEnd()
{
	if (!m_gbegin || m_rctx.n == 0)
	{
		m_gbegin = false;
		return;
	}

	if (m_rctx.use_z)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
	}

	if (m_rctx.tex == NULL)
	{
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_rctx.tex->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_rctx.use_tex_linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_rctx.use_tex_linear ? GL_LINEAR : GL_NEAREST);

		if (m_rctx.use_tex_repeat)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
	}

	switch (m_rctx.type)
	{
		case RECTS:
			switch (m_renderMode)
			{
			case RM_DARRAY: g2dEndRects_DARR();	 break;
			case RM_VBO:    g2dEndRects_VBO(); 	 break;
			case RM_PMVBO:  g2dEndRects_PMVBO(); break;
			}
			break;

		case LINES:  
			switch (m_renderMode)
			{
			case RM_DARRAY: g2dEndLines_DARR();  break;
			case RM_VBO:	g2dEndLines_VBO();	 break;
			case RM_PMVBO: 	g2dEndLines_PMVBO(); break;
			}
			break;
		
		case QUADS:
			switch (m_renderMode)
			{
			case RM_DARRAY: g2dEndQuads_DARR();  break;
			case RM_VBO:	g2dEndQuads_VBO();   break;
			case RM_PMVBO:  g2dEndQuads_PMVBO(); break;
			}
			break;
		
		case POINTSS: 
			switch (m_renderMode)
			{
			case RM_DARRAY: g2dEndPoints_DARR();  break;
			case RM_VBO:	g2dEndPoints_VBO();   break;
			case RM_PMVBO: 	g2dEndPoints_PMVBO(); break;
			}
			break;
	}

	m_gbegin = false;

	if (m_rctx.use_z) m_zclear = true;

	bufferDataIndex = 0;
}

void RenderProvider::g2dReset()
{
	g2dResetCoord();
	g2dResetScale();
	g2dResetColor();
	g2dResetAlpha();
	g2dResetRotation();
	g2dResetCrop();
	g2dResetTex();
}

void RenderProvider::g2dAdd()
{
	if (!m_gbegin || m_rctx.cur_obj.scale_w == 0.f || m_rctx.cur_obj.scale_h == 0.f)
	{
		return;
	}

	if (m_rctx.n % MALLOC_STEP == 0)
	{
		m_rctx.obj = (Object *)realloc(m_rctx.obj, (m_rctx.n + MALLOC_STEP) * sizeof(Object));
	}
	// Instead of filling this m_rctx buffer we should directly fill the VBOs

	m_rctx.n++;
	m_rctx.cur_obj;

	// Coordinate mode stuff
	m_rctx.cur_obj.rot_x = m_rctx.cur_obj.x;
	m_rctx.cur_obj.rot_y = m_rctx.cur_obj.y;

	switch (m_rctx.coord_mode)
	{
	case G2D_UP_RIGHT:
		m_rctx.cur_obj.x -= m_rctx.cur_obj.scale_w;
		break;

	case G2D_DOWN_RIGHT:
		m_rctx.cur_obj.x -= m_rctx.cur_obj.scale_w;
		m_rctx.cur_obj.y -= m_rctx.cur_obj.scale_h;
		break;

	case G2D_DOWN_LEFT:
		m_rctx.cur_obj.y -= m_rctx.cur_obj.scale_h;
		break;

	case G2D_CENTER:
		m_rctx.cur_obj.x -= m_rctx.cur_obj.scale_w / 2.f;
		m_rctx.cur_obj.y -= m_rctx.cur_obj.scale_h / 2.f;
		break;

	case G2D_DOWN_CENTER:
		m_rctx.cur_obj.x -= m_rctx.cur_obj.scale_w / 2.f;
		m_rctx.cur_obj.y -= m_rctx.cur_obj.scale_h;
		break;

	case G2D_UP_LEFT:

	default:
		break;
	};

	// Alpha stuff
	m_rctx.cur_obj.color = G2D_MODULATE(m_rctx.cur_obj.color, 255, m_rctx.cur_obj.alpha);

	switch (m_renderMode)
	{
	case RM_DARRAY:
		break;
	case RM_VBO:
		switch (m_rctx.type)
		{
		case RECTS:
			g2dSetRectVertices(m_rctx.cur_obj);
			break;

		case LINES:
			g2dSetLineVertices(m_rctx.cur_obj);
			break;

		case QUADS:
			g2dSetQuadVertices(m_rctx.cur_obj);
			break;

		case POINTSS:
			g2dSetPointVertices(m_rctx.cur_obj);
			break;
		}
		break;
	case RM_PMVBO:
		switch (m_rctx.type)
		{
		case RECTS:
			g2dSetRectVertices(m_rctx.cur_obj);
			break;

		case LINES:
			break;

		case QUADS:
			break;

		case POINTSS:
			break;
		}
		break;
	default:
		TRACE(T_DEB, "No special way to add object.Using Drawing Arrays");
		break;
	}
}

void RenderProvider::g2dPush()
{
	if (m_tstack_size >= TSTACK_MAX)
		return;

	m_tstack_size++;

	m_tstack[m_tstack_size - 1].x = m_rctx.cur_obj.x;
	m_tstack[m_tstack_size - 1].y = m_rctx.cur_obj.y;
	m_tstack[m_tstack_size - 1].z = m_rctx.cur_obj.z;
	m_tstack[m_tstack_size - 1].rot = m_rctx.cur_obj.rot;
	m_tstack[m_tstack_size - 1].rot_sin = m_rctx.cur_obj.rot_sin;
	m_tstack[m_tstack_size - 1].rot_cos = m_rctx.cur_obj.rot_cos;
	m_tstack[m_tstack_size - 1].scale_w = m_rctx.cur_obj.scale_w;
	m_tstack[m_tstack_size - 1].scale_h = m_rctx.cur_obj.scale_h;
}

void RenderProvider::g2dPop()
{
	if (m_tstack_size <= 0)
		return;

	m_rctx.cur_obj.x		 = m_tstack[m_tstack_size - 1].x;
	m_rctx.cur_obj.y		 = m_tstack[m_tstack_size - 1].y;
	m_rctx.cur_obj.z		 = m_tstack[m_tstack_size - 1].z;
	m_rctx.cur_obj.rot	 = m_tstack[m_tstack_size - 1].rot;
	m_rctx.cur_obj.rot_sin = m_tstack[m_tstack_size - 1].rot_sin;
	m_rctx.cur_obj.rot_cos = m_tstack[m_tstack_size - 1].rot_cos;
	m_rctx.cur_obj.scale_w = m_tstack[m_tstack_size - 1].scale_w;
	m_rctx.cur_obj.scale_h = m_tstack[m_tstack_size - 1].scale_h;

	m_tstack_size--;

	if (m_rctx.cur_obj.rot != 0.f) m_rctx.use_rot = true;
	if (m_rctx.cur_obj.z != 0.f)   m_rctx.use_z = true;
}

void RenderProvider::g2dResetCoord()
{
	m_rctx.cur_obj.x = DEFAULT_X;
	m_rctx.cur_obj.y = DEFAULT_Y;
	m_rctx.cur_obj.z = DEFAULT_Z;
}

void RenderProvider::g2dSetCoordMode(g2dCoord_Mode mode)
{
	m_rctx.coord_mode = mode;
}

void RenderProvider::g2dGetCoordXYZ(float *x, float *y, float *z)
{
	if (x != NULL) *x = m_rctx.cur_obj.x;
	if (y != NULL) *y = m_rctx.cur_obj.y;
	if (z != NULL) *z = m_rctx.cur_obj.z;
}

void RenderProvider::g2dSetCoordXY(float x, float y)
{
	m_rctx.cur_obj.x = x * m_global_scale;
	m_rctx.cur_obj.y = y * m_global_scale;
	m_rctx.cur_obj.z = 0.f;
}

void RenderProvider::g2dSetCoordXYZ(float x, float y, float z)
{
	m_rctx.cur_obj.x = x * m_global_scale;
	m_rctx.cur_obj.y = y * m_global_scale;
	m_rctx.cur_obj.z = z * m_global_scale;

	if (z != 0.f)
		m_rctx.use_z = true;
}

void RenderProvider::g2dSetCoordXYRelative(float x, float y)
{
	float inc_x = x;
	float inc_y = y;

	if (m_rctx.cur_obj.rot_cos != 1.f)
	{
		inc_x = -m_rctx.cur_obj.rot_sin*y + m_rctx.cur_obj.rot_cos*x;
		inc_y = m_rctx.cur_obj.rot_cos*y + m_rctx.cur_obj.rot_sin*x;
	}

	m_rctx.cur_obj.x += inc_x * m_global_scale;
	m_rctx.cur_obj.y += inc_y * m_global_scale;
}

void RenderProvider::g2dSetCoordXYZRelative(float x, float y, float z)
{
	g2dSetCoordXYRelative(x, y);

	m_rctx.cur_obj.z += z * m_global_scale;

	if (z != 0.f)
		m_rctx.use_z = true;
}

void RenderProvider::g2dSetCoordInteger(bool use)
{
	m_rctx.use_int = use;
}

void RenderProvider::g2dResetGlobalScale()
{
	m_global_scale = 1.f;
}

void RenderProvider::g2dResetScale()
{
	if (m_rctx.tex == NULL)
	{
		m_rctx.cur_obj.scale_w = DEFAULT_SIZE;
		m_rctx.cur_obj.scale_h = DEFAULT_SIZE;
	}
	else
	{
		m_rctx.cur_obj.scale_w = m_rctx.tex->w;
		m_rctx.cur_obj.scale_h = m_rctx.tex->h;
	}

	m_rctx.cur_obj.scale_w *= m_global_scale;
	m_rctx.cur_obj.scale_h *= m_global_scale;
}

void RenderProvider::g2dGetGlobalScale(float *scale)
{
	if (scale != NULL)
		*scale = m_global_scale;
}

void RenderProvider::g2dGetScaleWH(float *w, float *h)
{
	if (w != NULL) *w = m_rctx.cur_obj.scale_w;
	if (h != NULL) *h = m_rctx.cur_obj.scale_h;
}

void RenderProvider::g2dSetGlobalScale(float scale)
{
	m_global_scale = scale;
}

void RenderProvider::g2dSetScale(float w, float h)
{
	g2dResetScale();

	g2dSetScaleRelative(w, h);
}

void RenderProvider::g2dSetScaleWH(float w, float h)
{
	m_rctx.cur_obj.scale_w = w * m_global_scale;
	m_rctx.cur_obj.scale_h = h * m_global_scale;

	// A trick to prevent an unexpected behavior when mirroring with GU_SPRITES.
	if (m_rctx.cur_obj.scale_w < 0 || m_rctx.cur_obj.scale_h < 0)
		m_rctx.use_rot = true;
}

void RenderProvider::g2dSetScaleRelative(float w, float h)
{
	m_rctx.cur_obj.scale_w *= w;
	m_rctx.cur_obj.scale_h *= h;

	if (m_rctx.cur_obj.scale_w < 0 || m_rctx.cur_obj.scale_h < 0)
		m_rctx.use_rot = true;
}

void RenderProvider::g2dSetScaleWHRelative(float w, float h)
{
	m_rctx.cur_obj.scale_w += w * m_global_scale;
	m_rctx.cur_obj.scale_h += h * m_global_scale;

	if (m_rctx.cur_obj.scale_w < 0 || m_rctx.cur_obj.scale_h < 0)
		m_rctx.use_rot = true;
}

void RenderProvider::g2dResetColor()
{
	m_rctx.cur_obj.color = DEFAULT_COLOR;
}

void RenderProvider::g2dResetAlpha()
{
	m_rctx.cur_obj.alpha = DEFAULT_ALPHA;
}

void RenderProvider::g2dGetAlpha(g2dAlpha *alpha)
{
	if (alpha != NULL)
		*alpha = m_rctx.cur_obj.alpha;
}

void RenderProvider::g2dSetColor(g2dColor color)
{
	m_rctx.cur_obj.color = color;

	if (++m_rctx.color_count > 1)
		m_rctx.use_vert_color = true;
}

void RenderProvider::g2dSetAlpha(g2dAlpha alpha)
{
	if (alpha < 0)   alpha = 0;
	if (alpha > 255) alpha = 255;

	m_rctx.cur_obj.alpha = alpha;

	if (++m_rctx.color_count > 1)
		m_rctx.use_vert_color = true;
}

void RenderProvider::g2dSetAlphaRelative(int alpha)
{
	g2dSetAlpha(m_rctx.cur_obj.alpha + alpha);
}

/* Rotation functions */

void RenderProvider::g2dResetRotation()
{
	m_rctx.cur_obj.rot = 0.f;
	m_rctx.cur_obj.rot_sin = 0.f;
	m_rctx.cur_obj.rot_cos = 1.f;
}

void RenderProvider::g2dGetRotationRad(float *radians)
{
	if (radians != NULL)
		*radians = m_rctx.cur_obj.rot;
}

void RenderProvider::g2dGetRotation(float *degrees)
{
	if (degrees != NULL)
		*degrees = m_rctx.cur_obj.rot * M_180_PI;
}

void RenderProvider::g2dSetRotationRad(float radians)
{
	if (radians == m_rctx.cur_obj.rot)
	{
		return;
	}

	m_rctx.cur_obj.rot = radians;
	
	m_rctx.cur_obj.rot_sin = sinf(radians);

	m_rctx.cur_obj.rot_cos = cosf(radians);

	//  sincosf(radians, &m_rctx.cur_obj.rot_sin, &m_rctx.cur_obj.rot_cos);


	if (radians != 0.f)
	{
		m_rctx.use_rot = true;
	}
}

void RenderProvider::g2dSetRotation(float degrees)
{
	g2dSetRotationRad(degrees * M_PI_180);
}

void RenderProvider::g2dSetRotationRadRelative(float radians)
{
	g2dSetRotationRad(m_rctx.cur_obj.rot + radians);
}

void RenderProvider::g2dSetRotationRelative(float degrees)
{
	g2dSetRotationRadRelative(degrees * M_PI_180);
}

/* Crop functions */

void RenderProvider::g2dResetCrop()
{
	if (m_rctx.tex == NULL)
		return;

	m_rctx.cur_obj.crop_x = 0;
	m_rctx.cur_obj.crop_y = 0;
	m_rctx.cur_obj.crop_w = m_rctx.tex->w;
	m_rctx.cur_obj.crop_h = m_rctx.tex->h;
}

void RenderProvider::g2dGetCropXY(int *x, int *y)
{
	if (m_rctx.tex == NULL)
		return;

	if (x != NULL) *x = m_rctx.cur_obj.crop_x;
	if (y != NULL) *y = m_rctx.cur_obj.crop_y;
}

void RenderProvider::g2dGetCropWH(int *w, int *h)
{
	if (m_rctx.tex == NULL)
		return;

	if (w != NULL) *w = m_rctx.cur_obj.crop_w;
	if (h != NULL) *h = m_rctx.cur_obj.crop_h;
}

void RenderProvider::g2dSetCropXY(int x, int y)
{
	if (m_rctx.tex == NULL)
		return;

	m_rctx.cur_obj.crop_x = x;
	m_rctx.cur_obj.crop_y = y;
}

void RenderProvider::g2dSetCropWH(int w, int h)
{
	if (m_rctx.tex == NULL)
		return;

	m_rctx.cur_obj.crop_w = w;
	m_rctx.cur_obj.crop_h = h;
}

void RenderProvider::g2dSetCropXYRelative(int x, int y)
{
	if (m_rctx.tex == NULL)
		return;

	g2dSetCropXY(m_rctx.cur_obj.crop_x + x, m_rctx.cur_obj.crop_y + y);
}

void RenderProvider::g2dSetCropWHRelative(int w, int h)
{
	if (m_rctx.tex == NULL)
		return;

	g2dSetCropWH(m_rctx.cur_obj.crop_w + w, m_rctx.cur_obj.crop_h + h);
}

/* Texture functions */

void RenderProvider::g2dResetTex()
{
	if (m_rctx.tex == NULL)
		return;

	m_rctx.use_tex_repeat = false;
	m_rctx.use_tex_linear = true;
}

void RenderProvider::g2dSetTexRepeat(bool use)
{
	if (m_rctx.tex == NULL)
		return;

	m_rctx.use_tex_repeat = use;
}

void RenderProvider::g2dSetTexLinear(bool use)
{
	if (m_rctx.tex == NULL)
		return;

	m_rctx.use_tex_linear = use;
}

/* Scissor functions */

void RenderProvider::g2dResetScissor()
{
	int width, height;

	glfwGetWindowSize(gWin, &width, &height);

	g2dSetScissor(0, 0, width, m_scissor);

	m_scissor = false;
}

void RenderProvider::g2dSetScissor(int x, int y, int w, int h)
{
	glScissor(x, y, w, h);

	m_scissor = true;
}

/* Swap Buffers */

void RenderProvider::g2dSwap()
{
	glfwSwapBuffers(gWin);
}
