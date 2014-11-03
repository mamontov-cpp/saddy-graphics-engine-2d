/*! \file animations/animationsfontsize.h
	\author HiddenSeeker

	An animations as animation of font size by time
 */
#pragma once

#include "../fuzzyequal.h"

#include "animationsanimation.h"

namespace sad
{
	
namespace animations
{

class FontSize: public sad::animations::Animation
{
SAD_OBJECT
public:
	/*! Creates new empty animation
	 */
	FontSize();
	/*! Can be inherited
	 */
	virtual ~FontSize();
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successfull
     */
    virtual bool loadFromValue(const picojson::value& v);
	/*! Sets minimal font size
		\param[in] size minimal font size
	 */
	void setMinSize(unsigned int size);
	/*! Returns minimal font size
		\return minimal font size
	 */
	unsigned int minSize() const;
	/*! Sets maximal font size
		\param[in] size maximal font size
	 */
	void setMaxSize(unsigned int size);
	/*! Returns maximal font size
		\return maximal font size
	 */
	unsigned int maxSize() const;
	    /*! Sets state of object from animation
        \param[in] o object
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::db::Object* o, double time);
    /*! Saves states of object in animation instance
        \param[in] i an animation instance
        \return whether we can work further with this object in instance
     */
    virtual bool saveState(sad::animations::Instance* i);
    /*! Resets state of object in animation instance, when animation ended
        \param[in] i an animation instance
     */
    virtual void resetState(sad::animations::Instance* i);
protected:
	/*! A minimal font size
	 */
	unsigned int m_min_size;
	/*! A maximal font size
	 */
	unsigned int m_max_size;
};

}

}
