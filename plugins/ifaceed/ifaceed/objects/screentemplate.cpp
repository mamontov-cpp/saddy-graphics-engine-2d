#include "screentemplate.h"
#include "abstractscreenobject.h"
#include <algorithm>
#include "../editorcore/editor.h"
#include <extra/geometry2d.h>

AbstractScreenObject * ScreenTemplate::templateBegin()
{
	SerializableObject* obj = this->begin();
	if (!obj) return NULL;
	return static_cast<AbstractScreenObject *>(obj);
}

AbstractScreenObject * ScreenTemplate::templateNext()
{
	SerializableObject* obj = this->next();
	if (!obj) return NULL;
	return static_cast<AbstractScreenObject *>(obj);
}

void ScreenTemplate::add(AbstractScreenObject * obj)
{
	this->HashBasedSerializableContainer::add((SerializableObject*)obj);
	obj->addRef();
}

void ScreenTemplate::remove(AbstractScreenObject * obj)
{
	this->HashBasedSerializableContainer::remove((SerializableObject*)obj);
	obj->delRef();
}

ScreenTemplate::~ScreenTemplate()
{
	AbstractScreenObject * obj = this->templateBegin();
	while(obj)
	{
		obj->delRef();
		obj = this->templateNext();
	}
}

bool LayerComparator::operator() (AbstractScreenObject * o1, AbstractScreenObject * o2) 
{ 
	unsigned int i1 = o1->scene()->findLayer(o1);
	unsigned int i2 = o2->scene()->findLayer(o2);
	return i1 > i2;
}


#define PRECISION 0.00001

const std::vector<AbstractScreenObject *> & ScreenTemplate::fetchObjectsWithin(const hPointF & point)
{
	if (!(m_within_objects.size() != 0
		&& (fabs(m_cached_point.x() - point.x()) < PRECISION 
		    && fabs(m_cached_point.y() - point.y()) < PRECISION )))
	{
		clearPickedCache();
		m_cached_point = point;
		AbstractScreenObject * o = this->templateBegin();
	
		while(o != NULL) 
		{
			if (o->isWithin(point) && o->active()) 
			{
				m_within_objects.push_back(o);
			}
			o = this->templateNext();
		}
		if (m_within_objects.empty() == false) 
		{
			LayerComparator c;
			std::sort(m_within_objects.begin(), m_within_objects.end(), c);
		}
	}
	return m_within_objects;
}

#undef PRECISION

bool ScreenTemplate::isObjectInPicked(const hPointF & point,AbstractScreenObject * object)
{
	this->fetchObjectsWithin(point);
	return std::find(m_within_objects.begin(), m_within_objects.end(), object) 
		!= m_within_objects.end();
}

bool ScreenTemplate::pickedIsBackground(const hPointF & p,AbstractScreenObject * o)
{
	if (isObjectInPicked(p, o) == false)
			return false;
	// We don't change picked when don't need to
	if (m_within_objects.size() == 1)
			return false;
	hRectF r = o->region();
	float s = dist(r[0], r[1]) * dist(r[0], r[3]);
	unsigned int l = o->prop<unsigned int>("layer", sad::Log::ref());
	bool is_maximum_size = true;
	unsigned int is_least_layer = true;
	for(int i = 0; i < m_within_objects.size(); i++)
	{
		AbstractScreenObject * to = m_within_objects[i];
		if (o != to)
		{
			hRectF rt = to->region();
			float ts = dist(rt[0], rt[1]) * dist(rt[0], rt[3]);
			if (ts > s)  is_maximum_size = false;
			unsigned int lt = to->prop<unsigned int>("layer", sad::Log::ref());
			if (lt < l)	 is_least_layer  = false;
		}
	}
	return is_maximum_size && is_least_layer;
}

void ScreenTemplate::clearPickedCache()
{
	m_within_objects.clear();
}


unsigned int ScreenTemplate::minLayer()
{
	unsigned int result = -1;
	bool found = false;
	AbstractScreenObject * o = this->templateBegin();
	
	while(o != NULL) 
	{
		if (o->active() && o->scene() != NULL) 
		{
			unsigned int clayer = o->scene()->findLayer(o);
			if (!found && clayer < result)
			{
				result = clayer;
			}
		}
		o = this->templateNext();
	}
	return result;
}

unsigned int ScreenTemplate::maxLayer()
{
	unsigned int result = -1;
	bool found = false;
	AbstractScreenObject * o = this->templateBegin();
	
	while(o != NULL) 
	{
		if (o->active() && o->scene() != NULL) 
		{
			unsigned int clayer = o->scene()->findLayer(o);
			if (!found && clayer > result)
			{
				result = clayer;
			}
		}
		o = this->templateNext();
	}
	return result;
}

int ScreenTemplate::count()
{
	int c = 0;
	AbstractScreenObject * o = this->templateBegin();
	
	while(o != NULL) 
	{
		++c;
		o = this->templateNext();
	}
	return c;
}