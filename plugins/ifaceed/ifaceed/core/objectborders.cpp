#include "objectborders.h"
#include "../objects/abstractscreenobject.h"
#include <renderer.h>
#include "ifaceeditor.h"

#include <config/sprite2dobserver.h>
#include <extra/geometry2d.h>
bool ActiveObjectBorder::tryPerform()
{
	AbstractScreenObject * o = this->m_data->activeObject();
	if (this->m_data->mustShowActiveBorder()) 
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

hRectF deletePositionRectangle(const hRectF & region,float size)
{
	hPointF delposition = region[2];
	hPointF center = (region[0] + region[2])/2;
	float angle = atan2f(delposition.y() - center.y(), delposition.x() - center.x() );
	delposition += hPointF(cos(angle), sin(angle)) * HOTSPOT_PADDING;
	hRectF v(hPointF(-size, -size), hPointF(size, -size), hPointF(size, size), hPointF(-size, size));
	for(int i = 0; i < 4; i++) 
	{
		v[i] += delposition; 
	}
	return v;
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
		Sprite2DConfigObserver observer("delete", 0, c);
		if (observer.createSprite(hPointF(0,0)))
		{
			hRectF v = deletePositionRectangle(region, HOTSPOT_PADDING);
			observer.sprite()->adapter()->setRect(v);
			//observer.sprite()->adapter()->setSize(hPointF(size,size));
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
	moveAndRotateNormalized(deltaangle,  result, v);
	return v;
}



hst::vector<hRectF> ObjectBorder::createHotSpots(AbstractScreenObject * o, bool canDelete)
{
	hRectF region = o->region();
	hst::vector<hRectF> result;
	if (o->resizable())
	{
	 result <<  hotSpotRectangleFrom(region,3,0,0,1, HOTSPOT_PADDING, 0.0f);
	 result <<  hotSpotRectangleFrom(region,2,3,3,0, HOTSPOT_PADDING, 1.57f);
	 result <<  hotSpotRectangleFrom(region,1,2,2,3, HOTSPOT_PADDING, 3.14f);
	 result <<  hotSpotRectangleFrom(region,0,1,1,2, HOTSPOT_PADDING, 4.71f);
	}
	if (canDelete) 
	{
		hRectF v = 	deletePositionRectangle(region, HOTSPOT_PADDING);
		result << v;
	}
	return result;
}

bool ObjectBorder::removable()
{
	return true;
}

bool ObjectBorder::resizable()
{
	return false;
}

bool ActiveObjectBorder::removable()
{
	return false;
}


hst::vector<BorderHotSpots> ObjectBorder::isWithin(const hPointF & p, AbstractScreenObject * o)
{
	SL_SCOPE("ObjectBorder::isWithin()");
	
	hst::vector<BorderHotSpots> hotspotsets;
	QVector<QString> m_setnames;
	if (this->resizable()) 
	{
		hotspotsets << BHS_LEFT;   m_setnames << "BHS_LEFT";
		hotspotsets << BHS_BOTTOM; m_setnames << "BHS_BOTTOM";
		hotspotsets << BHS_RIGHT;  m_setnames << "BHS_RIGHT";
		hotspotsets << BHS_TOP;    m_setnames << "BHS_TOP";
	}
	if (this->removable()) 
	{
		hotspotsets << BHS_REMOVE; m_setnames << "BHS_REMOVE";
	}
	hst::vector<BorderHotSpots> result;
	hst::vector<hRectF> r = this->createHotSpots(o, this->removable());
	SL_DEBUG(
		QString("Testing click(%1, %2)")
		.arg(p.x())
		.arg(p.y())
	);
	for(int i = 0; i <  r.count(); i++) 
	{
		SL_DEBUG(
			QString("%1: bounding rectangle [(%2, %3), (%4, %5), (%6, %7), (%8, %9)]")
			.arg(m_setnames[i])
			.arg(r[i][0].x())
			.arg(r[i][0].y())
			.arg(r[i][1].x())
			.arg(r[i][1].y())
			.arg(r[i][2].x())
			.arg(r[i][2].y())
			.arg(r[i][3].x())
			.arg(r[i][3].y())
		);
		if (::isWithin(p, r[i]))
		{
			SL_DEBUG("Hit!");		
			result << hotspotsets[i];
		}
		else
		{
			SL_DEBUG("Missed!");
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


bool SelectedObjectBorder::resizable()
{
  if (m_data->selectedObject())
  {
	  return m_data->selectedObject()->resizable();
  }
}