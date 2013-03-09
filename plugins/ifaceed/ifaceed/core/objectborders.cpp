#include "objectborders.h"
#include "../objects/abstractscreenobject.h"
#include <renderer.h>
#include <config/sprite2dobserver.h>

bool ActiveObjectBorder::tryPerform()
{
	AbstractScreenObject * o = this->m_data->activeObject();
	if (o) 
	{
		hRectF r = o->region();
		glDisable(GL_TEXTURE_2D);
		GLint   clr[4]={};
		glGetIntegerv(GL_CURRENT_COLOR,clr);
		glColor4ub(0,255,255,255);
		glBegin(GL_LINES);
		glVertex2f(r[0].x(), r[0].y());
		glVertex2f(r[1].x(), r[1].y());

		glVertex2f(r[1].x(), r[1].y());
		glVertex2f(r[2].x(), r[2].y());

		glVertex2f(r[2].x(), r[2].y());
		glVertex2f(r[3].x(), r[3].y());	

		glVertex2f(r[3].x(), r[3].y());	
		glVertex2f(r[0].x(), r[0].y());

		glEnd();
		glColor4iv(clr);
		glEnable(GL_TEXTURE_2D);

		//this->renderHotSpots(o, false);
	}
	return false;
}




bool SelectedObjectBorder::tryPerform()
{
	AbstractScreenObject * o = this->m_data->selectedObject();
	if (o) 
	{
		hRectF r = o->region();
		glDisable(GL_TEXTURE_2D);
		GLint   clr[4]={};
		glGetIntegerv(GL_CURRENT_COLOR,clr);
		glColor4ub(255,0,0,255);
		glBegin(GL_LINES);
		glVertex2f(r[0].x(), r[0].y());
		glVertex2f(r[1].x(), r[1].y());

		glVertex2f(r[1].x(), r[1].y());
		glVertex2f(r[2].x(), r[2].y());

		glVertex2f(r[2].x(), r[2].y());
		glVertex2f(r[3].x(), r[3].y());	

		glVertex2f(r[3].x(), r[3].y());	
		glVertex2f(r[0].x(), r[0].y());

		glEnd();
		glColor4iv(clr);
		glEnable(GL_TEXTURE_2D);

		this->renderHotSpots(o, true);
	}
	return false;
}

#define HOTSPOT_PADDING 12

static inline hPointF center(const hRectF & r, int p1, int p2, int a1, int a2, float size) 
{
	hPointF center = (r[p1] + r[p2]) / 2;
	float angle = atan2f(r[a2].y() - r[a1].y(), r[a2].x() - r[a1].x());
	//hPointF result = center;
	hPointF result = center  - ( hPointF(cos(angle), sin(angle)) * size);
	return result;
}

void ObjectBorder::renderSpot(const char * c, const hRectF & r, int r0, int r1, int r2, int r3, float dangle)
{
	Sprite2DConfig * ci = this->m_data->icons();
	Sprite2DConfigObserver observer(c,0, ci);
	hPointF point = center(r,r0,r1,r2,r3,HOTSPOT_PADDING); 
	if (observer.createSprite(point))
	{
		observer.sprite()->rotate(atan2f(r[r3].y() - r[r2].y(), r[r3].x() - r[r2].x()) - dangle);
		observer.sprite()->adapter()->render();
	}
}

void ObjectBorder::renderHotSpots(AbstractScreenObject * o, bool canDelete)
{
	Sprite2DConfig * c = this->m_data->icons();
	hRectF region = o->region();
	if (o->resizable()) 
	{
		this->renderSpot("left",  region,3,0,0,1, 0.0f);
		this->renderSpot("bottom",   region,2,3,3,0, 1.57f);
		this->renderSpot("right", region,1,2,2,3, 3.14f);
		this->renderSpot("top",  region,0,1,1,2, 4.71f);
	}

	// Render delete button
	if (canDelete) 
	{
		hPointF delposition = region[2];
		hPointF center = (region[0] + region[2])/2;
		float angle = atan2f(delposition.y() - center.y(), delposition.x() - center.x() );
		delposition += hPointF(cos(angle), sin(angle)) * HOTSPOT_PADDING;
		Sprite2DConfigObserver observer("delete", 0, c);
		if (observer.createSprite(delposition))
		{
			observer.sprite()->adapter()->render();
		}
	}
	
	
}


