/** \file screentemplate.h
	\author HiddenSeeker

	Describes a main result of editing  - a screen template, which can be used in some other stuff
 */
#include <marshal/hashbasedserializablecontainer.h>
#include <sadpoint.h>
#include <vector>
#pragma once


class AbstractScreenObject;

/** A simple comparator for sorting layers of objects
 */
class LayerComparator 
{
public:    
	bool operator() (AbstractScreenObject * o1, AbstractScreenObject * o2); 
};

/** Describes a screen template as main result of editing of data
 */
class ScreenTemplate: public HashBasedSerializableContainer
{
 private:
	 /*! Cached point within objects
	  */
	 sad::Point2D  m_cached_point;
	 /*! A buffer, for picking
	  */
	 std::vector<AbstractScreenObject *> m_within_objects;
 public:
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

	/*! Fetches and returns objects which point belongs to.
		This call is cached
		\param[in] point point
		\return objects within range
	 */
	virtual const std::vector<AbstractScreenObject *> & fetchObjectsWithin(const sad::Point2D & point);
	/*! Tests, whether object is in picked list
		\param[in] point point data
		\param[in] object object
		\return whether he is in list
	 */
	virtual bool isObjectInPicked(const sad::Point2D & point,AbstractScreenObject * object);
	/*! Tests, whether object is background for all oher
		It's defines, when it's size is maximum and it's layer
		is minumum of all
		\param[in] point point data
		\param[in] object object
		\return whether those conditions are met
	 */
	virtual bool pickedIsBackground(const sad::Point2D & point,AbstractScreenObject * object);
	
	/*! Clears a picked element cache
	 */ 
	virtual void clearPickedCache();
	/** Returns minimal layer, used by objects
	 */
	virtual unsigned int minLayer();
	/** Returns minimal layer, used by objects
	 */
	virtual unsigned int maxLayer();
	/*! Returns amount of objects in template
	 */
	int count();
	/** Frees data 
	 */
	~ScreenTemplate();
};
