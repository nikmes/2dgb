#ifndef SANDBOXSTATE_H
#define SANDBOXSTATE_H

#include "applicationstate.h"
//#define TINYC2_IMPL
#include "tinyc2.h"

class CSandBoxState : public CApplicationState
{
	struct Vertex
	{
		c2v pos;
		g2dColor col;
	};
	int code = 0;
	c2v mp;
	c2Circle user_circle;
	c2Capsule user_capsule;
	float user_rotation;
	g2dColor lineColor=RED;

public:
	void init(Application *app);
	void cleanup();

	void pause();
	void resume();

	void handleEvents();
	void update(float tpf);
	void draw();

	static CSandBoxState* Instance()
	{
		return &m_SandBoxState;
	}


protected:
	CSandBoxState() { }

private:
	static CSandBoxState m_SandBoxState;

	Sprite m_mySprite;

	ImGuiIO io;

	float rot = 0.0;

	// for collision testing
	
	vector<Vertex> verts;
	
	void tgLine(float pX1, float pY1, float pZ1, float pX2, float pY2, float pZ2);

	void DrawPoly(c2v* verts, int count);

	void DrawNormals(c2v* verts, c2v* norms, int count);

	void DrawPoly2(c2Poly* p, c2x x);

	void DrawAABB(c2v a, c2v b);

	void DrawHalfCircle(c2v a, c2v b);

	void DrawCapsule(c2v a, c2v b, float r);

	void DrawCircle(c2v p, float r);

	void TestRotation();

	void TestDrawPrim();

	void TestBoolean0();

	void TestBoolean1();

	float randf();

	c2v RandomVec();
	
	void TestBoolean2();

	void TestRay0();

	void TestRay1();

	void TestRay2();

	void DrawManifold(c2Manifold m);

	void DrawCircles(c2Circle ca, c2Circle cb);

	void DrawCircleAABB(c2Circle c, c2AABB bb);

	void DrawCircleCapsule(c2Circle c, c2Capsule cap);

	void DrawBB(c2AABB ba, c2AABB bb);

	void TestManifold0();

	void TestManifold1();

	void TestManifold2();

	void PlastburkRayBug();
	
	c2Capsule GetCapsule();

	void Rotate(c2v* src, c2v* dst, int count);
};

#endif