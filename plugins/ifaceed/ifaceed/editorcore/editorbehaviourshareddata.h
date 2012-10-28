/*! \file   editorbehaviourshareddata.h
	\author HiddenSeeker

	Defines an editor behaviour shared data, needed  to define
	interaction with behaviours
 */
#pragma once

class AbstractScreenObject;

/*! A shared data for editor behaviour
 */
class EditorBehaviourSharedData
{
 private:
	 AbstractScreenObject * m_selected_object;
 public:
	 /*! Shared data is initialized with zero selected object
	  */
	 inline EditorBehaviourSharedData() 
	 {
		 m_selected_object = NULL;
	 }
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
	 }
};

