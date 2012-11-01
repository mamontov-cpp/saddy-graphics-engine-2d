/*! \file objectborders.h
	\author HiddenSeeker

	Contains an object borders for some dynamic rendering
 */
#include <input.h>
#include "../../editorcore/editorbehaviourshareddata.h"
#pragma once


class ObjectBorder: public sad::RepeatingTask
{
 protected:
	 EditorBehaviourSharedData * m_data; //!< Data for rendering object
 public:
	 inline ObjectBorder(EditorBehaviourSharedData  * data)
	 {
		 m_data = data;
	 }
};

class ActiveObjectBorder: public ObjectBorder
{
 public:
	 inline ActiveObjectBorder(EditorBehaviourSharedData  * data):ObjectBorder(data)
	 {
		
	 }
	 /*! Renders an active object border as blue border
		 \return success of task
	  */
	 bool tryPerform();
};

class SelectedObjectBorder: public ObjectBorder
{
 public:
	 inline SelectedObjectBorder(EditorBehaviourSharedData  * data):ObjectBorder(data)
	 {
		 
	 }
	 /*! Renders a selected object border as render border
		 \return success of task
	  */
	 bool tryPerform();
};


