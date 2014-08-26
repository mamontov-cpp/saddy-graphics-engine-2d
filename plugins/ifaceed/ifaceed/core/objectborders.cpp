#include <renderer.h>
#include <geometry2d.h>

#include <config/sprite2dconfig.h>
#include <config/sprite2dtemplate.h>
#include <config/sprite2dobserver.h>

#include "objectborders.h"

#include "../objects/abstractscreenobject.h"

#include "editor.h"



void ActiveObjectBorder::_process()
{
	AbstractScreenObject * o = this->m_data->activeObject();
	if (this->m_data->mustShowActiveBorder()) 
	{
		sad::Rect2D r = o->region();
		sad::Renderer::ref()->render()
							->rectangle(r, sad::AColor(0, 255, 255, 255));
	}
}

ActiveObjectBorder::~ActiveObjectBorder()
{
	delete this->m_data->activeObject();
}


void SelectedObjectBorder::_process()
{
	AbstractScreenObject * o = this->m_data->selectedObject();
	if (o) 
	{
		sad::Rect2D r = o->region();
		sad::Renderer::ref()->render()
							->rectangle(r, sad::AColor(255, 0, 0, 255));
		this->renderHotSpots(o, true);
	}
}

#define HOTSPOT_PADDING 12

static inline sad::Point2D center(const sad::Rect2D & r, int p1, int p2, int a1, int a2, float size) 
{
	sad::Point2D center = (r[p1] + r[p2]) / 2;
	float angle = atan2f(r[a2].y() - r[a1].y(), r[a2].x() - r[a1].x());
	//sad::Point2D result = center;
	sad::Point2D result = center  - ( sad::Point2D(cos(angle), sin(angle)) * size);
	return result;
}

void ObjectBorder::renderSpot(const char * c, const sad::Rect2D & r, int r0, int r1, int r2, int r3, float dangle)
{
	sad::Sprite2DConfig * ci = this->m_data->icons();
	sad::Sprite2DConfigObserver observer(c,0, ci);
	sad::Point2D point = center(r,r0,r1,r2,r3,HOTSPOT_PADDING); 
	if (observer.createSprite(point))
	{
		observer.sprite()->rotate(atan2f(r[r3].y() - r[r2].y(), r[r3].x() - r[r2].x()) - dangle);
		observer.sprite()->render();
	}
}

sad::Rect2D deletePositionRectangle(const sad::Rect2D & region,float size)
{
	sad::Point2D delposition = region[2];
	sad::Point2D center = (region[0] + region[2])/2;
	float angle = atan2f(delposition.y() - center.y(), delposition.x() - center.x() );
	delposition += sad::Point2D(cos(angle), sin(angle)) * HOTSPOT_PADDING;
	sad::Rect2D v(sad::Point2D(-size, -size), sad::Point2D(size, -size), sad::Point2D(size, size), sad::Point2D(-size, size));
	for(int i = 0; i < 4; i++) 
	{
		v[i] += delposition; 
	}
	return v;
}

#define DELETEPADDINGSIZE 8
void ObjectBorder::renderHotSpots(AbstractScreenObject * o, bool canDelete)
{
	sad::Sprite2DConfig * c = this->m_data->icons();
	sad::Rect2D region = o->region();
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
		sad::Sprite2DConfigObserver observer("delete", 0, c);
		if (observer.createSprite(sad::Point2D(0,0)))
		{
			sad::Rect2D v = deletePositionRectangle(region, DELETEPADDINGSIZE);
			observer.sprite()->setRenderableArea(v);
			//observer.sprite()->adapter()->setSize(sad::Point2D(size,size));
			observer.sprite()->render();
		}
	}
	
	
}


static inline sad::Rect2D hotSpotRectangleFrom(const sad::Rect2D & r, int p1, int p2, int a1, int a2, float size, float dangle) 
{
	sad::Point2D center = (r[p1] + r[p2]) / 2;
	float angle = atan2f(r[a2].y() - r[a1].y(), r[a2].x() - r[a1].x());
	//sad::Point2D result = center;
	sad::Point2D result = center  - ( sad::Point2D(cos(angle), sin(angle)) * size);
	sad::Rect2D v(sad::Point2D(-size, -size), sad::Point2D(size, -size), sad::Point2D(size, size), sad::Point2D(-size, size));
	float deltaangle = angle - dangle;
	sad::moveAndRotateNormalized(deltaangle,  result, v);
	return v;
}



sad::Vector<sad::Rect2D> ObjectBorder::createHotSpots(AbstractScreenObject * o, bool canDelete)
{
	sad::Rect2D region = o->region();
	sad::Vector<sad::Rect2D> result;
	if (o->resizable())
	{
	 result <<  hotSpotRectangleFrom(region,3,0,0,1, HOTSPOT_PADDING, 0.0f);
	 result <<  hotSpotRectangleFrom(region,2,3,3,0, HOTSPOT_PADDING, 1.57f);
	 result <<  hotSpotRectangleFrom(region,1,2,2,3, HOTSPOT_PADDING, 3.14f);
	 result <<  hotSpotRectangleFrom(region,0,1,1,2, HOTSPOT_PADDING, 4.71f);
	}
	if (canDelete) 
	{
		sad::Rect2D v = 	deletePositionRectangle(region, HOTSPOT_PADDING);
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


sad::Vector<BorderHotSpots> ObjectBorder::isWithin(const sad::Point2D & p, AbstractScreenObject * o)
{
	SL_SCOPE("ObjectBorder::isWithin()");
	
	sad::Vector<BorderHotSpots> hotspotsets;
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
	sad::Vector<BorderHotSpots> result;
	sad::Vector<sad::Rect2D> r = this->createHotSpots(o, this->removable());
	SL_DEBUG(
		QString("Testing click(%1, %2)")
		.arg(p.x())
		.arg(p.y())
	);
    for(unsigned int i = 0; i <  r.count(); i++)
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
		if (sad::isWithin(p, r[i]))
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

sad::Vector<BorderHotSpots> ActiveObjectBorder::isWithin(const sad::Point2D & p)
{
	AbstractScreenObject * o = this->m_data->activeObject();
	if (o) 
	{
		return this->ObjectBorder::isWithin(p,o);
	}
	return sad::Vector<BorderHotSpots> ();
}

sad::Vector<BorderHotSpots> SelectedObjectBorder::isWithin(const sad::Point2D & p)
{
	AbstractScreenObject * o = this->m_data->selectedObject();
	if (o) 
	{
		return this->ObjectBorder::isWithin(p,o);
	}
	return sad::Vector<BorderHotSpots> ();
}


bool SelectedObjectBorder::resizable()
{
  if (m_data->selectedObject())
  {
	  return m_data->selectedObject()->resizable();
  }
  return false;
}
