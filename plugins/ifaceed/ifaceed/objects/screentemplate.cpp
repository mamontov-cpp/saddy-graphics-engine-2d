#include "screentemplate.h"
#include "abstractscreenobject.h"
#include <algorithm>
#include "../editorcore/editor.h"

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
			if (o->isWithin(point)) 
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

void ScreenTemplate::clearPickedCache()
{
	m_within_objects.clear();
}
