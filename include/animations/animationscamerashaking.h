/*! \file animations/animationscamershaking.h
	\author HiddenSeeker

	Declares animation of shaking the camera
 */
#pragma once
#include "animationsanimation.h"

#include "../sadpoint.h"
#include "../scene.h"

namespace sad
{
	
namespace animations
{

/*! An animation of shaking the camera
 */
class CameraShaking: public sad::animations::Animation
{
SAD_OBJECT
public:
	/*! Creates new empty animation
	 */
	CameraShaking();
	/*! Can be inherited
	 */
	virtual ~CameraShaking();
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successfull
     */
    virtual bool loadFromValue(const picojson::value& v);
	/*! Sets offset of how far camera should be translated
		\param[in] offset offset of how far camera should be translated
	 */
	void setOffset(const sad::Point2D& offset);
	/*! Returns offset of how far camera should be translated
		\return offset
	 */
	const sad::Point2D & offset() const;
	/*! Sets frequency of how many times, should be changed offset of camera of scene
		\param[in] freq frequency
	 */
	void setFrequency(int freq);
	/*! Returns frequency of how many times, should be changed offset of camera of scene
		\return frequency
	 */
	int frequency() const;
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
	/*! A vector of, how far should the camera be translated
	 */
	sad::Point2D m_offset;
	/*! A frequency of how many translations should be performed, when camera shake is finished
	 */
	int m_frequency;
};

}

}
