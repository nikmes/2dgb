#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <string>
#include "platform.h"

using namespace std;

#define PI 3.14159265359f	// PI used in rotation calculations
#define BSCALE 30.f			// Box2D Scaling
#define poffsetof(type, field)((void *) (&((type *) 0)->field))
#define MAXPATH 256

#if defined(PLATFORM_PS3)
#define BASE_PATH "/dev_hdd0/game/SPRITE2D2/USRDIR/media/"
#else
#define BASE_PATH "media/"
#endif

struct textItem
{
	float x;
	float y;
	char cbuffer[256];
};

struct Position
{
	float x, y;
};

typedef enum
{
	G2D_SWIZZLE = 1, /**< Recommended. Use it to speedup rendering. */
	G2D_STD = 0

} g2dTex_Mode;

typedef unsigned int g2dColor;

typedef struct
{
	int tw;             /**< Real texture width. A power of two. */
	int th;             /**< Real texture height. A power of two. */
	int w;              /**< Texture width, as seen when drawing. */
	int h;              /**< Texture height, as seen when drawing. */
	float ratio;        /**< Width/height ratio. */
	bool swizzled;      /**< Is the texture swizzled ? */
	g2dColor *data;     /**< Pointer to raw data. */
	unsigned int id;
	char name[128];
} g2dTexture;

typedef struct
{
	float x;	 // starting x position of item on texture atlas
	float y;	 // starting y position of item on texture atlas
	float w;	 // width of item on atlas (real)
	float h;	 // height of item on atlas (real)
	string name; // name of item
	string hint; // icon hint
	int id;		 // identifier
	float pX;	 // x position of item in world
	float pY;	 // y position of item in world
	float sW;	 // scalsed width of item
	float sH;	 // scaled height of item
	float r;     // item rotaiton in radiants
} textureAtlasItem;

typedef struct point
{
	float x;
	float y;
} point;

typedef enum
{
  FT_GAME,
  FT_TEXTUREATLAS,
  FT_PNG
} FileType;

typedef enum
{
	// MAIN STATES
	IDLE,
	GAME,
	EDIT,
	FILEO,
	INTRO,
	PHYSIC,
	GAMEPLAY,
	GAMEINTRO,
	GAMEMENU,
	SANDBOX,
	BENCHMARK
} State;

typedef enum
{
	// EDITSTATE SUBSTATES
	EST_DRAWTILED,
	EST_DRAWFREEHAND,
	EST_MOVEITEMS,
	EST_DELETE,
	EST_SINGLE_SELECT,
	EST_MULTIPLE_SELECT,
	EST_BOX2D,
	EST_SQUARE
	// PHYSICSSTATE SUBSTATES
	// INTROSTATE SYBSTATES
	// GAMESTATE SUBSTATES
} SubState;

typedef enum
{
	DT_TILED_CLICKED,
	DT_TILED_PRESSED
} DrawType;

typedef enum
{
	RO_ROTATE_ITEM,
	RO_SCALE_ITEM,
	RO_ADJUST_TRANSPARENCY,
	RO_FLIP_VERTICALLY,
	RO_FLIP_HORIZONTALLY
} RollerOperation;

typedef enum
{
	MT_CAMERA,
	MT_PLAYER,
	MT_LAYER
} MoveType;

typedef enum
{
	TILED,
	FREEHAND
} DRAWINGMODE;

typedef enum
{
	HORIZONTAL,
	VERTICAL,
	UP,
	DOWN,
	RIGHT,
	LEFT
} Orientation;

typedef enum
{
	FRONT,
	BACK
} layerPostion;

typedef enum 
{
	RM_DARRAY,
	RM_VBO,
	RM_PMVBO
} RenderMode;

enum
{
	LEFT_MOUSE_BUTTON,
	RIGHT_MOUSE_BUTTON,
	MIDLE_MOUSE_BUTTON
};

typedef enum
{
	TEXTUREPACKER,
	SHOEBOX
} TexturePackerType;

struct Vec2
{
	float x;
	float y;
};

/**
* \def G2D_VOID
* \brief Generic constant, equals to 0 (do nothing).
*/

#define G2D_VOID (0)

/**
* \def G2D_RGBA(r,g,b,a)
* \brief Create a g2dColor.
*
* This macro creates a g2dColor from 4 values, red, green, blue and alpha.
* Input range is from 0 to 255.
*/
#define G2D_RGBA(r, g, b, a) ((r)|((g)<<8)|((b)<<16)|((a)<<24))

/**
* \def G2D_GET_R(color)
* \brief Get red channel value from a g2dColor.
*/
/**
* \def G2D_GET_G(color)
* \brief Get green channel value from a g2dColor.
*/
/**
* \def G2D_GET_B(color)
* \brief Get blue channel value from a g2dColor.
*/
/**
* \def G2D_GET_A(color)
* \brief Get alpha channel value from a g2dColor.
*/
#define G2D_GET_R(color) (((color)      ) & 0xFF)
#define G2D_GET_G(color) (((color) >>  8) & 0xFF)
#define G2D_GET_B(color) (((color) >> 16) & 0xFF)
#define G2D_GET_A(color) (((color) >> 24) & 0xFF)

