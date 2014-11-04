/*! \file animations/animationsoptionlist.h
	\author HiddenSeeker

	An animations as animation of changing options of sprite by time
 */
#pragma once

#include "../fuzzyequal.h"
#include "../sadvector.h"
#include "../sadstring.h"
#include "../sadhash.h"

#include "animationsanimation.h"


namespace sad
{
	
namespace animations
{

/*! An animation, which changes options of sprite over time
 */
class OptionList: public sad::animations::Animation
{
SAD_OBJECT
public:
	/*! Creates new empty animation
	 */
	OptionList();
	/*! Can be inherited
	 */
	virtual ~OptionList();
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successfull
     */
    virtual bool loadFromValue(const picojson::value& v);
	/*! Sets list of options
		\param[in] fonts a font list, used in animation
	 */
	void setList(const sad::Vector<sad::String>& list);
	/*! Returns option list
		\return option list
	 */
	const sad::Vector<sad::String> & list() const;
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
	/*! A list of texture coordinates to be set
	 */
	sad::Vector<sad::String> m_list;
};

}

}
