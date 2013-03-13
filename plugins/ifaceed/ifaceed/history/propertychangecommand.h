/** \file propertychangecommand.h
	\author HiddenSeeker
	
	This command allows setting and resetting some object property
 */
#include "abstractcommand.h"
#include "../objects/abstractscreenobject.h"
#include "templates/hstring.h"
#include "primitives/hrect.h"
#pragma once

class EditorLog;
class ScreenTemplate;

// T is type of value
template<
	typename T
>
class PropertyChangeCommand: public AbstractCommand 
{
private:
	AbstractScreenObject * m_object; //!< Object of data
	EditorLog * m_log;				 //!< Log of editor
	
	hst::string m_property_name;     //!< Name of property
	T m_prev_value;				     //!< Previous value of object
	T m_current_value;               //!< Current value of object

public:
	/** Creates new command
		\param[in] o screen object
		\param[in] s string
		\param[in] new_value new value
		\param[in] log       log
	 */
	PropertyChangeCommand(AbstractScreenObject * o, const hst::string & s, T new_value, EditorLog * log) 
	{
		m_log = log;
		m_object = o;
		m_property_name = s;
		m_current_value = new_value;
		m_prev_value = o->prop<T>(s, log);
	}

	/** Creates new command
		\param[in] o screen object
		\param[in] s string
		\param[in] new_value new value
		\param[in] log       log
	 */
	PropertyChangeCommand(AbstractScreenObject * o, const hst::string & s, T new_value, T old_value, EditorLog * log) 
	{
		m_log = log;
		m_object = o;
		m_property_name = s;
		m_current_value = new_value;
		m_prev_value = old_value;
	}


	void setValue(T v) 
	{
		m_current_value = v;	
	}

	void commit(ActionContext *c, CommandChangeObserver * ob = NULL)
	{
		m_object->setProp(m_property_name, m_current_value, m_log);
	}

	void rollback(ActionContext *c, CommandChangeObserver * ob = NULL)
	{
		m_object->setProp(m_property_name, m_prev_value, m_log);
	}


};


class FontTemplateDatabase;
class ScreenSprite;

class SpritePropertyChangeCommandInfo
{
public:
	hst::string config;
	hst::string group;
	int index;

	hRectF rect;
	float angle;
};



class SpritePropertyChangeCommand: public AbstractCommand 
{
 private:
	ScreenSprite * m_sprite;
	FontTemplateDatabase * m_db;
	EditorLog * m_log;				
	SpritePropertyChangeCommandInfo m_old;
	SpritePropertyChangeCommandInfo m_new;
 public:
	inline SpritePropertyChangeCommand(ScreenSprite * sprite,
									   FontTemplateDatabase * db,
									   EditorLog * log,
									   const SpritePropertyChangeCommandInfo & _old,
									   const SpritePropertyChangeCommandInfo & _new)
	{
		m_sprite = sprite;
		m_db = db;
		m_old = _old;
		m_new = _new;
		m_log = log;
	}

	void commit(ActionContext *c, CommandChangeObserver * ob = NULL);

	void rollback(ActionContext *c, CommandChangeObserver * ob = NULL);
};


class SpriteRectChangeCommand: public AbstractCommand
{
 private:
		ScreenSprite * m_sprite;
		float m_angle;
		hRectF m_old_rect;
		hRectF m_new_rect;
public:
	inline SpriteRectChangeCommand(ScreenSprite * sprite, float angle, const hRectF & oldrect, const hRectF &  newrect)
	{
		m_sprite = sprite;
		m_angle = angle;
		m_old_rect = oldrect;
		m_new_rect = newrect;
	}
	void commit(ActionContext *c, CommandChangeObserver * ob = NULL);

	void rollback(ActionContext *c, CommandChangeObserver * ob = NULL);
};