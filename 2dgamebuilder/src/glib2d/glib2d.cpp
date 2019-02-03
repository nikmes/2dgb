/*
* gLib2D - A simple, fast, light-weight 2D graphics library.
*
* Copyright 2012 Cl��ment Gu��rin <geecko.dev@free.fr>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include "servicelocator.h"
#include <glib2d/glib2d.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <png.h>
#include <math.h>
#include "globals.h"

/* Local variables */

static int *dlist;

static RenderContext rctx;

static Transform tstack[TSTACK_MAX];
static unsigned int tstack_size;

static bool init    = true;
static bool start   = false;
static bool gbegin  = false;
static bool zclear  = true;
static bool scissor = false;

static float global_scale;

void _g2dStart()
{
	start = true;
}

float* _g2dSetVertex(float *vp, int i, float vx, float vy)
{
	// Vertex order: [texture uv] [color] [coord]

	// A Prepare the texture vertices information
	if (rctx.tex != NULL)
	{
		vp[0] = (float)(OBJ_I.crop_x + vx*OBJ_I.crop_w) / rctx.tex->tw;
		vp[1] = (float)(OBJ_I.crop_y + vy*OBJ_I.crop_h) / rctx.tex->th;
	}
	else
	{
		vp[0] = 0;
		vp[1] = 0;
	}

	// A Prepare the color vertices information
	vp[2] = G2D_GET_R(OBJ_I.color) / 255.;
	vp[3] = G2D_GET_G(OBJ_I.color) / 255.;
	vp[4] = G2D_GET_B(OBJ_I.color) / 255.;
	vp[5] = G2D_GET_A(OBJ_I.color) / 255.;

	// A Prepare the position vertices information
	vp[6] = OBJ_I.x;
	vp[7] = OBJ_I.y;
	vp[8] = OBJ_I.z;

	if (rctx.type == RECTS)
	{
		vp[6] += vx * OBJ_I.scale_w;
		vp[7] += vy * OBJ_I.scale_h;

		if (rctx.use_rot) // Apply a rotation
		{
			float tx = vp[6] - OBJ_I.rot_x;
			float ty = vp[7] - OBJ_I.rot_y;

			vp[6] = OBJ_I.rot_x - OBJ_I.rot_sin*ty + OBJ_I.rot_cos*tx,
			vp[7] = OBJ_I.rot_y + OBJ_I.rot_cos*ty + OBJ_I.rot_sin*tx;
		}
	}

	if (rctx.use_int) // Pixel perfect
	{
		vp[6] = floorf(vp[6]);
		vp[7] = floorf(vp[7]);
	}

	return (vp + 9);
}

void g2dClear(g2dColor color)
{

	glClear(GL_COLOR_BUFFER_BIT | (zclear ? GL_DEPTH_BUFFER_BIT : 0));
	zclear = false;

	//glClearColor(r, g, b, a);//G2D_GET_R(color), G2D_GET_G(color), G2D_GET_B(color), G2D_GET_A(color));
	//glClearColor(G2D_GET_R(color), G2D_GET_G(color), G2D_GET_B(color), G2D_GET_A(color));
	
	//zclear = false;

	//glClear(GL_DEPTH_BUFFER_BIT);

	//zclear = true;
}

void g2dClearZ()
{
	if (!start)
	{
		_g2dStart();
	}

	glClear(GL_DEPTH_BUFFER_BIT);

	zclear = true;
}

void _g2dBeginCommon(Obj_Type type, g2dTexture *tex)
{
	if (gbegin)
		return;

	if (!start)
		_g2dStart();

	// Reset render context
	rctx.obj = (Object *)realloc(rctx.obj, MALLOC_STEP * sizeof(Object));
	rctx.n = 0;
	rctx.type = type;
	rctx.tex = tex;
	rctx.use_strip = false;
	rctx.use_z = false;
	rctx.use_vert_color = false;
	rctx.use_rot = false;
	rctx.use_tex_linear = true;
	rctx.use_tex_repeat = false;
	rctx.use_int = false;
	rctx.color_count = 0;
	rctx.coord_mode = DEFAULT_COORD_MODE;

	// Reset current object
	g2dReset();

	gbegin = true;
}

void g2dBeginRects(g2dTexture *tex)
{
	_g2dBeginCommon(RECTS, tex);
}

void g2dBeginLines(g2dLine_Mode mode)
{
	_g2dBeginCommon(LINES, NULL);

	rctx.use_strip = (mode & G2D_STRIP);
}

