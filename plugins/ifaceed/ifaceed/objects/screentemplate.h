/** \file screentemplate.h
	\author HiddenSeeker

	Describes a main result of editing  - a screen template, which can be used in some other stuff
 */
#include "marshal/hashbasedserializablecontainer.h"
#pragma once


class AbstractScreenObject;
class Editor;
/** Describes a screen template as main result of editing of data
 */
class ScreenTemplate: public HashBasedSerializableContainer
{
 private:
	 Editor * m_editor; //!< Editor part
 public:
	 /** Editor parts
		 \param[in] editor data
	  */
	 inline void setEditor(Editor * editor)
	 {
		 m_editor = editor;
	 }
	 /** Returns editor backreference
		 \return editor backreference
	  */
	 inline Editor * editor() 
	 {
		 return m_editor;
	 }
	 /*! Adds an object to container
		\param[in] obj object
	 */
    virtual void add(AbstractScreenObject * obj);
	/*! Removes an object to container
		\param[in] obj object
	 */
    virtual void remove(AbstractScreenObject * obj);

	/*! Returns a first object from container, starting new iteration
		\return beginning of container
	 */
	virtual AbstractScreenObject * templateBegin();
	/*! Returns next iteration
	    \return next object from container , null on end of container 
	 */
	virtual AbstractScreenObject * templateNext();

	/** Frees data 
	 */
	~ScreenTemplate();
};
