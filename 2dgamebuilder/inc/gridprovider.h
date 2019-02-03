#ifndef GRIDPROVIDER_H
#define GRIDPROVIDER_H

#include "definitions.h"

class GRIDProvider
{
private:


	//float m_itemWidth;
	//float m_itemHeight;
	//Sprite *m_item;
	XMLConfigurationProvider *confProvider;

public:

	Sprite *m_item;
	
	int		m_width;
	int		m_height;
	float	m_rotation;
	point	m_rotationOrigin;

	GRIDProvider();

	~GRIDProvider();

	void setGridRotationOrigin(float x, float y);

	void setGridRotation(float rot);

	void setGridItemRotation(float rot);

	void draw();

	void drawOrtho();

	void setItem(Sprite *item);

	void setGridItemScale(float scale);

	void destroy(); 

	void switchDrawingMode();

	Sprite* getCurrentSprite();

};

#endif
