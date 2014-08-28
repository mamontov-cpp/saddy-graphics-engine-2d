/** \file propertychangecommand.h
	\author HiddenSeeker
	
	This command allows setting and resetting some object property
 */
#include "command.h"
#include "../objects/abstractscreenobject.h"
#include "sadstring.h"
#include "sadrect.h"
#include <3rdparty/format/format.h>
#include <log/log.h>
#include <unused.h>
#pragma once



class EditorLog;
class ScreenTemplate;

/*! This command is invoked, when object changes any of his properties, except for bounding rectangle
	and inner sprite
 */
template<
	typename T
>
class PropertyChangeCommand: public history::Command 
{
private:
	AbstractScreenObject * m_object; //!< Object of data
	sad::log::Log * m_log;				 //!< Log of editor
	
	sad::String m_property_name;     //!< Name of property
	T m_prev_value;				     //!< Previous value of object
	T m_current_value;               //!< Current value of object

public:
	/** Creates new command
		\param[in] o screen object
		\param[in] s string
		\param[in] new_value new value
		\param[in] log       log
	 */
	PropertyChangeCommand(AbstractScreenObject * o, const sad::String & s, T new_value, sad::log::Log * log) 
	{
		SL_SCOPE("PropertyChangeCommand::PropertyChangeCommand");
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
		\param[in] old_value an old value of property
		\param[in] log       log
	 */
	PropertyChangeCommand(AbstractScreenObject * o, const sad::String & s, T new_value, T old_value, sad::log::Log * log) 
	{
		m_log = log;
		m_object = o;
		m_property_name = s;
		m_current_value = new_value;
		m_prev_value = old_value;
	}

	/*! Sets new value for command
		\param[in] v value
	 */
	void setValue(T v) 
	{
		m_current_value = v;	
	}
	/*! Commits a command
		\param[in] ob observer for command
	 */
    void commit(core::Editor * ob = NULL)
	{
		SL_SCOPE(str(fmt::Format("PropertyChangeCommand<{0}>::commit") << m_property_name.data()) );
		m_object->setProp(m_property_name, m_current_value, m_log);
		ob->submitEvent("PropertyChangeCommand::commit", sad::db::Variant(0));
	}
	/*! Rollbacks a command
		\param[in] ob observer for command
	 */
    void rollback(core::Editor * ob = NULL)
	{
		SL_SCOPE(str(fmt::Format("PropertyChangeCommand<{0}>::rollback") << m_property_name.data()) );
		m_object->setProp(m_property_name, m_prev_value, m_log);
		ob->submitEvent("PropertyChangeCommand::rollback", sad::db::Variant(0));
	}


};


class FontTemplateDatabase;
class ScreenSprite;

class SpritePropertyChangeCommandInfo
{
public:
	sad::String config;
	sad::String group;
	int index;

	sad::Rect2D rect;
	float angle;
};


/*! A command, which is performed when sprite changes any his inner texture property
 */
class SpritePropertyChangeCommand: public history::Command 
{
 private:
	ScreenSprite * m_sprite;
	FontTemplateDatabase * m_db;
	sad::log::Log * m_log;				
	SpritePropertyChangeCommandInfo m_old;
	SpritePropertyChangeCommandInfo m_new;
 public:
	inline SpritePropertyChangeCommand(ScreenSprite * sprite,
									   FontTemplateDatabase * db,
									   sad::log::Log * log,
									   const SpritePropertyChangeCommandInfo & _old,
									   const SpritePropertyChangeCommandInfo & _new)
	{
		SL_SCOPE("SpritePropertyChangeCommand::SpritePropertyChangeCommand");
		m_sprite = sprite;
		m_db = db;
		m_old = _old;
		m_new = _new;
		m_log = log;
	}
	/*! Commits a command
		\param[in] ob observer for command
	 */
	void commit(core::Editor * ob = NULL);
	/*! Rollbacks a command
		\param[in] ob observer for command
	 */
	void rollback(core::Editor * ob = NULL);
};

/*! A command, which is performed when sprite changes bounding rectangle
 */
class SpriteRectChangeCommand: public history::Command
{
 private:
		ScreenSprite * m_sprite;
		float m_angle;
		sad::Rect2D m_old_rect;
		sad::Rect2D m_new_rect;
public:
	inline SpriteRectChangeCommand(ScreenSprite * sprite, float angle, const sad::Rect2D & oldrect, const sad::Rect2D &  newrect)
	{
		SL_SCOPE("SpriteRectChangeCommand::SpriteRectChangeCommand");
		m_sprite = sprite;
		m_angle = angle;
		m_old_rect = oldrect;
		m_new_rect = newrect;
	}
	/*! Commits a command
		\param[in] ob observer for command
	 */
	void commit(core::Editor * ob = NULL);
	/*! Rollbacks a command
		\param[in] ob observer for command
	 */
	void rollback(core::Editor * ob = NULL);
};

