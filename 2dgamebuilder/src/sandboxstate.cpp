/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file introstate.cpp
*
*/

#include <stdio.h>
#include <glib2d/glib2d.h>
#include "sandboxstate.h"
#include "globals.h"

#define TINYC2_IMPL
#include "tinyc2.h"

point r_point(float cx, float cy, float angle, point p)
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

CSandBoxState CSandBoxState::m_SandBoxState;

void CSandBoxState::init(Application *app)
{
	m_app = app;
	m_app->m_currentState = SANDBOX;
	ServiceLocator::getStateService()->setState(SANDBOX);
	io = ImGui::GetIO();
	TRACE(T_INF, "Init function for SandBoxState finished");

	m_mySprite.x = 1920 / 2;
	m_mySprite.y = 1200 / 2;
	m_mySprite.sH = 256;
	m_mySprite.sW = 256;

	// setup some models

	user_capsule.a = c2V(-30.0f, 0);
	user_capsule.b = c2V(30.0f, 0);
	user_capsule.r = 10.0f;

}

void CSandBoxState::cleanup()
{
}

void CSandBoxState::pause()
{
}

void CSandBoxState::resume()
{
}

void CSandBoxState::handleEvents()
{

}

void CSandBoxState::update(float tpf)
{
	float speed = 8;
	double xMouse, yMouse;
	int width, height;
	
	m_app->renderProvider->getWindowSize(&width, &height);

	glfwGetCursorPos(gWin, &xMouse, &yMouse);

	//float mouse_x = (float)xMouse - width / 2;
	
	//float mouse_y = -((float)yMouse - height / 2);
	
	//mp = c2V(mouse_x, mouse_y);

	mp = c2V(xMouse, yMouse);

	user_circle.p = mp;
	
	user_circle.r = 10.0f;

	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (glfwGetKey(gWin, GLFW_KEY_UP) == GLFW_PRESS)
		{
			m_mySprite.y -= speed;
			//code = (code + 1) % 11;
		}

		if (glfwGetKey(gWin, GLFW_KEY_0) == GLFW_PRESS)
		{
			code = 0;
		}

		if (glfwGetKey(gWin, GLFW_KEY_1) == GLFW_PRESS)
		{
			code = 1;
		}

		if (glfwGetKey(gWin, GLFW_KEY_2) == GLFW_PRESS)
		{
			code = 2;
		}

		if (glfwGetKey(gWin, GLFW_KEY_3) == GLFW_PRESS)
		{
			code = 3;
		}

		if (glfwGetKey(gWin, GLFW_KEY_4) == GLFW_PRESS)
		{
			code = 4;
		}

		if (glfwGetKey(gWin, GLFW_KEY_5) == GLFW_PRESS)
		{
			code = 5;
		}

		if (glfwGetKey(gWin, GLFW_KEY_6) == GLFW_PRESS)
		{
			code = 6;
		}

		if (glfwGetKey(gWin, GLFW_KEY_7) == GLFW_PRESS)
		{
			code = 7;
		}

		if (glfwGetKey(gWin, GLFW_KEY_8) == GLFW_PRESS)
		{
			code = 8;
		}

		if (glfwGetKey(gWin, GLFW_KEY_9) == GLFW_PRESS)
		{
			code = 9;
		}

		if (glfwGetKey(gWin, GLFW_KEY_Q) == GLFW_PRESS)
		{
			code = 10;
		}

		if (glfwGetKey(gWin, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			m_mySprite.y += speed;
		}

		if (glfwGetKey(gWin, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			m_mySprite.x += speed;
		}

		if (glfwGetKey(gWin, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			m_mySprite.x -= speed;
		}
	}

	rot += 1;



}

void CSandBoxState::draw()
{
	/*g2dBeginRects(NULL);
	{
		g2dSetCoordMode(G2D_CENTER);
		g2dSetColor(RED);
		g2dSetCoordXY(m_mySprite.x, m_mySprite.y);
		g2dSetScaleWH(m_mySprite.sW, m_mySprite.sH);
		g2dSetRotation(rot);
		g2dAdd();
		g2dEnd();
	}*/
	switch (code)
	{
	case 0:  TestDrawPrim(); break;
	case 1:  TestBoolean0(); break;
	case 2:  TestBoolean1(); break;
	case 3:  TestBoolean2(); break;
	case 4:  TestRay0(); break;
	case 5:  TestRay1(); break;
	case 6:  TestRay2(); break;
	case 7:  TestManifold0(); break;
	case 8:  TestManifold1(); break;
	case 9:  TestManifold2(); break;
	case 10: PlastburkRayBug(); break;
	}

	if (ImGui::GetIO().MouseWheel)
	{
		Rotate((c2v*)&user_capsule, (c2v*)&user_capsule, 2);
	}
}

float CSandBoxState::randf()
{
	float r = (float)(rand() & RAND_MAX);
	r /= RAND_MAX;
	r = 5.0f * r +3.0f;
	return r;
}

c2v CSandBoxState::RandomVec()
{
	return c2V(randf() * 100.0f, randf() * 100.0f);
}

void CSandBoxState::tgLine(float pX1, float pY1, float pZ1, float pX2, float pY2, float pZ2)
{
	g2dBeginLines(G2D_NULL);
	{
		g2dSetColor(lineColor);
		g2dSetCoordXY(pX1, pY1);
		g2dAdd();
		g2dSetCoordXY(pX2, pY2);
		g2dAdd();
	}
	g2dEnd();
}

void CSandBoxState::DrawPoly(c2v* verts, int count)
{
	for (int i = 0; i < count; ++i)
	{
		int iA = i;
		int iB = (i + 1) % count;
		c2v a = verts[iA];
		c2v b = verts[iB];
		tgLine(a.x, a.y, 0, b.x, b.y, 0);
	}
}

void CSandBoxState::DrawNormals(c2v* verts, c2v* norms, int count)
{
	for (int i = 0; i < count; ++i)
	{
		int iA = i;
		int iB = (i + 1) % count;
		c2v a = verts[iA];
		c2v b = verts[iB];
		c2v p = c2Mulvs(c2Add(a, b), 0.5f);
		c2v n = norms[iA];
		tgLine(p.x, p.y, 0, p.x + n.x, p.y + n.y, 0);
	}
}

void CSandBoxState::DrawPoly2(c2Poly* p, c2x x)
{
	for (int i = 0; i < p->count; ++i)
	{
		int iA = i;
		int iB = (i + 1) % p->count;
		c2v a = c2Mulxv(x, p->verts[iA]);
		c2v b = c2Mulxv(x, p->verts[iB]);
		tgLine(a.x, a.y, 0, b.x, b.y, 0);
	}
}

void CSandBoxState::DrawAABB(c2v a, c2v b)
{
	c2v c = c2V(a.x, b.y);
	c2v d = c2V(b.x, a.y);
	
	tgLine(a.x, a.y, 0, c.x, c.y, 0);
	tgLine(c.x, c.y, 0, b.x, b.y, 0);
	tgLine(b.x, b.y, 0, d.x, d.y, 0);
	tgLine(d.x, d.y, 0, a.x, a.y, 0);
}

void CSandBoxState::DrawHalfCircle(c2v a, c2v b)
{
	c2v u = c2Sub(b, a);
	float r = c2Len(u);
	u = c2Skew(u);
	c2v v = c2CCW90(u);
	c2v s = c2Add(v, a);
	c2m m;
	m.x = c2Norm(u);
	m.y = c2Norm(v);

	int kSegs = 20;
	float theta = 0;
	float inc = 3.14159265f / (float)kSegs;
	c2v p0;
	c2SinCos(theta, &p0.y, &p0.x);
	p0 = c2Mulvs(p0, r);
	p0 = c2Add(c2Mulmv(m, p0), a);
	for (int i = 0; i < kSegs; ++i)
	{
		theta += inc;
		c2v p1;
		c2SinCos(theta, &p1.y, &p1.x);
		p1 = c2Mulvs(p1, r);
		p1 = c2Add(c2Mulmv(m, p1), a);
		tgLine(p0.x, p0.y, 0, p1.x, p1.y, 0);
		p0 = p1;
	}
}

void CSandBoxState::DrawCapsule(c2v a, c2v b, float r)
{
	c2v n = c2Norm(c2Sub(b, a));
	DrawHalfCircle(a, c2Add(a, c2Mulvs(n, -r)));
	DrawHalfCircle(b, c2Add(b, c2Mulvs(n, r)));
	c2v p0 = c2Add(a, c2Mulvs(c2Skew(n), r));
	c2v p1 = c2Add(b, c2Mulvs(c2CCW90(n), -r));
	tgLine(p0.x, p0.y, 0, p1.x, p1.y, 0);
	p0 = c2Add(a, c2Mulvs(c2Skew(n), -r));
	p1 = c2Add(b, c2Mulvs(c2CCW90(n), r));
	tgLine(p0.x, p0.y, 0, p1.x, p1.y, 0);
}

void CSandBoxState::DrawCircle(c2v p, float r)
{
	int kSegs = 40;
	float theta = 0;
	float inc = 3.14159265f * 2.0f / (float)kSegs;
	float px, py;
	c2SinCos(theta, &py, &px);
	px *= r; py *= r;
	px += p.x; py += p.y;
	for (int i = 0; i <= kSegs; ++i)
	{
		theta += inc;
		float x, y;
		c2SinCos(theta, &y, &x);
		x *= r; y *= r;
		x += p.x; y += p.y;
		tgLine(x, y, 0, px, py, 0);
		px = x; py = y;
	}
}

// should see slow rotation CCW, then CW space toggles between two different rotation implements after toggling implementations space toggles rotation direction

void CSandBoxState::TestRotation()
{
	static int first = 1;
	static Vertex v[3];
	
	if (first)
	{
		first = 0;
		
		v[0].col = GREEN;
		v[1].col = GREEN;
		v[2].col = GREEN;
		
		v[0].pos = c2V(0, 100);
		v[1].pos = c2V(0, 0);
		v[2].pos = c2V(100, 0);
	}

	static int which0;

	static int which1;

	if (glfwGetKey(gWin, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		which0 = !which0;
	}
	if ((glfwGetKey(gWin, GLFW_KEY_SPACE) == GLFW_PRESS) && which0)
	{
		which1 = !which1;
	}

	if (which0)
	{
		c2m m;
		m.x = c2Norm(c2V(1, 0.01f));
		m.y = c2Skew(m.x);
		for (int i = 0; i < 3; ++i)
			v[i].pos = which1 ? c2Mulmv(m, v[i].pos) : c2MulmvT(m, v[i].pos);
	}

	else
	{
		c2r r = c2Rot(0.01f);
		for (int i = 0; i < 3; ++i)
			v[i].pos = which1 ? c2Mulrv(r, v[i].pos) : c2MulrvT(r, v[i].pos);
	}

	for (int i = 0; i < 3; ++i)
		verts.push_back(v[i]);
}

void CSandBoxState::TestDrawPrim()
{
	TestRotation();

	//tgLineColor(ctx, 0.2f, 0.6f, 0.8f);

	tgLine(0, 0, 0, 100, 100, 0);
	
	//tgLineColor(ctx, 0.8f, 0.6f, 0.2f);
	
	tgLine(100, 100, 0, -100, 200, 0);

	DrawCircle(c2V(0, 0), 100.0f);

	//tgLineColor(ctx, 0, 1.0f, 0);
	
	DrawHalfCircle(c2V(0, 0), c2V(50, -50));

	//tgLineColor(ctx, 0, 0, 1.0f);
	
	DrawCapsule(c2V(0, 200), c2V(75, 150), 20.0f);

	//tgLineColor(ctx, 1.0f, 0, 0);

	DrawAABB(c2V(-20, -20), c2V(20, 20));

	//tgLineColor(ctx, 0.5f, 0.9f, 0.1f);

	c2v poly[] = {
		{ 0, 0 },
		{ 20.0f, 10.0f },
		{ 5.0f, 15.0f },
		{ -3.0f, 7.0f },
	};
	DrawPoly(poly, 4);
}

void CSandBoxState::TestBoolean0()
{
	c2AABB aabb;
	aabb.min = c2V(1000, 500);
	aabb.max = c2V(1064, 564);


	c2AABB aabb2;
	aabb2.min = c2V(1020, 500);
	aabb2.max = c2V(1100, 540);

	c2Circle circle;
	circle.p = c2V(-70.0f+1000, 0+500);
	circle.r = 20.0f;

	c2Capsule capsule;
	capsule.a = c2V(-40.0f+1000, 40.0f+500);
	capsule.b = c2V(-20.0f+1000, 100.0f+500);
	capsule.r = 10.0f;

	if (c2CircletoCircle(user_circle, circle))
	{
		lineColor = GREEN;
		//tgLineColor(ctx, 1.0f, 0.0f, 0.0f);
	}
	else
	{
		lineColor = BLUE;
		//tgLineColor(ctx, 5.0f, 7.0f, 9.0f);
	}

	DrawCircle(circle.p, circle.r);

	g2dBeginRects(ServiceLocator::getTextureService()->GetTextureFromFileSystem("resources/myBall.png"));
	{
		g2dSetCoordMode(G2D_CENTER);	
		g2dSetScaleWH(40, 40);
		g2dSetCoordXY(circle.p.x, circle.p.y);
		g2dAdd();
	}
	g2dEnd();

	if (c2CircletoAABB(user_circle, aabb))
	{
		lineColor = GREEN;
		//tgLineColor(ctx, 1.0f, 0.0f, 0.0f);
	}
	else
	{
		lineColor = BLUE;
		//tgLineColor(ctx, 5.0f, 7.0f, 9.0f);
	}

	g2dBeginRects(ServiceLocator::getTextureService()->GetTextureFromFileSystem("resources/sprite.png"));
	{
		g2dSetCoordMode(G2D_CENTER);
		//g2dSetScaleWH(40, 40);
		g2dSetCoordXY(1032, 532); // aabb.max.y); /// -(aabb.max.x - aabb.min.x) / 2, aabb.max.y - (aabb.max.y - aabb.min.y) / 2);
		//g2dSetColor(CYAN);
		g2dSetRotation(45);
		g2dSetAlpha(100);
		g2dAdd();
	}
	g2dEnd();
	/*point p;
	p.x = 1032;
	p.y = 532;

	point a = r_point(aabb.min.x, aabb.min.y, 45, p);
	point b = r_point(aabb.max.x, aabb.max.y, 45, p);

	aabb.min.x = a.x;
	aabb.min.y = a.y;
	aabb.max.x = b.x;
	aabb.max.y = b.y;*/

	DrawAABB(aabb.min, aabb.max);

	if (c2CircletoAABB(user_circle, aabb2))
	{
		lineColor = GREEN;
		//tgLineColor(ctx, 1.0f, 0.0f, 0.0f);
	}
	else
	{
		lineColor = BLUE;
		//tgLineColor(ctx, 5.0f, 7.0f, 9.0f);
	}

	DrawAABB(aabb2.min, aabb2.max);

	if (c2CircletoCapsule(user_circle, capsule))
	{
		lineColor = GREEN;
		//tgLineColor(ctx, 1.0f, 0.0f, 0.0f);
	}
	else
	{
		lineColor = BLUE;
		//tgLineColor(ctx, 5.0f, 7.0f, 9.0f);
	}

	DrawCapsule(capsule.a, capsule.b, capsule.r);

	lineColor = BLUE;
	//tgLineColor(ctx, 0.5f, 0.7f, 0.9f);

	DrawCircle(user_circle.p, user_circle.r);
}

void CSandBoxState::TestBoolean1()
{
	c2AABB bb;
	bb.min = c2V(1000-100.0f, 500-30.0f);
	bb.max = c2V(1000-50.0f, 500+30.0f);
	c2Capsule cap = GetCapsule();

	c2v a, b;
	c2GJK(&bb, C2_AABB, 0, &cap, C2_CAPSULE, 0, &a, &b, 1);
	DrawCircle(a, 2.0f);
	DrawCircle(b, 2.0f);
	tgLine(a.x, a.y, 0, b.x, b.y, 0);

	if (c2AABBtoCapsule(bb, cap))
	{
		lineColor = GREEN;
		//tgLineColor(ctx, 1.0f, 0.0f, 0.0f);
	}
	else
	{
		lineColor = BLUE;
		//tgLineColor(ctx, 5.0f, 7.0f, 9.0f);
	}
	DrawAABB(bb.min, bb.max);

	lineColor = BLUE;
	//tgLineColor(ctx, 0.5f, 0.7f, 0.9f);
	DrawCapsule(cap.a, cap.b, cap.r);
}

void CSandBoxState::TestBoolean2()
{
	static c2Poly poly;
	static c2Poly poly2;
	static int first = 1;
	if (first)
	{
		first = 0;
		poly.count = C2_MAX_POLYGON_VERTS;
		for (int i = 0; i < poly.count; ++i) poly.verts[i] = RandomVec();
		poly.count = c2Hull(poly.verts, poly.count);
		poly2.count = C2_MAX_POLYGON_VERTS;
		
		for (int i = 0; i < poly2.count; ++i)
		{
			poly2.verts[i] = RandomVec();
		}

		poly2.count = c2Hull(poly2.verts, poly2.count);
	}

	static int which;
	
	if (glfwGetKey(gWin, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		which = (which + 1) % 4;
	}

	if (ImGui::GetIO().MouseWheel>0)
	{
		Rotate(poly2.verts, poly2.verts, poly2.count);
	}
	switch (which)
	{
	case 0:
	{
		c2v a, b;
		c2GJK(&user_circle, C2_CIRCLE, 0, &poly, C2_POLY, 0, &a, &b, 1);
		DrawCircle(a, 2.0f);
		DrawCircle(b, 2.0f);
		tgLine(a.x, a.y, 0, b.x, b.y, 0);

		if (c2CircletoPoly(user_circle, &poly, 0))
		{
			lineColor = GREEN;
			//tgLineColor(ctx, 1.0f, 0.0f, 0.0f);
		}
		else
		{
			lineColor = BLUE;
			//tgLineColor(ctx, 5.0f, 7.0f, 9.0f);
		}
		DrawPoly(poly.verts, poly.count);

		lineColor = BLUE;
		//tgLineColor(ctx, 0.5f, 0.7f, 0.9f);
		DrawCircle(user_circle.p, user_circle.r);
	}	break;

	case 1:
	{
		c2v a, b;
		c2AABB bb;
		bb.min = c2V(1000+-10.0f, 1000-10.0f);
		bb.max = c2V(1000+10.0f, 1000+10.0f);
		bb.min = c2Add(bb.min, mp);
		bb.max = c2Add(bb.max, mp);
		c2GJK(&bb, C2_AABB, 0, &poly, C2_POLY, 0, &a, &b, 1);
		DrawCircle(a, 2.0f);
		DrawCircle(b, 2.0f);
		tgLine(a.x, a.y, 0, b.x, b.y, 0);

		if (c2AABBtoPoly(bb, &poly, 0))
		{
			lineColor = GREEN;
			//tgLineColor(ctx, 1.0f, 0.0f, 0.0f);
		}
		else
		{
			lineColor = BLUE;
			//tgLineColor(ctx, 5.0f, 7.0f, 9.0f);
		}
		DrawPoly(poly.verts, poly.count);
		//tgLineColor(ctx, 0.5f, 0.7f, 0.9f);
		lineColor = BLUE;
		DrawAABB(bb.min, bb.max);
	}	break;

	case 2:
	{
		c2v a, b;
		c2Capsule cap = GetCapsule();
		c2GJK(&cap, C2_CAPSULE, 0, &poly, C2_POLY, 0, &a, &b, 1);
		DrawCircle(a, 2.0f);
		DrawCircle(b, 2.0f);
		tgLine(a.x, a.y, 0, b.x, b.y, 0);

		if (c2CapsuletoPoly(cap, &poly, 0))
		{
			lineColor = GREEN;
			//tgLineColor(ctx, 1.0f, 0.0f, 0.0f);
		}
		else
		{
			lineColor = BLUE;
			//tgLineColor(ctx, 5.0f, 7.0f, 9.0f);
		}

		DrawPoly(poly.verts, poly.count);

		//tgLineColor(ctx, 0.5f, 0.7f, 0.9f);
		lineColor = BLUE;
		DrawCapsule(cap.a, cap.b, cap.r);
	}	break;

	case 3:
	{
		c2v a, b;
		c2Poly poly3;
		for (int i = 0; i < poly2.count; ++i) poly3.verts[i] = c2Add(mp, poly2.verts[i]);
		poly3.count = poly2.count;

		c2GJK(&poly, C2_POLY, 0, &poly3, C2_POLY, 0, &a, &b, 1);
		DrawCircle(a, 2.0f);
		DrawCircle(b, 2.0f);
		tgLine(a.x, a.y, 0, b.x, b.y, 0);

		if (c2PolytoPoly(&poly, 0, &poly3, 0))
		{
			lineColor = GREEN;
			//tgLineColor(ctx, 1.0f, 0.0f, 0.0f);
		}
		else
		{
			lineColor = BLUE;
			//tgLineColor(ctx, 5.0f, 7.0f, 9.0f);
		}

		DrawPoly(poly.verts, poly.count);

		lineColor = BLUE;
		//tgLineColor(ctx, 0.5f, 0.7f, 0.9f);
		DrawPoly(poly3.verts, poly3.count);
	}	break;
	}
}

void CSandBoxState::TestRay0()
{
	c2Circle c;
	c.p = c2V(1920/2, 1200/2);
	c.r = 20.0f;

	c2AABB bb;
	bb.min = c2V(1000+30.0f, 1000+30.0f);
	bb.max = c2V(1000+70.0f, 1000+70.0f);

	c2Ray ray;
	ray.p = c2V(-100.0f, 500.0f);
	ray.d = c2Norm(c2Sub(mp, ray.p));
	ray.t = c2Dot(mp, ray.d) - c2Dot(ray.p, ray.d);

	//tgLineColor(1.0f, 1.0f, 1.0f);
	lineColor = WHITE;
	DrawCircle(c.p, c.r);

	DrawAABB(bb.min, bb.max);

	c2Raycast cast;
	int hit = c2RaytoCircle(ray, c, &cast);
	
	if (hit)
	{
		ray.t = cast.t;
		c2v impact = c2Impact(ray, ray.t);
		
		c2v end = c2Add(impact, c2Mulvs(cast.n, 15.0f));

		//tgLineColor(ctx, 1.0f, 0.2f, 0.4f);
		lineColor = MAGENTA;
		tgLine(impact.x, impact.y, 0, end.x, end.y, 0);
		tgLine(ray.p.x, ray.p.y, 0, ray.p.x + ray.d.x * ray.t, ray.p.y + ray.d.y * ray.t, 0);
	}
	else
	{

		ray.d = c2Norm(c2Sub(mp, ray.p));
		ray.t = c2Dot(mp, ray.d) - c2Dot(ray.p, ray.d);

		if (c2RaytoAABB(ray, bb, &cast))
		{
			ray.t = cast.t;
			c2v impact = c2Impact(ray, ray.t);
			c2v end = c2Add(impact, c2Mulvs(cast.n, 15.0f));
			//tgLineColor(1.0f, 0.2f, 0.4f);
			lineColor = MAGENTA;
			tgLine(impact.x, impact.y, 0, end.x, end.y, 0);
		}
		else
		{
			lineColor = WHITE;
			//tgLineColor(ctx, 1.0f, 1.0f, 1.0f);
		}

		tgLine(ray.p.x, ray.p.y, 0, ray.p.x + ray.d.x * ray.t, ray.p.y + ray.d.y * ray.t, 0);
	}
}

void CSandBoxState::TestRay1()
{
	c2Capsule cap;
	cap.a = c2V(1000-100.0f, 600+60.0f);
	cap.b = c2V(1000+50.0f, 600-40.0f);
	cap.r = 20.0f;

	c2Ray ray;
	ray.p = c2V(475.0f, 100.0f);
	ray.d = c2Norm(c2Sub(mp, ray.p));
	ray.t = c2Dot(mp, ray.d) - c2Dot(ray.p, ray.d);

	//tgLineColor(ctx, 1.0f, 1.0f, 1.0f);
	lineColor = WHITE;
	DrawCapsule(cap.a, cap.b, cap.r);
	c2Raycast cast;
	if (c2RaytoCapsule(ray, cap, &cast))
	{
		ray.t = cast.t;
		c2v impact = c2Impact(ray, ray.t);
		c2v end = c2Add(impact, c2Mulvs(cast.n, 15.0f));
		//tgLineColor(ctx, 1.0f, 0.2f, 0.4f);
		lineColor = MAGENTA;
		tgLine(impact.x, impact.y, 0, end.x, end.y, 0);
	}

	tgLine(ray.p.x, ray.p.y, 0, ray.p.x + ray.d.x * ray.t, ray.p.y + ray.d.y * ray.t, 0);
}

void CSandBoxState::TestRay2()
{
	static c2Poly poly;
	static int first = 1;
	if (first)
	{
		first = 0;
		poly.count = C2_MAX_POLYGON_VERTS;
		for (int i = 0; i < poly.count; ++i) poly.verts[i] = RandomVec();
		c2MakePoly(&poly);
	}

	//tgLineColor(ctx, 1.0f, 1.0f, 1.0f);
	lineColor = WHITE;
	DrawPoly(poly.verts, poly.count);

	c2Ray ray;
	ray.p = c2V(-75.0f, 100.0f);
	ray.d = c2Norm(c2Sub(mp, ray.p));
	ray.t = c2Dot(mp, ray.d) - c2Dot(ray.p, ray.d);

	c2Raycast cast;
	if (c2RaytoPoly(ray, &poly, 0, &cast))
	{
		ray.t = cast.t;
		c2v impact = c2Impact(ray, ray.t);
		c2v end = c2Add(impact, c2Mulvs(cast.n, 15.0f));
		lineColor = MAGENTA;
		//tgLineColor(ctx, 1.0f, 0.2f, 0.4f);
		tgLine(impact.x, impact.y, 0, end.x, end.y, 0);
	}

	tgLine(ray.p.x, ray.p.y, 0, ray.p.x + ray.d.x * ray.t, ray.p.y + ray.d.y * ray.t, 0);
}

void CSandBoxState::TestManifold0()
{
	c2Circle ca;
	ca.p = c2V(500-200.0f, 500+0);
	ca.r = 20.0f;
	c2Circle cb;
	cb.p = c2V(500-220.0f, 500+10.0f);
	cb.r = 15.0f;
	DrawCircles(ca, cb);

	cb.p = ca.p;
	cb.r = 10.0f;
	DrawCircles(ca, cb);

	c2AABB bb;
	bb.min = c2V(500-150.0f, 500+20.0f);
	bb.max = c2V(500-60.0f, 500+140.0f);

	// outside
	ca.p = c2V(500-160.0f, 500+80.0f);
	ca.r = 15.0f;
	DrawCircleAABB(ca, bb);

	ca.p = c2V(500-120.0f,500+ 150.0f);
	ca.r = 15.0f;
	DrawCircleAABB(ca, bb);

	ca.p = c2V(500-50.0f, 500+100.0f);
	ca.r = 15.0f;
	DrawCircleAABB(ca, bb);

	ca.p = c2V(500-120.0f,500+ 10.0f);
	ca.r = 15.0f;
	DrawCircleAABB(ca, bb);

	// inside
	ca.p = c2V(500-140.0f, 500+60.0f);
	ca.r = 10.0f;
	DrawCircleAABB(ca, bb);

	ca.p = c2V(500-100.0f, 500+40.0f);
	ca.r = 10.0f;
	DrawCircleAABB(ca, bb);

	ca.p = c2V(500-80.0f,500+ 70.0f);
	ca.r = 10.0f;
	DrawCircleAABB(ca, bb);

	ca.p = c2V(500-80.0f, 500+130.0f);
	ca.r = 10.0f;
	DrawCircleAABB(ca, bb);

	// capsule things
	c2Capsule cap;
	cap.a = c2V(500+100.0f,500+ 0);
	cap.b = c2V(500+250.0f,500+ 50.0f);
	cap.r = 20.0f;
	ca.p = c2V(500+120.0f, 500+30.0f);
	ca.r = 25.0f;
	DrawCircleCapsule(ca, cap);

	ca.p = c2V(500+150.0f, 500 + 45.0f);
	ca.r = 15.0f;
	DrawCircleCapsule(ca, cap);

	ca.p = c2V(500 + 100.0f, 500 + 0);
	ca.r = 15.0f;
	DrawCircleCapsule(ca, cap);

	ca.p = c2V(500 + 260.0f, 500 + 60.0f);
	ca.r = 10.0f;
	DrawCircleCapsule(ca, cap);

	// bb things
	c2AABB ba;
	ba.min = c2V(500 -50.0f, 500 -200.0f);
	ba.max = c2V(500 + 50.0f, 500 -100.0f);
	bb.min = c2V(500 -10.0f, 500 -110.0f);
	bb.max = c2V(500 + 10.0f, 500 -80.0f);
	DrawBB(ba, bb);

	bb.min = c2V(500+20.0f, 500-140.0f);
	bb.max = c2V(500+40.0f, 500-110.0f);
	DrawBB(ba, bb);

	bb.min = c2V(500-20.0f, 500-140.0f);
	bb.max = c2V(500-40.0f, 500-110.0f);
	DrawBB(ba, bb);

	bb.min = c2V(500-10.0f, 500-205.0f);
	bb.max = c2V(500+10.0f, 500-190.0f);
	DrawBB(ba, bb);

	//bb.min = c2Sub( mp, c2V( 20.0f, 5.0f ) );
	//bb.max = c2Add( mp, c2V( 20.0f, 5.0f ) );
	//DrawBB( ba, bb );
}

void CSandBoxState::TestManifold1()
{
	static c2Poly a;
	static c2Poly b;
	c2x ax = c2Transform(c2V(500-50.0f, 0), 0.9f);
	
	c2x bx = c2Transform(mp, -1.0f);

	static int which = 0;

	if (glfwGetKey(gWin, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		which = !which;
	}

	if (which)
	{
		srand(2);
		a.count = C2_MAX_POLYGON_VERTS;
		for (int i = 0; i < a.count; ++i) a.verts[i] = RandomVec();
		c2MakePoly(&a);
		b.count = C2_MAX_POLYGON_VERTS;
		for (int i = 0; i < b.count; ++i) b.verts[i] = RandomVec();
		c2MakePoly(&b);
		static float r;

		r += ImGui::GetIO().MouseWheel;

		bx.r = c2Rot(-1.0f + r * 0.2f);
		bx.p = mp;
	}

	else
	{
		ax = c2xIdentity();
		bx = c2xIdentity();
		c2AABB ba;
		c2AABB bb;
		ba.min = c2V(200-20.0f, 200-20.0f);
		ba.max = c2V(200+20.0f, 200+20.0f);
		bb.min = c2V(200-40.0f, 200-40.0f);
		bb.max = c2V(200-20.0f, 200-20.0f);
		ax.r = c2Rot(-1.0f);
		ax.p = c2V(500+50.0f, 500-50.0f);
		bx.p = mp;
		bx.r = c2Rot(1.0f);

		c2BBVerts(a.verts, &ba);
		a.count = 4;
		c2Norms(a.verts, a.norms, 4);

		c2BBVerts(b.verts, &bb);
		b.count = 4;
		c2Norms(b.verts, b.norms, 4);
	}

	//tgLineColor(ctx, 1.0f, 1.0f, 1.0f);
	lineColor = WHITE;
	DrawPoly2(&a, ax);
	DrawPoly2(&b, bx);
	c2Manifold m;
	m.count = 0;
	c2PolytoPolyManifold(&a, &ax, &b, &bx, &m);
	DrawManifold(m);
}

void CSandBoxState::TestManifold2()
{
	static c2Poly a;
	c2x ax = c2Transform(c2V(900-50.0f, 500), 2.0f);
	srand(3);
	a.count = C2_MAX_POLYGON_VERTS;
	for (int i = 0; i < a.count; ++i) a.verts[i] = RandomVec();
	c2MakePoly(&a);

	c2Capsule cap = GetCapsule();

	//tgLineColor(ctx, 1.0f, 1.0f, 1.0f);
	lineColor = WHITE;
	DrawPoly2(&a, ax);
	DrawCapsule(cap.a, cap.b, cap.r);

	c2Manifold m;
	m.count = 0;
	c2CapsuletoPolyManifold(cap, &a, &ax, &m);
	DrawManifold(m);
}

void CSandBoxState::PlastburkRayBug()
{
	c2Poly p;
	p.verts[0] = c2V(0.875f, -11.5f);
	p.verts[1] = c2V(0.875f, 11.5f);
	p.verts[2] = c2V(-0.875f, 11.5f);
	p.verts[3] = c2V(-0.875f, -11.5f);
	p.norms[0] = c2V(1, 0);
	p.norms[1] = c2V(0, 1);
	p.norms[2] = c2V(-1, 0);
	p.norms[3] = c2V(0, -1);
	p.count = 4;

	c2Ray ray0 = { { -3.869416f, 13.0693407f },{ 1, 0 }, 4 };
	c2Ray ray1 = { { -3.869416f, 13.0693407f },{ 0, -1 }, 4 };

	c2Raycast out0;
	c2Raycast out1;
	int hit0 = c2RaytoPoly(ray0, &p, 0, &out0);
	int hit1 = c2RaytoPoly(ray0, &p, 0, &out1);

#define DBG_DRAW_RAY( ray ) \
	tgLine(ray.p.x, ray.p.y, 0, ray.p.x + ray.d.x * ray.t, ray.p.y + ray.d.y * ray.t, 0 )

	//tgLineColor(ctx, 1.0f, 1.0f, 1.0f);
	DBG_DRAW_RAY(ray0);
	DBG_DRAW_RAY(ray1);
	DrawPoly(p.verts, p.count);
	DrawNormals(p.verts, p.norms, p.count);

	if (hit0)
	{
		ray0.t = out0.t;
		c2v impact = c2Impact(ray0, ray0.t);
		c2v end = c2Add(impact, c2Mulvs(out0.n, 1.0f));
		//tgLineColor(ctx, 1.0f, 0.2f, 0.4f);
		lineColor = MAGENTA;
		tgLine(impact.x, impact.y, 0, end.x, end.y, 0);
	}

	if (hit1)
	{
		ray1.t = out1.t;
		c2v impact = c2Impact(ray1, ray1.t);
		c2v end = c2Add(impact, c2Mulvs(out1.n, 1.0f));
		//tgLineColor(ctx, 1.0f, 0.2f, 0.4f);
		lineColor = MAGENTA;
		tgLine(impact.x, impact.y, 0, end.x, end.y, 0);
	}
}


void CSandBoxState::DrawManifold(c2Manifold m)
{
	c2v n = m.normal;
	//tgLineColor(ctx, 1.0f, 0.2f, 0.4f);
	lineColor = MAGENTA;
	for (int i = 0; i < m.count; ++i)
	{
		c2v p = m.contact_points[i];
		float d = m.depths[i];
		DrawCircle(p, 3.0f);
		tgLine(p.x, p.y, 0, p.x + n.x * d, p.y + n.y * d, 0);
	}
}

void CSandBoxState::DrawCircles(c2Circle ca, c2Circle cb)
{
	c2Manifold m;
	m.count = 0;
	c2CircletoCircleManifold(ca, cb, &m);
	//tgLineColor(ctx, 1.0f, 1.0f, 1.0f);
	lineColor = WHITE;
	DrawCircle(ca.p, ca.r);
	DrawCircle(cb.p, cb.r);
	DrawManifold(m);
}

void CSandBoxState::DrawCircleAABB(c2Circle c, c2AABB bb)
{
	c2Manifold m;
	m.count = 0;
	c2CircletoAABBManifold(c, bb, &m);
	//tgLineColor(ctx, 1.0f, 1.0f, 1.0f);
	lineColor = WHITE;
	DrawCircle(c.p, c.r);
	DrawAABB(bb.min, bb.max);
	DrawManifold(m);
}

void CSandBoxState::DrawCircleCapsule(c2Circle c, c2Capsule cap)
{
	c2Manifold m;
	m.count = 0;
	c2CircletoCapsuleManifold(c, cap, &m);
	lineColor = WHITE;
	//tgLineColor(ctx, 1.0f, 1.0f, 1.0f);
	DrawCircle(c.p, c.r);
	DrawCapsule(cap.a, cap.b, cap.r);
	DrawManifold(m);
}

void CSandBoxState::DrawBB(c2AABB ba, c2AABB bb)
{
	c2Manifold m;
	m.count = 0;
	c2AABBtoAABBManifold(ba, bb, &m);
	//tgLineColor(ctx, 1.0f, 1.0f, 1.0f);
	lineColor = WHITE;
	DrawAABB(ba.min, ba.max);
	DrawAABB(bb.min, bb.max);
	DrawManifold(m);
}

c2Capsule CSandBoxState::GetCapsule()
{
	c2Capsule cap = user_capsule;
	cap.a = c2Add(mp, cap.a);
	cap.b = c2Add(mp, cap.b);
	return cap;
}

void CSandBoxState::Rotate(c2v* src, c2v* dst, int count)
{
	if (!ImGui::GetIO().MouseWheel) return;
	c2r r = c2Rot(ImGui::GetIO().MouseWheel > 0 ? 3.14159265f / 16.0f : -3.14159265f / 16.0f);
	for (int i = 0; i < count; ++i) dst[i] = c2Mulrv(r, src[i]);
}