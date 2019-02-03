#include "servicelocator.h"
#include "box2ddebugdraw.h"
#include <glib2d/glib2d.h>

void translateToCam(float32 &x, float32 &y)
{
	int width, height;
	ServiceLocator::getRenderService()->getWindowSize(&width, &height);

	CameraProvider *cam = ServiceLocator::getCameraService();

	float angle = -(ServiceLocator::getCameraService()->rot * (b2_pi / 180));

	float x1 = cosf(angle) * ((x * BSCALE) - cam->x) - sinf(angle) * ((y * BSCALE) - cam->y) + cam->x;
	float y1 = sinf(angle) * ((x * BSCALE) - cam->x) + cosf(angle) * ((y * BSCALE) - cam->y) + cam->y;

	x = (x1 - cam->x) + (width / 2) / cam->zoom;
	y = (y1 - cam->y) + (height / 2) / cam->zoom;
}

void Box2dDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	float32 x, y;

	g2dBeginLines(G2D_STRIP);
	{
		g2dSetAlpha(150);
		g2dSetColor(RED);
		for (int i = 0; i < vertexCount; i++)
		{
			x = vertices[i].x;
			y = vertices[i].y;
			translateToCam(x, y);
			g2dSetCoordXY(x, y);
			g2dAdd();
		}
		x = vertices[0].x;
		y = vertices[0].y;
		translateToCam(x, y);
		g2dSetCoordXY(x, y);
		g2dAdd();
	}
	g2dEnd();
}

void Box2dDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{


}

void Box2dDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{

}

void Box2dDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	// draw a circle for box2d debug reasons

	float32 x = center.x;
	float32 y = center.y;
	float32 x1, y1;
	float32 newXPos, newYPos;

	g2dBeginRects(NULL);
		g2dSetCoordMode(G2D_CENTER);
		g2dSetAlpha(150);
		g2dSetColor(RED);
		translateToCam(x, y);
		g2dSetCoordXY(x, y);
		g2dSetScaleWH(2, 2);
		g2dAdd();
	g2dEnd();

	for (int i = 0; i <= 360;i++)
	{
		x1 = cosf(i) * radius*BSCALE;
		y1 = sinf(i) * radius*BSCALE;

		newXPos = x + x1;
		newYPos = y + y1;

		g2dBeginPoints();
			g2dSetAlpha(150);
			g2dSetColor(WHITE);
			g2dSetCoordXY(newXPos, newYPos);
			g2dAdd();
		g2dEnd();
	}
}

void Box2dDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	float32 x, y;

	g2dBeginLines(G2D_STRIP);
	{
		g2dSetAlpha(150);
		g2dSetColor(RED);

		x = p1.x;
		y = p1.y;
		translateToCam(x, y);
		g2dSetCoordXY(x, y);
		g2dAdd();

		x = p2.x;
		y = p2.y;
		translateToCam(x, y);
		g2dSetCoordXY(x, y);
		g2dAdd();
	}
	g2dEnd();

}

void Box2dDebugDraw::DrawTransform(const b2Transform& xf)
{

}

void Box2dDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	g2dBeginPoints();
	{
		g2dSetCoordXY(p.x, p.y);
		g2dAdd();
	}
	g2dEnd();
}