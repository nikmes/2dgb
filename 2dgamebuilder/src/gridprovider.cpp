#include "servicelocator.h"
#include "gridprovider.h"
#include "globals.h"
#include <glib2d/glib2d.h>

int GetNumCircleSegments(float r)
{
	return 10 * sqrtf(r);//change the 10 to a smaller/bigger number as needed 
}

void drawCircle(float cx, float cy, float r, int num_segments)
{
	float theta = 2 * 3.1415926 / float(num_segments);
	float c = cosf(theta); //precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = r; //we start at angle = 0 
	float y = 0;

	g2dBeginLines(G2D_STRIP);
	g2dSetColor(BLUE);

	for (int ii = 0; ii < num_segments; ii++)
	{
		g2dSetCoordXY(x + cx, y + cy);//output vertex 
 	    //apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
		g2dAdd();
	}

	x = r;
	y = 0;

	g2dSetCoordXY(x + cx, y + cy);//output vertex 

	t = x;
	x = c * x - s * y;
	y = s * t + c * y;

	g2dAdd();

	g2dEnd();
}

bool lineIntersection(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Dx, float Dy, float *X, float *Y) 
{
	double  distAB, theCos, theSin, newX, ABpos;

	//  Fail if either line is undefined.
	if (Ax == Bx && Ay == By || Cx == Dx && Cy == Dy) return false;

	//  (1) Translate the system so that point A is on the origin.
	Bx -= Ax; By -= Ay;
	Cx -= Ax; Cy -= Ay;
	Dx -= Ax; Dy -= Ay;

	//  Discover the length of segment A-B.
	distAB = sqrt(Bx*Bx + By*By);

	//  (2) Rotate the system so that point B is on the positive X axis.
	theCos = Bx / distAB;
	theSin = By / distAB;
	newX = Cx*theCos + Cy*theSin;
	Cy = Cy*theCos - Cx*theSin; Cx = newX;
	newX = Dx*theCos + Dy*theSin;
	Dy = Dy*theCos - Dx*theSin; Dx = newX;

	//  Fail if the lines are parallel.
	if (Cy == Dy) return false;

	//  (3) Discover the position of the intersection point along line A-B.
	ABpos = Dx + (Cx - Dx)*Dy / (Dy - Cy);

	//  (4) Apply the discovered position to line A-B in the original coordinate system.
	*X = Ax + ABpos*theCos;
	*Y = Ay + ABpos*theSin;

	//  Success.
	return true;
}

point rotate_point(float cx, float cy, float angle, point p)
{
	float s = sinf(angle);
	float c = cosf(angle);

	// translate point back to origin:
	p.x -= cx;
	p.y -= cy;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + cx;
	p.y = ynew + cy;
	return p;
}

bool isPointLeftOfRay(double x, double y, double raySx, double raySy, double rayEx, double rayEy) {

	//  public-domain code by Darel Rex Finley,
	//  2010.  See diagrams at http://
	//  alienryderflex.com/point_left_of_ray

	double  theCos, theSin, dist;

	//  Translate the system so that the ray
	//  starts on the origin.
	rayEx -= raySx; rayEy -= raySy;
	x -= raySx; y -= raySy;

	//  Discover the ray’s length.
	dist = sqrt(rayEx*rayEx + rayEy*rayEy);

	//  Rotate the system so that the ray
	//  points along the positive X-axis.
	theCos = rayEx / dist;
	theSin = rayEy / dist;
	y = y*theCos - x*theSin;

	//  Return the result.
	return y>0.;
}

GRIDProvider::GRIDProvider()
{
	ServiceLocator::getRenderService()->getWindowSize(&m_width, &m_height);

	m_rotationOrigin.x = m_width / 2; // rotation origin is the center of the screen initialy
	m_rotationOrigin.y = m_height / 2;
	m_rotation = 0.0f;	// no rotation applied to the grid initialy
	
	m_item = new Sprite();

	m_item->x = 512;
	m_item->y = 512;
	m_item->cW = 64;
	m_item->cH = 64;
	m_item->sW = 64;
	m_item->sH = 64;
	m_item->rotation = 0.0; // item rotation initialy is 0

	confProvider = ServiceLocator::getConfigurationService();
}

