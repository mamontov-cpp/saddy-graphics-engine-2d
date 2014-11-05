/*! \file animations/animationsfontlist.h
	\author HiddenSeeker

	An animations as animation of changing fonts by time
 */
#pragma once

#include "../fuzzyequal.h"
#include "../sadvector.h"
#include "../sadstring.h"

#include "animationsanimation.h"


namespace sad
{
	
namespace animations
{

/*! An animation, which changes font of label
 */
class FontList: public sad::animations::Animation
{
SAD_OBJECT
public:
	/*! Creates new empty animation
	 */
	FontList();
	/*! Can be inherited
	 */
	virtual ~FontList();
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successfull
     */
    virtual bool loadFromValue(const picojson::value& v);
	/*! Sets list of fonts
		\param[in] fonts a font list, used in animation
	 */
	void setFonts(const sad::Vector<sad::String>& fonts);
	/*! Returns font list
		\return font list
	 */
	const sad::Vector<sad::String> & fonts() const;
	/*! Sets state of object from animation
        \param[in] i an animation instance
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::animations::Instance* i, double time);
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
	/*! A list of fonts to be set in animation
	 */
	sad::Vector<sad::String> m_fonts;
};

}

}