void g2dBeginQuads(g2dTexture *tex)
{
	_g2dBeginCommon(QUADS, tex);
}

void g2dBeginPoints()
{
	_g2dBeginCommon(POINTSS, NULL);
}

void _g2dEndRects()
{
	int v_prim = GL_TRIANGLES;
	int v_obj_nbr = 6;
	int v_nbr = v_obj_nbr * rctx.n;
	int v_size = 9 * sizeof(float);

	// Allocate vertex list memory
	float *v = (float*)malloc(v_nbr * v_size);
	float *vi = v;

	// Add each object	
	for (int i = 0; i<rctx.n; i += 1)
	{
		vi = _g2dSetVertex(vi, i, 0.f, 0.f); //
		vi = _g2dSetVertex(vi, i, 1.f, 0.f); // 1st triangle
		vi = _g2dSetVertex(vi, i, 0.f, 1.f); //

		vi = _g2dSetVertex(vi, i, 0.f, 1.f); //
		vi = _g2dSetVertex(vi, i, 1.f, 0.f); // 2nd triangle
		vi = _g2dSetVertex(vi, i, 1.f, 1.f); // 
	}

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, v_size, v);
	glColorPointer	 (4, GL_FLOAT, v_size, v + 2);
	glVertexPointer	 (3, GL_FLOAT, v_size, v + 6);

	glDrawArrays(v_prim, 0, v_nbr);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	free(v);
}