GRIDProvider::~GRIDProvider()
{
}

void GRIDProvider::draw()
{
	// should take as arguments the origin point of the roation
	// m_item = ServiceLocator::getImguiService()->spriteSelector->getCurrentSprite();
	
	point newPoint1 = { 0, 0 }, newPoint2 = { 0,0 };
	point aPoint1 = { 0,0 }, aPoint2 = { 0,0 }, bPoint1 = { 0,0 }, bPoint2 = { 0,0 };
	point currPoint;
	point intersectPoint;
	int times;
	double mX, mY;
	bool foundX = false, foundY = false;

	CameraProvider *cam = ServiceLocator::getCameraService();

	ServiceLocator::getRenderService()->getWindowSize(&m_width, &m_height);

	m_width = m_width / cam->zoom;
	m_height = m_height / cam->zoom;

	mX= ImGui::GetIO().MousePos.x;
	mY= ImGui::GetIO().MousePos.y;

	//drawCircle(m_rotationOrigin.x / cam->zoom, m_rotationOrigin.y / cam->zoom, 16.0 / cam->zoom, GetNumCircleSegments(16));
	/*
	g2dBeginLines(G2D_NULL);
	{
		g2dSetColor(BLUE);

		g2dSetCoordXY(m_rotationOrigin.x / cam->zoom, m_rotationOrigin.y / cam->zoom);
		g2dAdd();

		newPoint1.x = m_rotationOrigin.x / cam->zoom;
		newPoint1.y = m_rotationOrigin.y / cam->zoom;

		newPoint2.x = m_rotationOrigin.x / cam->zoom;
		newPoint2.y = m_rotationOrigin.y / cam->zoom - (this->m_item->sW / 2 + this->m_item->sH / 2) / 2;

		currPoint = rotate_point(newPoint1.x, newPoint1.y, m_rotation, newPoint2);

		g2dSetCoordXY(currPoint.x, currPoint.y);
		g2dAdd();
		g2dEnd();
	}

	*/

	g2dBeginLines(G2D_NULL);
	g2dSetColor(GREEN);
	g2dSetAlpha(70);

	times = (2 * m_width) / m_item->sW;

	if ((times % 2) == 0)
	{
		times += 1;
	}

	for (float x = ((m_width / 2) - (m_item->sW / 2)*times); x < m_width * 2; x += m_item->sW)
	{
		currPoint.x = x;
		currPoint.y = -m_height;
		newPoint1 = rotate_point(m_rotationOrigin.x, m_rotationOrigin.y, m_rotation, currPoint);

		currPoint.x = x;
		currPoint.y = m_height * 2;
		newPoint2 = rotate_point(m_rotationOrigin.x, m_rotationOrigin.y, m_rotation, currPoint);

		if (isPointLeftOfRay(mX / cam->zoom, mY / cam->zoom, newPoint1.x, newPoint1.y, newPoint2.x, newPoint2.y) && !foundX)
		{
			aPoint1 = newPoint1;
			aPoint2 = newPoint2;

			if (!ImGui::GetIO().WantCaptureMouse)
			{
				g2dSetColor(RED);
				g2dSetAlpha(255);
			}

			foundX = true;
		}
		else
		{
			g2dSetAlpha(70);
			g2dSetColor(GREEN);
		}

		if (confProvider->pEditor_drawwireframe)
		{
			g2dSetCoordXY(newPoint1.x, newPoint1.y);
			g2dAdd();

			g2dSetCoordXY(newPoint2.x, newPoint2.y);
			g2dAdd();
		}

	}

	g2dSetColor(GREEN);
	g2dSetAlpha(70);
	times = (2 * m_height) / m_item->sH;

	if ((times % 2) == 0)
	{
		times += 1;
	}

	for (float y = ((m_height / 2) - (m_item->sH / 2) * times); y < m_height * 2; y += m_item->sH)
	{
		currPoint.x = -m_width;
		currPoint.y = y;
		newPoint1 = rotate_point(m_rotationOrigin.x, m_rotationOrigin.y, m_rotation, currPoint);

		currPoint.x = m_width * 2;
		currPoint.y = y;
		newPoint2 = rotate_point(m_rotationOrigin.x, m_rotationOrigin.y, m_rotation, currPoint);

		if (!isPointLeftOfRay(mX / cam->zoom, mY / cam->zoom, newPoint1.x, newPoint1.y, newPoint2.x, newPoint2.y) && !foundY)
		{
			bPoint1 = newPoint1;
			bPoint2 = newPoint2;

			if (!ImGui::GetIO().WantCaptureMouse)
			{
				g2dSetColor(RED);
				g2dSetAlpha(255);
			}

			foundY = true;
		}
		else
		{
			g2dSetAlpha(70);
			g2dSetColor(GREEN);
		}
		if (confProvider->pEditor_drawwireframe)
		{
			g2dSetCoordXY(newPoint1.x, newPoint1.y);
			g2dAdd();

			g2dSetCoordXY(newPoint2.x, newPoint2.y);
			g2dAdd();
		}
	}
	g2dEnd();

	// we have 2 points from each intersecting line, lets find the intersection point

	lineIntersection(aPoint1.x, aPoint1.y, aPoint2.x, aPoint2.y, bPoint1.x, bPoint1.y, bPoint2.x, bPoint2.y, &m_item->x, &m_item->y);

	// reverse rotation

	currPoint.x = m_item->x;
	currPoint.y = m_item->y;

	newPoint1 = rotate_point(m_rotationOrigin.x, m_rotationOrigin.y, -m_rotation, currPoint);

	m_item->x = newPoint1.x;
	m_item->y = newPoint1.y;

	// find the center
	m_item->x -= m_item->sW / 2;
	m_item->y -= m_item->sH / 2;

	// rotate the center
	currPoint.x = m_item->x;
	currPoint.y = m_item->y;

	newPoint1 = rotate_point(m_rotationOrigin.x, m_rotationOrigin.y, m_rotation, currPoint);

	m_item->x = newPoint1.x;
	m_item->y = newPoint1.y;

	SubState subState = ServiceLocator::getStateService()->getSubState();

	if (!ImGui::GetIO().WantCaptureMouse && (subState == EST_DRAWFREEHAND || subState == EST_DRAWTILED))
	{
		float layerDepth;

		Layer *currentLayer = ServiceLocator::getGameService()->getCurrentLayer();

		if (currentLayer)
		{
			layerDepth = currentLayer->zDepth;
		}

		if (ServiceLocator::getStateService()->getSubState() == EST_DRAWFREEHAND)
		{
			// if freehand replace what you calculated with mouse coordinates
			m_item->x = mX;
			m_item->y = mY;
		}

		SpriteBatch *spriteBatch = ServiceLocator::getImguiService()->spriteSelector->getSpriteBatch();

		if (spriteBatch)
		{
			spriteBatch->beginDraw();
			g2dSetCoordMode(G2D_CENTER);

			if (ServiceLocator::getStateService()->getSubState() == EST_DRAWFREEHAND)
			{
				g2dSetCoordXYZ(m_item->x / cam->zoom, m_item->y / cam->zoom, 999.9);
			}
			else
			{
				g2dSetCoordXYZ(m_item->x, m_item->y, 999.9);
				// we devided earliser by zoom factor so multiply now
				m_item->x = m_item->x * cam->zoom;
				m_item->y = m_item->y * cam->zoom;
			}

			g2dSetCropXY(m_item->cX, m_item->cY);
			g2dSetCropWH(m_item->cW, m_item->cH);
			g2dSetRotationRad(m_item->rotation);
			g2dSetScaleWH(m_item->sW, m_item->sH);
			g2dSetAlpha(m_item->alpha);
			g2dSetColor(m_item->color);
			g2dAdd();

			spriteBatch->endDraw();
		}

	}

	//drawCircle(m_rotationOrigin.x / cam->zoom, m_rotationOrigin.y / cam->zoom, 16.0 / cam->zoom, GetNumCircleSegments(16));
	drawCircle(camX / cam->zoom, camY / cam->zoom, 32.0 / cam->zoom, GetNumCircleSegments(16));
}


