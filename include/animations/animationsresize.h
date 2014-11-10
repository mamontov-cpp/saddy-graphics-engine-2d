/*! \file animationsresize.h
	\author HiddenSeeker

	Describes an animation for resizing an object
 */
#pragma once
#include "animationsanimation.h"

#include "../sadpoint.h"

namespace sad
{
	
namespace animations
{

/*! An animation of rotating the camera
 */
class Resize: public sad::animations::Animation
{
SAD_OBJECT
public:
	/*! Creates new empty animation
	 */
	Resize();
	/*! Can be inherited
	 */
	virtual ~Resize();
	/*! A basic schema for object
        \return a schema
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successfull
     */
    virtual bool loadFromValue(const picojson::value& v);
	/*! Sets how long should be resized object during animation iteration
		\param[in] v vector
	 */
	void setVector(const sad::Point2D& v);
	/*! Sets how long should be resized object during animation iteration
		\return vector
	 */
	const sad::Point2D& vector() const;
	/*! An animations instance
		\paam[in] i an instance of animations
	 */
	virtual void start(sad::animations::Instance* i);
	/*! Sets state of object from animation
        \param[in] i an animation instance
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::animations::Instance* i, double time);
	/*! Creates a state command for an object
        \param[in] o object
        \return state command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* stateCommand(sad::db::Object* o);
    /*! Checks, whether animation is applicable to an object
        \param[in] o object
        \return whether animation is applicable to that object
     */
    virtual bool applicableTo(sad::db::Object* o);
protected:
	/*! A vector for resizing objects
	 */
	sad::Point2D m_vector;
};

}

}