void _g2dEndLines()
{
	int		v_prim		= (rctx.use_strip ? GL_LINE_STRIP : GL_LINES);
	int		v_obj_nbr	= (rctx.use_strip ? 1 : 2);
	int		v_nbr		= v_obj_nbr * (rctx.use_strip ? rctx.n : rctx.n / 2);
	int		v_size		= 9 * sizeof(float);
	float  *v			= (float*)malloc(v_nbr * v_size);
	float  *vi			= v;

	// Add each object
	int i;
	if (rctx.use_strip)
	{
		vi = _g2dSetVertex(vi, 0, 0., 0.);
		for (i = 1; i<rctx.n; i += 1)
		{
			vi = _g2dSetVertex(vi, i, 0., 0.);
		}
	}
	else
	{
		for (i = 0; i + 1<rctx.n; i += 2)
		{
			vi = _g2dSetVertex(vi, i, 0., 0.);
			vi = _g2dSetVertex(vi, i + 1, 0., 0.);
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

void _g2dEndQuads()
{
	// Define vertices properties
	int v_prim = GL_TRIANGLES;
	int v_obj_nbr = 6;
	int v_nbr = v_obj_nbr * (rctx.n / 4);
	int v_size = 9 * sizeof(float);

	float *v = (float*)malloc(v_nbr*v_size);
	float *vi = v;

	// Add each object

	for (int i = 0; i + 3<rctx.n; i += 4)
	{
		vi = _g2dSetVertex(vi, i, 0.f, 0.f);
		vi = _g2dSetVertex(vi, i + 1, 1.f, 0.f);
		vi = _g2dSetVertex(vi, i + 3, 0.f, 1.f);
		vi = _g2dSetVertex(vi, i + 3, 0.f, 1.f);
		vi = _g2dSetVertex(vi, i + 1, 1.f, 0.f);
		vi = _g2dSetVertex(vi, i + 2, 1.f, 1.f);
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

void _g2dEndPoints()
{
	// Define vertices properties
	int v_prim = GL_POINTS;
	int v_obj_nbr = 1;
	int v_nbr = v_obj_nbr * rctx.n;
	int v_size = 9 * sizeof(float);

	// Allocate vertex list memory
	float *v = (float*)malloc(v_nbr*v_size);
	float *vi = v;
	int i;

	// Add each object
	for (i = 0; i<rctx.n; i += 1)
	{
		vi = _g2dSetVertex(vi, i, 0.f, 0.f);
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

void g2dEnd()
{
	if (!gbegin || rctx.n == 0)
	{
		gbegin = false;
		return;
	}

	// Manage extensions
	if (rctx.use_z)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
	}

	if (rctx.tex == NULL)
	{
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, rctx.tex->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, rctx.use_tex_linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, rctx.use_tex_linear ? GL_LINEAR : GL_NEAREST);

		if (rctx.use_tex_repeat)
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

	switch (rctx.type)
	{
	case RECTS:   _g2dEndRects();  break;
	case LINES:   _g2dEndLines();  break;
	case QUADS:   _g2dEndQuads();  break;
	case POINTSS: _g2dEndPoints(); break;
	}

	gbegin = false;
	
	if (rctx.use_z) zclear = true;
}

void g2dReset()
{
	g2dResetCoord();
	g2dResetScale();
	g2dResetColor();
	g2dResetAlpha();
	g2dResetRotation();
	g2dResetCrop();
	g2dResetTex();
}

void g2dFlip(g2dFlip_Mode mode)
{
	if (scissor)
	{
		g2dResetScissor();
	}

	glFinish();
	
	zclear = false;

	start = false;
}

void g2dAdd()
{
	if (!gbegin || rctx.cur_obj.scale_w == 0.f || rctx.cur_obj.scale_h == 0.f)
	{
		return;
	}

	if (rctx.n % MALLOC_STEP == 0)
	{
		rctx.obj = (Object *)realloc(rctx.obj, (rctx.n + MALLOC_STEP) * sizeof(Object));
	}

	rctx.n++;
	OBJ = rctx.cur_obj;

	// Coordinate mode stuff
	OBJ.rot_x = OBJ.x;
	OBJ.rot_y = OBJ.y;

	float rx = OBJ.x; 
	float ry = OBJ.y; 

	float sw = OBJ.scale_w;
	float sh = OBJ.scale_h;

	float x = OBJ.x - OBJ.scale_w / 2.f;
	float y = OBJ.y - OBJ.scale_h / 2.f;
	

	switch (rctx.coord_mode)
	{
	case G2D_UP_RIGHT:
		OBJ.x -= OBJ.scale_w;
		break;

	case G2D_DOWN_RIGHT:
		OBJ.x -= OBJ.scale_w;
		OBJ.y -= OBJ.scale_h;
		break;

	case G2D_DOWN_LEFT:
		OBJ.y -= OBJ.scale_h;
		break;

	case G2D_CENTER:
		OBJ.x -= OBJ.scale_w / 2.f;
		OBJ.y -= OBJ.scale_h / 2.f;

		//TRACE(T_ERR, "Adding Rectangle at center [%f,%f]", OBJ.x, OBJ.y);
		break;

	case G2D_DOWN_CENTER:
		OBJ.x -= OBJ.scale_w / 2.f;
		OBJ.y -= OBJ.scale_h;
		break;

	case G2D_UP_LEFT:

	default:
		break;
	};

	// Alpha stuff
	OBJ.color = G2D_MODULATE(OBJ.color, 255, rctx.cur_obj.alpha);
}

void g2dPush()
{
	if (tstack_size >= TSTACK_MAX)
		return;

	tstack_size++;

	TRANSFORM.x = rctx.cur_obj.x;
	TRANSFORM.y = rctx.cur_obj.y;
	TRANSFORM.z = rctx.cur_obj.z;
	TRANSFORM.rot = rctx.cur_obj.rot;
	TRANSFORM.rot_sin = rctx.cur_obj.rot_sin;
	TRANSFORM.rot_cos = rctx.cur_obj.rot_cos;
	TRANSFORM.scale_w = rctx.cur_obj.scale_w;
	TRANSFORM.scale_h = rctx.cur_obj.scale_h;
}

void g2dPop()
{
	if (tstack_size <= 0)
		return;

	rctx.cur_obj.x = TRANSFORM.x;
	rctx.cur_obj.y = TRANSFORM.y;
	rctx.cur_obj.z = TRANSFORM.z;
	rctx.cur_obj.rot = TRANSFORM.rot;
	rctx.cur_obj.rot_sin = TRANSFORM.rot_sin;
	rctx.cur_obj.rot_cos = TRANSFORM.rot_cos;
	rctx.cur_obj.scale_w = TRANSFORM.scale_w;
	rctx.cur_obj.scale_h = TRANSFORM.scale_h;

	tstack_size--;

	if (rctx.cur_obj.rot != 0.f) rctx.use_rot = true;
	if (rctx.cur_obj.z != 0.f)   rctx.use_z = true;
}

/* Coord functions */

void g2dResetCoord()
{
	rctx.cur_obj.x = DEFAULT_X;
	rctx.cur_obj.y = DEFAULT_Y;
	rctx.cur_obj.z = DEFAULT_Z;
}

void g2dSetCoordMode(g2dCoord_Mode mode)
{
	//if (mode > G2D_CENTER)
	// return;

	rctx.coord_mode = mode;
}

void g2dGetCoordXYZ(float *x, float *y, float *z)
{
	if (x != NULL) *x = rctx.cur_obj.x;
	if (y != NULL) *y = rctx.cur_obj.y;
	if (z != NULL) *z = rctx.cur_obj.z;
}

void g2dSetCoordXY(float x, float y)
{
	rctx.cur_obj.x = x * global_scale;
	rctx.cur_obj.y = y * global_scale;
	rctx.cur_obj.z = 0.f;
}

void g2dSetCoordXYZ(float x, float y, float z)
{
	rctx.cur_obj.x = x * global_scale;
	rctx.cur_obj.y = y * global_scale;
	rctx.cur_obj.z = z;//*global_scale;

	if (z != 0.f)
		rctx.use_z = true;
}

void g2dSetCoordXYRelative(float x, float y)
{
	float inc_x = x;
	float inc_y = y;

	if (rctx.cur_obj.rot_cos != 1.f)
	{
		inc_x = -rctx.cur_obj.rot_sin*y + rctx.cur_obj.rot_cos*x;
		inc_y = rctx.cur_obj.rot_cos*y + rctx.cur_obj.rot_sin*x;
	}

	rctx.cur_obj.x += inc_x * global_scale;
	rctx.cur_obj.y += inc_y * global_scale;
}

void g2dSetCoordXYZRelative(float x, float y, float z)
{
	g2dSetCoordXYRelative(x, y);

	rctx.cur_obj.z += z;// *global_scale;

	if (z != 0.f)
		rctx.use_z = true;
}

void g2dSetCoordInteger(bool use)
{
	rctx.use_int = use;
}

/* Scale functions */

void g2dResetGlobalScale()
{
	global_scale = 1.f;
}

void g2dResetScale()
{
	if (rctx.tex == NULL)
	{
		rctx.cur_obj.scale_w = DEFAULT_SIZE;
		rctx.cur_obj.scale_h = DEFAULT_SIZE;
	}
	else
	{
		rctx.cur_obj.scale_w = rctx.tex->w;
		rctx.cur_obj.scale_h = rctx.tex->h;
	}

	rctx.cur_obj.scale_w *= global_scale;
	rctx.cur_obj.scale_h *= global_scale;
}

void g2dGetGlobalScale(float *scale)
{
	if (scale != NULL)
		*scale = global_scale;
}

void g2dGetScaleWH(float *w, float *h)
{
	if (w != NULL) *w = rctx.cur_obj.scale_w;
	if (h != NULL) *h = rctx.cur_obj.scale_h;
}

void g2dSetGlobalScale(float scale)
{
	global_scale = scale;
}

void g2dSetScale(float w, float h)
{
	g2dResetScale();

	g2dSetScaleRelative(w, h);
}

void g2dSetScaleWH(float w, float h)
{
	rctx.cur_obj.scale_w = w * global_scale;
	rctx.cur_obj.scale_h = h * global_scale;

	// A trick to prevent an unexpected behavior when mirroring with GU_SPRITES.
 	if (rctx.cur_obj.scale_w < 0 || rctx.cur_obj.scale_h < 0)
		rctx.use_rot = true;
}

void g2dSetScaleRelative(float w, float h)
{
	rctx.cur_obj.scale_w *= w;
	rctx.cur_obj.scale_h *= h;

	if (rctx.cur_obj.scale_w < 0 || rctx.cur_obj.scale_h < 0)
		rctx.use_rot = true;
}

void g2dSetScaleWHRelative(float w, float h)
{
	rctx.cur_obj.scale_w += w * global_scale;
	rctx.cur_obj.scale_h += h * global_scale;

	if (rctx.cur_obj.scale_w < 0 || rctx.cur_obj.scale_h < 0)
		rctx.use_rot = true;
}

/* Color functions */

void g2dResetColor()
{
	rctx.cur_obj.color = DEFAULT_COLOR;
}

void g2dResetAlpha()
{
	rctx.cur_obj.alpha = DEFAULT_ALPHA;
}

void g2dGetAlpha(g2dAlpha *alpha)
{
	if (alpha != NULL)
		*alpha = rctx.cur_obj.alpha;
}

void g2dSetColor(g2dColor color)
{
	rctx.cur_obj.color = color;

	if (++rctx.color_count > 1)
		rctx.use_vert_color = true;
}

void g2dSetAlpha(g2dAlpha alpha)
{
	if (alpha < 0)   alpha = 0;
	if (alpha > 255) alpha = 255;

	rctx.cur_obj.alpha = alpha;

	if (++rctx.color_count > 1)
		rctx.use_vert_color = true;
}

void g2dSetAlphaRelative(int alpha)
{
	g2dSetAlpha(rctx.cur_obj.alpha + alpha);
}

/* Rotation functions */

void g2dResetRotation()
{
	rctx.cur_obj.rot = 0.f;
	rctx.cur_obj.rot_sin = 0.f;
	rctx.cur_obj.rot_cos = 1.f;
}

void g2dGetRotationRad(float *radians)
{
	if (radians != NULL)
		*radians = rctx.cur_obj.rot;
}

void g2dGetRotation(float *degrees)
{
	if (degrees != NULL)
		*degrees = rctx.cur_obj.rot * M_180_PI;
}

void g2dSetRotationRad(float radians)
{
	if (radians == rctx.cur_obj.rot)
		return;

	rctx.cur_obj.rot = radians;
	rctx.cur_obj.rot_sin = sinf(radians);
	rctx.cur_obj.rot_cos = cosf(radians);

	//  sincosf(radians, &rctx.cur_obj.rot_sin, &rctx.cur_obj.rot_cos);


	if (radians != 0.f)
		rctx.use_rot = true;
}

void g2dSetRotation(float degrees)
{
	g2dSetRotationRad(degrees * M_PI_180);
}

void g2dSetRotationRadRelative(float radians)
{
	g2dSetRotationRad(rctx.cur_obj.rot + radians);
}

void g2dSetRotationRelative(float degrees)
{
	g2dSetRotationRadRelative(degrees * M_PI_180);
}

/* Crop functions */

void g2dResetCrop()
{
	if (rctx.tex == NULL)
		return;

	rctx.cur_obj.crop_x = 0;
	rctx.cur_obj.crop_y = 0;
	rctx.cur_obj.crop_w = rctx.tex->w;
	rctx.cur_obj.crop_h = rctx.tex->h;
}

void g2dGetCropXY(int *x, int *y)
{
	if (rctx.tex == NULL)
		return;

	if (x != NULL) *x = rctx.cur_obj.crop_x;
	if (y != NULL) *y = rctx.cur_obj.crop_y;
}

void g2dGetCropWH(int *w, int *h)
{
	if (rctx.tex == NULL)
		return;

	if (w != NULL) *w = rctx.cur_obj.crop_w;
	if (h != NULL) *h = rctx.cur_obj.crop_h;
}

void g2dSetCropXY(int x, int y)
{
	if (rctx.tex == NULL)
		return;

	rctx.cur_obj.crop_x = x;
	rctx.cur_obj.crop_y = y;
}

void g2dSetCropWH(int w, int h)
{
	if (rctx.tex == NULL)
		return;

	rctx.cur_obj.crop_w = w;
	rctx.cur_obj.crop_h = h;
}

void g2dSetCropXYRelative(int x, int y)
{
	if (rctx.tex == NULL)
		return;

	g2dSetCropXY(rctx.cur_obj.crop_x + x, rctx.cur_obj.crop_y + y);
}

void g2dSetCropWHRelative(int w, int h)
{
	if (rctx.tex == NULL)
		return;

	g2dSetCropWH(rctx.cur_obj.crop_w + w, rctx.cur_obj.crop_h + h);
}

/* Texture functions */

void g2dResetTex()
{
	if (rctx.tex == NULL)
		return;

	rctx.use_tex_repeat = false;
	rctx.use_tex_linear = true;
}

void g2dSetTexRepeat(bool use)
{
	if (rctx.tex == NULL)
		return;

	rctx.use_tex_repeat = use;
}

void g2dSetTexLinear(bool use)
{
	if (rctx.tex == NULL)
		return;

	rctx.use_tex_linear = use;
}

/* Scissor functions */

void g2dResetScissor()
{
	int width, height;
	
	ServiceLocator::getRenderService()->getWindowSize(&width, &height);

	g2dSetScissor(0, 0, width, height);

	scissor = false;
}

void g2dSetScissor(int x, int y, int w, int h)
{
	glScissor(x, y, w, h);

	scissor = true;
}

// EOF