Sprite* GRIDProvider::getCurrentSprite()
{
	return m_item;
}

void GRIDProvider::drawOrtho()
{/*
	point newPoint1,newPoint2;
	point currPoint;
	int times;
	static float angle = 0.0;
	double mX, mY;
	
	ServiceLocator::getVideoService()->getGetWindowSize(&m_width, &m_height);

	glfwGetCursorPos(gWin, &mX, &mY);

	g2dBeginLines(G2D_NULL);
	g2dSetColor(YELLOW);
	g2dSetAlpha(15);

	times = (2 * m_width) / m_item.w;

	if ((times % 2) == 0)
	{
		times += 1;
	}

	for (float x = ((m_width/2) - (m_item.sW / 2)*times); x < m_width* 2; x += m_item.sW)
	{
		currPoint.x = x;
		currPoint.y = -m_height;
		newPoint1 = rotate_point(m_rotationOrigin.x, m_rotationOrigin.y, angle, currPoint);

		currPoint.x = x;
		currPoint.y = m_height * 2;
		newPoint2 = rotate_point(m_rotationOrigin.x, m_rotationOrigin.y, angle, currPoint);
		if (confProvider->pEditor_drawwireframe)
		{
			g2dSetCoordXY(newPoint1.x, newPoint1.y);
			g2dAdd();

			g2dSetCoordXY(newPoint2.x, newPoint2.y);
			g2dAdd();
		}
	}

	times = (2 * m_height) / m_item.sH;

	if ((times % 2) == 0)
	{
		times += 1;
	}

	for (float y = ((m_height / 2) - (m_item.sH / 2) * times); y < m_height*2; y += m_item.sH)
	{
		currPoint.x = -m_width;
		currPoint.y = y;
		newPoint1 = rotate_point(m_rotationOrigin.x, m_rotationOrigin.y, angle, currPoint);

		currPoint.x = m_width * 2;
		currPoint.y = y;
		newPoint2 = rotate_point(m_rotationOrigin.x, m_rotationOrigin.y, angle, currPoint);

		if (confProvider->pEditor_drawwireframe)
		{
			g2dSetCoordXY(newPoint1.x, newPoint1.y);
			g2dAdd();

			g2dSetCoordXY(newPoint2.x, newPoint2.y);
			g2dAdd();
		}
	}

	g2dEnd();
	*/
}

void GRIDProvider::setGridRotationOrigin(float x, float y)
{
	m_rotationOrigin.x = x;
	m_rotationOrigin.y = y;
}

void GRIDProvider::setGridRotation(float rot)
{
	m_rotation = rot;
}

void GRIDProvider::setItem(Sprite *item)
{
	m_item->alpha = item->alpha;
	m_item->cH = item->cH;
	m_item->col[0] = item->col[0];
	m_item->col[1] = item->col[1];
	m_item->col[2] = item->col[2];
	m_item->col[3] = item->col[3];
	m_item->color = item->color;
	m_item->cW = item->cW;
	m_item->cX = item->cX;
	m_item->cY = item->cY;
	m_item->sH = item->sH;
	m_item->sW = item->sW;
	m_item->scale = item->scale;
	m_item->rotation = item->rotation;
	m_item->name = item->name;
	m_item->sprBatch = item->sprBatch;
}

void GRIDProvider::setGridItemRotation(float rot)
{
	m_item->rotation = rot;
}

void GRIDProvider::setGridItemScale(float scale)
{
	m_item->setScale(scale);
}

void GRIDProvider::destroy()
{
}