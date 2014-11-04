/*! \file animations/animationscontinuousproperty.h
	\author HiddenSeeker

	An animations as animation of continous property changing
 */
#pragma once

#include "../fuzzyequal.h"
#include "../sadstring.h"

#include "animationsanimation.h"
#include "animationsinstance.h"

namespace sad
{
	
namespace animations
{

/*! An animation of changing continuous property

	To instance it properly don't forget to add DECLARE_SOBJ_INHERITANCE(sad::animations::DiscreteProperty<T>, sad::animations::Animation);
	to place of instancing to init metadata properly.
 */
template<
	typename T
>
class ContinuousProperty: public sad::animations::Animation
{
SAD_OBJECT
public:
	/*! Creates new empty animation
	 */
	ContinuousProperty()
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ContinuousProperty()
	{

	}
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successfull
     */
    virtual bool loadFromValue(const picojson::value& v)
	{
		bool flag = this->sad::animations::Animation::loadFromValue(v);
		if (flag)
		{
			sad::Maybe<T> minsize = picojson::to_type<T>(
													picojson::get_property(v, "min_value")
												);
			sad::Maybe<T> maxsize = picojson::to_type<T>(
													picojson::get_property(v, "max_value")
												);
			sad::Maybe<sad::String> propertyname = picojson::to_type<sad::String>(
													picojson::get_property(v, "property")
												);
			bool result = minsize.exists() && maxsize.exists() && propertyname.exists();
			if (result)
			{
				m_min_value = minsize.value();
				m_max_value = maxsize.value();
				m_property_name = propertyname.value();
			}

			flag = flag && result;
		}
		return flag;
	}
	/*! Sets changing property name for animation
		\param[in] name name for property
	 */
	void setPropertyName(const sad::String& name)
	{
		m_property_name = name;
	}
	/*! Returns property name
		\return property name
	 */
	const sad::String& propertyName() const
	{
		return m_property_name;
	}
	/*! Sets minimal value
		\param[in] size minimal value
	 */
	void setMinValue(const T & value)
	{
		m_min_value = value;
	}
	/*! Returns minimal value
		\return minimal value
	 */
	unsigned int minValue() const
	{
		return m_min_value;
	}
	/*! Sets maximal value
		\param[in] size maximal value
	 */
	void setMaxValue(const T & value)
	{
		m_max_value = value;
	}
	/*! Returns maximal font size
		\return maximal font size
	 */
	unsigned int maxValue() const
	{
		return m_max_value;	
	}
	/*! Sets state of object from animation
        \param[in] o object
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::db::Object* o, double time)
	{
		if (sad::is_fuzzy_zero(m_time))
			return;

		T min = m_min_value;
		T max = m_max_value;
		T value = min + (max - min) * time / m_time;
		if (o)
		{
			o->setProperty(m_property_name, value);
		}
	}
    /*! Saves states of object in animation instance
        \param[in] i an animation instance
        \return whether we can work further with this object in instance
     */
    virtual bool saveState(sad::animations::Instance* i)
	{
		sad::db::Object* o = i->object();
		bool result = false;
		if (o)
		{
			sad::Maybe<T> maybevalue = o->getProperty<T>(m_property_name); 
			if (maybevalue.exists())
			{
				result = true;

				i->oldState().clear();
				i->oldState() << sad::db::Variant(maybevalue.value());
			}
		}
		return result;
	}
    /*! Resets state of object in animation instance, when animation ended
        \param[in] i an animation instance
     */
    virtual void resetState(sad::animations::Instance* i)
	{
		sad::db::Object* o = i->object();
		if (o && i->oldState().size() == 1)
		{
			sad::Maybe<T> value = i->oldState()[0].get<T>();
			if (value.exists())
			{
				o->setProperty(m_property_name, value.value());
			}
		}
	}
protected:
	/*! A name for property
	 */
	sad::String m_property_name;
	/*! A minimal property value
	 */
	T m_min_value;
	/*! A maximal property value
	 */
	T m_max_value;
};

}

}
