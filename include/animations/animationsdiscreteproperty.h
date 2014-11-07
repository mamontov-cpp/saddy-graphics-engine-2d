/*! \file animations/animationsdiscreteproperty.h
	\author HiddenSeeker

	An animations as animation of changing a discrete property
 */
#pragma once

#include "../fuzzyequal.h"
#include "../sadvector.h"
#include "../sadstring.h"
#include "../maybe.h"

#include "animationsanimation.h"
#include "animationsinstance.h"

#include "animationssavedobjectproperty.h"
#include "setstate/setproperty.h"

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
         m_creators.pushProperty<T>("", "");
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
            sad::Maybe<sad::String> propertyname = picojson::to_type<sad::String>(
                                                    picojson::get_property(v, "property")
                                                );
            bool result = list.exists() && propertyname.exists();
			if (result)
			{
				m_list = list.value();
				m_inner_valid = m_list.size() != 0;
				updateValidFlag();
                m_property_name = propertyname.value();
                sad::animations::SavedObjectPropertyCreator<T> * c = static_cast<sad::animations::SavedObjectPropertyCreator<T> *>(m_creators[0]);
                c->setPropertyName(m_property_name);
                c->setName(m_property_name);
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
        sad::animations::SavedObjectPropertyCreator<T> * c = static_cast<sad::animations::SavedObjectPropertyCreator<T> *>(m_creators[0]);
        c->setPropertyName(m_property_name);
        c->setName(m_property_name);
	}
	/*! Returns property name
		\return property name
	 */
	const sad::String& propertyName() const
	{
		return m_property_name;
	}
	/*! Sets state of object from animation
        \param[in] i an animation instance
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::animations::Instance* i, double time)
	{
		if (!m_valid)
			return;

		double value = static_cast<double>(m_list.size()) * time / m_time;
		unsigned int kvalue = static_cast<unsigned int>(value);
		if (kvalue < m_list.size())
		{
			o->setProperty(m_property_name, m_list[kvalue]);
		}
	}
    /*! Creates a state command for an object
        \param[in] o object
        \return state command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* stateCommand(sad::db::Object* o)
    {
        return new sad::animations::setstate::SetProperty<T>(o, m_property_name);
    }

    /*! Checks, whether animation is applicable to an object
        \param[in] o object
        \return whether animation is applicable to that object
     */
    virtual bool applicableTo(sad::db::Object* o)
    {
        bool result = false;
        if (o && m_valid)
        {
            sad::Maybe<T> maybevalue = o->getProperty<T>(m_property_name);
            result = maybevalue.exists();
        }
        return result;
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
