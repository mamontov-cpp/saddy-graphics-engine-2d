/** \file abstractobject.h
	\author HiddenSeeker

	Describes an abstract screen template object
 */
#include "scene.h"
#include "marshal/serializableobject.h"
#include "templates/refcountable.h"
#include "primitives/hpoint.h"
#pragma once

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
	/** A scene with object
	 */
	InterlockedScene * m_scene;
	/** Renders an object. NOTE: Overload this to do actual render of object
	 */
	virtual void _render()=0;
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
	/** Whether object is active
		\return object is active
	 */
	inline bool active() 
	{
		return m_active;
	}
	/** Whether object is visible
		\return object visible
	 */
	inline bool visible() 
	{
		return m_visible;
	}
	/** Sets activity of object
		\param[in] a activity
	 */
	inline void setActive(bool a) 
	{
		m_active = a;
	}
	/** Sets visibility of object
		\param[in] a activity
	 */
	inline void setVisible(bool a) 
	{
		m_visible = a;
	}
	/** Renders an abstract object if active and visible
	 */
	virtual void render();
	/** Returns a region, where object is placed. 
	    \param[in] p point
		\return whether point is within region
     */
	virtual bool isWithin(const hPointF & p)=0;
	/** Validates, whether object is valid in context of template.
	    This check contains, whether all resources is in template.

		Also it can be used to rebuild somke cached data in a rendering loop
		\param[in] t template
		\return whether object is valid in context of template
	 */
	virtual bool isValid(FontTemplateDatabase * t)=0;
	/** Tries to reload a database, getting actual data from database 
		\param[in] db database
		\return whether reloading was ok
	 */
	virtual bool tryReload(FontTemplateDatabase * db)=0;
	/** Returns a region of object. The region is not AABB
		\return region of object
	 */
	virtual hRectF region()=0;
	/** Deletes a object
	 */
	virtual ~AbstractScreenObject();
};

