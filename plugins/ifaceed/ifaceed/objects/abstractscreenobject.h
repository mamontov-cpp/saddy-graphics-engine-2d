/** \file abstractscreenobject.h
	\author HiddenSeeker

	Describes an abstract screen template object
 */
#include <log/log.h>
#include "scene.h"
#include "marshal/serializableobject.h"
#include "db/dbfield.h"
#include "sadpoint.h"
#include <QString>
#pragma once

class EditorLog;
class ScreenTemplate;
class InterlockedScene;
class FontTemplateDatabase;
/** Describes an abstract object of screen template
 */
class AbstractScreenObject: public sad::SceneNode, public SerializableObject
{
 SAD_OBJECT
 protected:
	/*! A unique ID of object in screenTemplate
	 */
	sad::String m_uid;
	/** Name of object
	 */
	sad::String m_name;
	/** Renders an object. NOTE: Overload this to do actual render of object
	 */
	virtual void _render()=0;
	/** Returns generated description of object
	 */
	virtual sad::String _description()=0;	
 public:
	/** Default screen object
	 */
    AbstractScreenObject();
	/** Returns parent object of template
	 */
	ScreenTemplate * screenTemplate() const;
	/*! Returns interlocked scene as needed
		\return interlocked scene
	 */
	inline InterlockedScene * scene() 
	{
		if (this->sad::SceneNode::scene() == NULL)
			return NULL;
		return reinterpret_cast<InterlockedScene *>(this->sad::SceneNode::scene());
	}
	/*! Whether we can rotate object
	 */
	virtual bool rotatable();
	/*! Whether we can resize object
	 */
	virtual bool resizable();
	/*! Renders an abstract object if active and visible
	 */
	virtual void render();
	/*! A type
	 */
	virtual sad::String type();
	/*! Names of type
	 */
	virtual sad::String typeName() = 0;
	/*! Returns a region, where object is placed. 
	    \param[in] p point
		\return whether point is within region
     */
	virtual bool isWithin(const sad::Point2D & p)=0;
	/*! Validates, whether object is valid in context of template.
	    This check contains, whether all resources is in template.

		Also it can be used to rebuild somke cached data in a rendering loop
		\param[in] t template
		\param[out] errors errors of objects
		\return whether object is valid in context of template
	 */
	virtual bool isValid(FontTemplateDatabase * t, sad::Vector<sad::String> * errors = NULL)=0;
	/*! Tries to reload a database, getting actual data from database 
		\param[in] db database
		\return whether reloading was ok
	 */
	virtual bool tryReload(FontTemplateDatabase * db)=0;
	/*!	Initialize graphic part of data
		\param[in] db database
	 */
	virtual void initializeGraphicAfterLoad(FontTemplateDatabase * db)=0;
	/*! Returns a region of object. The region is not AABB
		\return region of object
	 */
	virtual sad::Rect2D region()=0;
	/*! Moves a center of object to a following point
		\param[in] p point
	 */
	virtual void moveCenterTo(const sad::Point2D & p)=0;
	/*! Returns a description of object for putting it to a various list
		\return string with description
	 */
	virtual sad::String description();	
	/*! Deletes a object
	 */
	virtual ~AbstractScreenObject();
	/*! Returns unique ID for an object
		\return unique id for an object
	 */
	const sad::String & uid() const;
	/*! Sets unique id for an object
		\param[in] uid unique id
	 */
	void setUID(const sad::String & uid);
	/** Returns a property of screen obejct, handling all problems
		\param[in] s string
		\param[in] m_log log, where all messages go
		\return property
	 */
	template<typename T> T prop(const sad::String & s, sad::log::Log * m_log) 
	{
		/*
		#define STRINGCLASS QString
		sad::db::Property * p = this->getProperty(s);
		if (p != NULL)
		{
			sad::Maybe<T> value;// = p->get<T>();
			if (value.exists())
			{
				return value.value();
			}
		} 
		else 
		{
			SL_DEBUG(STRINGCLASS("Property \"") +  s.data() + STRINGCLASS("\" is not found"));
		}
		#undef STRINGCLASS
		*/
		return T();
	}

	template<typename T> void setProp(const sad::String & s, T val, sad::log::Log * m_log)
	{
		/*
		#define STRINGCLASS QString
		sad::db::Property * p = this->getProperty(s);
		if (p != NULL)
		{
			try 
			{
				p->set(val);
			}
			catch(std::logic_error ex) 
			{
				STRINGCLASS test = (STRINGCLASS("Property \"") +  s.data()) + "\"";
				SL_DEBUG( test + ex.what());
			}
		} 
		else 
		{
			SL_DEBUG(STRINGCLASS("Property \"") +  s.data() + STRINGCLASS("\" is not found"));
		}
		#undef STRINGCLASS
		*/
	}
	/*! Tests, whether object has following properties
		\param[in] s property
		\return whether object has a property
	 */
	bool hasProperty(const sad::String & s);
	/*! Sets an already rotated rectangle, restoring all data
	 */
	virtual void setRotatedRectangle(const sad::Rect2D & rotatedrectangle, float angle);
};