static inline hRectF hotSpotRectangleFrom(const hRectF & r, int p1, int p2, int a1, int a2, float size, float dangle) 
{
	hPointF center = (r[p1] + r[p2]) / 2;
	float angle = atan2f(r[a2].y() - r[a1].y(), r[a2].x() - r[a1].x());
	//hPointF result = center;
	hPointF result = center  - ( hPointF(cos(angle), sin(angle)) * size);
	hRectF v(hPointF(-size, -size), hPointF(size, -size), hPointF(size, size), hPointF(-size, size));
	float deltaangle = angle - dangle;
	for(int i = 0; i < 4; i++) {
		float x = v[i].x()*cos(deltaangle) - v[i].y()*sin(deltaangle);
		float y = v[i].x()*sin(deltaangle) + v[i].y()*cos(deltaangle);
		v[i] = result + hPointF(x,y);
	}
	return v;
}


bool testIsWithin(const hPointF & p1, const hRectF & r);

hst::vector<hRectF> ObjectBorder::createHotSpots(AbstractScreenObject * o, bool canDelete)
{
	hRectF region = o->region();
	hst::vector<hRectF> result;
	result <<  hotSpotRectangleFrom(region,3,0,0,1, HOTSPOT_PADDING, 0.0f);
	result <<  hotSpotRectangleFrom(region,2,3,3,0, HOTSPOT_PADDING, 1.57f);
	result <<  hotSpotRectangleFrom(region,1,2,2,3, HOTSPOT_PADDING, 3.14f);
	result <<  hotSpotRectangleFrom(region,0,1,1,2, HOTSPOT_PADDING, 4.71f);

	if (canDelete) 
	{
		float size = HOTSPOT_PADDING;
		hPointF delposition = region[2];
		hPointF center = (region[0] + region[2])/2;
		float angle = atan2f(delposition.y() - center.y(), delposition.x() - center.x() );
		delposition += hPointF(cos(angle), sin(angle)) * HOTSPOT_PADDING;
		hRectF v(hPointF(-size, -size), hPointF(size, -size), hPointF(size, size), hPointF(-size, size));
		for(int i = 0; i < 4; i++) {
			v[i] +=  delposition;
		}
		result << v;
	}
	return result;
}

bool ObjectBorder::removable()
{
	return true;
}

bool ActiveObjectBorder::removable()
{
	return false;
}

bool testIsWithin(const hPointF & p1, const hRectF & r) ;

hst::vector<BorderHotSpots> ObjectBorder::isWithin(const hPointF & p, AbstractScreenObject * o)
{
	hst::vector<BorderHotSpots> hotspotsets;
	hotspotsets << BHS_LEFT;
	hotspotsets << BHS_BOTTOM;
	hotspotsets << BHS_RIGHT;
	hotspotsets << BHS_TOP;
	if (this->removable()) 
	{
		hotspotsets << BHS_REMOVE;
	}
	hst::vector<BorderHotSpots> result;
	hst::vector<hRectF> r = this->createHotSpots(o, this->removable());
	for(int i = 0; i <  r.count(); i++) 
	{
		if (testIsWithin(p, r[i]))
		{
			result << hotspotsets[i];
		}
	}
	return result;
}

hst::vector<BorderHotSpots> ActiveObjectBorder::isWithin(const hPointF & p)
{
	AbstractScreenObject * o = this->m_data->activeObject();
	if (o) 
	{
		return this->ObjectBorder::isWithin(p,o);
	}
	return hst::vector<BorderHotSpots> ();
}

hst::vector<BorderHotSpots> SelectedObjectBorder::isWithin(const hPointF & p)
{
	AbstractScreenObject * o = this->m_data->selectedObject();
	if (o) 
	{
		return this->ObjectBorder::isWithin(p,o);
	}
	return hst::vector<BorderHotSpots> ();
}
