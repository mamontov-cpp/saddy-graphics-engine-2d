/** \file abstractobject.h
	\author HiddenSeeker

	Describes an abstract screen template object
 */
#include <log/log.h>
#include "scene.h"
#include "marshal/serializableobject.h"
#include "templates/refcountable.h"
#include "primitives/hpoint.h"
#include <QString>
#pragma once

class EditorLog;
class ScreenTemplate;
class InterlockedScene;
class FontTemplateDatabase;
/** Describes an abstract object of screen template
 */
class AbstractScreenObject: public sad::BasicNode, public SerializableObject, public RefCountable
{
 SAD_NODE
 protected:
	/** Whether object is active
	 */
	bool m_active;
	/** Whether object is visible
	 */ 
	bool m_visible;
	/** Name of object
	 */
	hst::string m_name;
	/** A scene with object
	 */
	InterlockedScene * m_scene;
	/** Renders an object. NOTE: Overload this to do actual render of object
	 */
	virtual void _render()=0;
	/** Returns generated description of object
	 */
	virtual hst::string _description()=0;	
 public:
	/** Default screen object
	 */
    AbstractScreenObject();
	/** Returns parent object of template
	 */
	ScreenTemplate * screenTemplate();
	/** Describes a scene with data
	 */
	inline void setScene(InterlockedScene * scene)
	{
		m_scene = scene;
	}
	/** Returns a scene for object
		\return object scene
	 */
	inline InterlockedScene * scene()
	{
		return m_scene;
	}
	/*! Whether object is active
		\return object is active
	 */
	inline bool active() 
	{
		return m_active;
	}
	/*! Whether object is visible
		\return object visible
	 */
	inline bool visible() 
	{
		return m_visible;
	}
	/*! Sets activity of object
		\param[in] a activity
	 */
	inline void setActive(bool a) 
	{
		m_active = a;
	}
	/*! Sets visibility of object
		\param[in] a activity
	 */
	inline void setVisible(bool a) 
	{
		m_visible = a;
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
	virtual hst::string type();
	/*! Names of type
	 */
	virtual hst::string typeName() = 0;
	/*! Returns a region, where object is placed. 
	    \param[in] p point
		\return whether point is within region
     */
	virtual bool isWithin(const hPointF & p)=0;
	/*! Validates, whether object is valid in context of template.
	    This check contains, whether all resources is in template.

		Also it can be used to rebuild somke cached data in a rendering loop
		\param[in] t template
		\return whether object is valid in context of template
	 */
	virtual bool isValid(FontTemplateDatabase * t)=0;
	/*! Tries to reload a database, getting actual data from database 
		\param[in] db database
		\return whether reloading was ok
	 */
	virtual bool tryReload(FontTemplateDatabase * db)=0;
	/*! Returns a region of object. The region is not AABB
		\return region of object
	 */
	virtual hRectF region()=0;
	/*! Moves a center of object to a following point
		\param[in] p point
	 */
	virtual void moveCenterTo(const hPointF & p)=0;
	/*! Returns a description of object for putting it to a various list
		\return string with description
	 */
	virtual hst::string description();	
	/*! Deletes a object
	 */
	virtual ~AbstractScreenObject();
	/** Returns a property of screen obejct, handling all problems
		\param[in] s string
		\param[in] m_log log, where all messages go
		\return property
	 */
	template<typename T> T prop(const hst::string & s, sad::Log * m_log) 
	{
		#define STRINGCLASS QString
		AbstractProperty * p = this->getProperty(s);
		if (p != NULL)
		{
			try 
			{
				sad::Variant * v = p->get(m_log); 
				T result = v->get<T>(m_log);
				return result;
			}
			catch(serializable::InvalidPropertyType ex) 
			{
				STRINGCLASS test = (STRINGCLASS("Property \"") +  s.data()) + "\"";
				SL_DEBUG( test + ex.getInformation().data());
			}
		} 
		else 
		{
			SL_DEBUG(STRINGCLASS("Property \"") +  s.data() + STRINGCLASS("\" is not found"));
		}
		#undef STRINGCLASS
		return T();
	}

	template<typename T> void setProp(const hst::string & s, T val, sad::Log * m_log)
	{
		#define STRINGCLASS QString
		AbstractProperty * p = this->getProperty(s);
		if (p != NULL)
		{
			try 
			{
				p->set(val,m_log);
			}
			catch(serializable::InvalidPropertyType ex) 
			{
				STRINGCLASS test = (STRINGCLASS("Property \"") +  s.data()) + "\"";
				SL_DEBUG( test + ex.getInformation().data());
			}
		} 
		else 
		{
			SL_DEBUG(STRINGCLASS("Property \"") +  s.data() + STRINGCLASS("\" is not found"));
		}
		#undef STRINGCLASS
	}
	/*! Tests, whether object has following properties
		\param[in] s property
		\return whether object has a property
	 */
	bool hasProperty(const hst::string & s);
	/*! Sets a top
	 */
	virtual void setTop(float x);
	/*! Sets a top
	 */
	virtual void setBottom(float x);
	/*! Sets a top
	 */
	virtual void setRight(float x);
	/*! Sets a top
	 */
	virtual void setLeft(float x);
	/*! Sets an already rotated rectangle, restoring all data
	 */
	virtual void setRotatedRectangle(const hRectF & rotatedrectangle, float angle);
};