/**
* \def G2D_MODULATE(color,luminance,alpha)
* \brief g2dColor modulation.
*
* This macro modulates the luminance & alpha of a g2dColor.
* Input range is from 0 to 255.
*/
#define G2D_MODULATE(color,luminance,alpha) \
    G2D_RGBA((int)(luminance) * G2D_GET_R(color) / 255, \
             (int)(luminance) * G2D_GET_G(color) / 255, \
             (int)(luminance) * G2D_GET_B(color) / 255, \
             (int)(alpha    ) * G2D_GET_A(color) / 255)

/**
* \enum g2dColors
* \brief Colors enumeration.
*
* Primary, secondary, tertiary and grayscale colors are defined.
*/
enum g2dColors
{
	// Primary colors
	RED   = 0xFF0000FF,
	GREEN = 0xFF00FF00,
	BLUE  = 0xFFFF0000,
	// Secondary colors
	CYAN = 0xFFFFFF00,
	MAGENTA = 0xFFFF00FF,
	YELLOW = 0xFF00FFFF,
	SKY = 0xFF87655C,
	// Tertiary colors
	AZURE = 0xFFFF7F00,
	VIOLET = 0xFFFF007F,
	ROSE = 0xFF7F00FF,
	ORANGE = 0xFF007FFF,
	CHARTREUSE = 0xFF00FF7F,
	SPRING_GREEN = 0xFF7FFF00,
	// Grayscale
	WHITE = 0xFFFFFFFF,
	LITEGRAY = 0xFFBFBFBF,
	GRAY = 0xFF7F7F7F,
	DARKGRAY = 0xFF3F3F3F,
	BLACK = 0xFF000000
};

/**
* \enum g2dCoord_Mode
* \brief Coordinates modes enumeration.
*
* Choose where the coordinates correspond in the object.
* Can only be used with g2dSetCoordMode.
*/
/**
* \enum g2dLine_Mode
* \brief Line modes enumeration.
*
* Change line draw properties.
* Can only be used with g2dBeginLines.
*/
/**
* \enum g2dFlip_Mode
* \brief Flip modes enumeration.
*
* Change flip properties.
* Can only be used with g2dFlip.
*/
/**
* \enum g2dTex_Mode
* \brief Texture modes enumeration.
*
* Change texture properties.
* Can only be used with g2dTexLoad.
*/
typedef enum
{
	G2D_UP_LEFT,
	G2D_UP_RIGHT,
	G2D_DOWN_RIGHT,
	G2D_DOWN_LEFT,
	G2D_CENTER,
	G2D_DOWN_CENTER
} g2dCoord_Mode;

typedef enum
{
	G2D_STRIP = 1, /**< Make a line strip. */
	G2D_NULL = 0
} g2dLine_Mode;

typedef enum
{
	G2D_VSYNC = 1 /**< Limit the FPS to 60 (synchronized with the screen).
				  Better quality and less power consumption. */
} g2dFlip_Mode;


/**
* \var g2dAlpha
* \brief Alpha type.
*/
/**
* \var g2dColor
* \brief Color type.
*/
typedef int g2dAlpha;
typedef unsigned int g2dColor;
#define DLIST_SIZE              (524288)
#define LINE_SIZE               (512)
#define PIXEL_SIZE              (4)
#define FRAMEBUFFER_SIZE        (LINE_SIZE*G2D_SCR_H*PIXEL_SIZE)
#define FRAME_BUFFER_SIZE	    (LINE_SIZE*G2D_SCR_H)
#define MALLOC_STEP             (128)
#define TSTACK_MAX              (64)
#define SLICE_WIDTH             (64.f)
#define M_180_PI                (57.29578f)
#define M_PI_180                (0.017453292f)

#define DEFAULT_SIZE            (10)
#define DEFAULT_COORD_MODE      (G2D_UP_LEFT)
#define DEFAULT_X               (0.f)
#define DEFAULT_Y               (0.f)
#define DEFAULT_Z               (0.f)
#define DEFAULT_COLOR           (WHITE)
#define DEFAULT_ALPHA           (0xFF)


#define OBJ                     rctx.obj[rctx.n-1]
#define OBJ_I                   rctx.obj[i]
#define TRANSFORM               tstack[tstack_size-1]
typedef enum
{
	RECTS,
	LINES,
	QUADS,
	POINTSS
} Obj_Type;

/* Structures */

typedef struct
{
	float x, y, z;
	float rot, rot_sin, rot_cos;
	float scale_w, scale_h;
} Transform;

typedef struct
{
	float x, y, z;
	float rot_x, rot_y; // Rotation center
	float rot, rot_sin, rot_cos;
	float scale_w, scale_h;
	int crop_x, crop_y;
	int crop_w, crop_h;

	g2dColor color;
	g2dAlpha alpha;

}Object;

typedef struct
{
	Object *obj;
	Object cur_obj;
	unsigned int n;
	Obj_Type type;
	g2dTexture *tex;
	bool use_strip;
	bool use_z;
	bool use_vert_color;
	bool use_rot;
	bool use_tex_linear;
	bool use_tex_repeat;
	bool use_int;
	unsigned int color_count;
	g2dCoord_Mode coord_mode;
} RenderContext;

#if defined(PLATFORM_LINUX)
typedef unsigned long DWORD;
#endif 

#endif
