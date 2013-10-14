/*! \file   editorbehaviourshareddata.h
	\author HiddenSeeker

	Defines an editor behaviour shared data, needed  to define
	interaction with behaviours
 */
#pragma once
#include <stdio.h>
#include <sadstring.h>
#include <log/log.h>
class AbstractScreenObject;
class EditorLog;


/*! A shared data for editor behaviour
 */
class EditorBehaviourSharedData
{
 private:
	 AbstractScreenObject * m_selected_object;    //!< A current selected object
	 AbstractScreenObject * m_active_object;      //!< A current active obhect
	 bool					m_show_active_border;  //!< A current active border
 public:
	 
	 /*! Shared data is initialized with zero selected object
	  */
	 inline EditorBehaviourSharedData() 
	 {
		 m_selected_object = NULL;
		 m_active_object = NULL;
		 m_show_active_border = true;
	 }
	 inline bool mustShowActiveBorder() { return m_show_active_border  && m_active_object != NULL;}
	 inline void toggleActiveBorder(bool flag) { m_show_active_border = flag;}
	 /*! Returns a current selected object
	  */
	 inline AbstractScreenObject * selectedObject() const
	 {
		return m_selected_object;
	 }
	 /*! Sets current selected object to a specified data
	  */
	 inline void setSelectedObject(AbstractScreenObject * o) 
	 {
		 m_selected_object = o;
		 SL_DEBUG(sad::String("selected object is now ") 
			     + sad::String::number(reinterpret_cast<unsigned long>(o))
				 );
	 }
	 /*! Returns a current active object
	  */
	 inline AbstractScreenObject * activeObject() const
	 {
		return m_active_object;
	 }
	 /*! Sets current active object to a specified data
	  */
	 inline void setActiveObject(AbstractScreenObject * o) 
	 {
		 m_active_object = o;
	 }
};

