/*! \file animations/animationsdiscreteproperty.h
	\author HiddenSeeker

	An animations as animation of changing a discrete property
 */
#pragma once

#include "../fuzzyequal.h"
#include "../sadvector.h"
#include "../sadstring.h"

#include "animationsanimation.h"
#include "animationsinstance.h"


namespace sad
{
	
namespace animations
{

/*! An animation, which changes a discrete property

	To instance it properly don't forget to add DECLARE_SOBJ_INHERITANCE(sad::animations::DiscreteProperty<T>, sad::animations::Animation);
	to place of instancing to init metadata properly.
 */
template<
	typename T
>
class DiscreteProperty: public sad::animations::Animation
{
SAD_OBJECT
public:
	/*! Creates new empty animation
	 */
	DiscreteProperty()
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~DiscreteProperty()
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
			sad::Maybe<sad::Vector<T> > list = picojson::to_type<sad::Vector<T> >(
											       picojson::get_property(v, "list")
											   );
			bool result = list.exists();
			if (result)
			{
				m_list = list.value();
			}

			flag = flag && result;
		}
		return flag;
	}
	/*! Sets list of property
		\param[in] list a list of properties
	 */
	void setList(const sad::Vector<T>& list)
	{
		m_list = list;
	}
	/*! Returns list of property
		\return list of properties
	 */
	const sad::Vector<T> & list()
	{
		return m_list;
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
	/*! Sets state of object from animation
        \param[in] o object
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::db::Object* o, double time)
	{
		if (sad::is_fuzzy_zero(m_time) || m_list.size() == 0)
			return;

		double value = static_cast<double>(m_list.size()) * time / m_time;
		unsigned int kvalue = static_cast<unsigned int>(value);
		if (kvalue < m_list.size())
		{
			o->setProperty(m_property_name, m_list[kvalue]);
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
			sad::Maybe<T> maybeopts = o->getProperty<T>(m_property_name); 
			if (maybeopts.exists())
			{
				result = true;

				i->oldState().clear();
				i->oldState() << sad::db::Variant(maybeopts.value());
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
	/*! A list of fonts to be set in animation
	 */
	sad::Vector<T> m_list;
};

}

}
