/*! \file animations/animationscontinuousproperty.h
    

    An animations as animation of continuous property changing
 */
#pragma once

#include "../fuzzyequal.h"
#include "../sadstring.h"
#include "../maybe.h"
#include "../sadmutex.h"

#include "../db/schema/schema.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "../db/dbproperty.h"
#include "../db/save.h"
#include "../db/load.h"
#include "../db/dbfield.h"
#include "../db/dbmethodpair.h"
#include "../db/dbtable.h"

#include "animationsanimation.h"
#include "animationsinstance.h"

#include "easing/easingfunction.h"

// ReSharper disable once CppUnusedIncludeDirective
#include "animationssavedobjectproperty.h"
#include "setstate/setproperty.h"

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
public:
    /*! Returns global meta data for object
        \return object metadata
     */
    static sad::ClassMetaData* globalMetaData()
    {
        sad::db::TypeName<T>::init();
        sad::String type_of_name = sad::db::TypeName<T>::baseName();
        const unsigned int pointer_count = sad::db::TypeName<T>::POINTER_STARS_COUNT;
        for(int i = 0; i < pointer_count;i++)
        {
            type_of_name += "*";
        }
        sad::String my_name = "sad::animations::ContinuousProperty<";
        my_name << type_of_name;
        my_name << ">";

        bool created = false;
        sad::ClassMetaData* m = sad::ClassMetaDataContainer::ref()->get(my_name, created);
        if (created)
        {
            if (sad::animations::Animation::globalMetaData() == nullptr) return nullptr;
            m->addAncestor("sad::animations::Animation");
        }
        return m;
    }
    /*! Return object meta data
        \return object meta data
     */
    sad::ClassMetaData * metaData() const override
    {
        return sad::animations::ContinuousProperty<T>::globalMetaData();
    }

    /*! Creates new empty animation
     */
    ContinuousProperty()
    {
        m_creators.pushProperty<T>("", "");
    }
    /*! Can be inherited
     */
    virtual ~ContinuousProperty() override
    {

    }
    /*! A basic schema for object
        \return a schema
     */
    static sad::db::schema::Schema* basicSchema()
    {
        static sad::db::schema::Schema* Schema = nullptr;
        static sad::Mutex SchemaInit;
        if (Schema == nullptr)
        {
            SchemaInit.lock();
            if (Schema == nullptr)
            {
                Schema = new sad::db::schema::Schema();
                Schema->addParent(sad::animations::Animation::basicSchema());

                Schema->add(
                    "min_value",
                    new sad::db::MethodPair<sad::animations::ContinuousProperty<T>, T>(
                        &sad::animations::ContinuousProperty<T>::minValue,
                        &sad::animations::ContinuousProperty<T>::setMinValue
                    )
                );
                Schema->add(
                    "max_value",
                    new sad::db::MethodPair<sad::animations::ContinuousProperty<T>, T>(
                        &sad::animations::ContinuousProperty<T>::maxValue,
                        &sad::animations::ContinuousProperty<T>::setMaxValue
                    )
                );
                Schema->add(
                    "property",
                    new sad::db::MethodPair<sad::animations::ContinuousProperty<T>, sad::String>(
                        &sad::animations::ContinuousProperty<T>::propertyName,
                        &sad::animations::ContinuousProperty<T>::setPropertyName
                    )
                );
            
                sad::ClassMetaDataContainer::ref()->pushGlobalSchema(Schema);
            }
            SchemaInit.unlock();
        }
        return Schema;
    }
    /*! Returns schema for an object
        \return schema
     */
    sad::db::schema::Schema* schema() const override
    {
        return sad::animations::ContinuousProperty<T>::basicSchema();
    }
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successful
     */
    virtual bool loadFromValue(const picojson::value& v) override
    {
        bool flag = this->sad::animations::Animation::loadFromValue(v);
        if (flag)
        {
            sad::Maybe<T> min_size = picojson::to_type<T>(
                                                    picojson::get_property(v, "min_value")
                                                );
            sad::Maybe<T> max_size = picojson::to_type<T>(
                                                    picojson::get_property(v, "max_value")
                                                );
            const sad::Maybe<sad::String> property_name = picojson::to_type<sad::String>(
                                                    picojson::get_property(v, "property")
                                                );
            const bool result = min_size.exists() && max_size.exists() && property_name.exists();
            if (result)
            {
                m_min_value = min_size.value();
                m_max_value = max_size.value();
                setPropertyName(property_name.value());
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
    /*! Sets minimal value
        \param[in] value minimal value
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
        \param[in] value maximal value
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
        \param[in] i an animation instance
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::animations::Instance* i, double time) override
    {
        if (!m_valid)
            return;
        
        sad::db::Object* o = i->object();

        T min = m_min_value;
        T max = m_max_value;
        double time_position = m_easing->eval(time, m_time);
        T value = min + (max - min) * time_position;
        if (o)
        {
            o->setProperty(m_property_name, value);
        }
    }

    /*! Creates a state command for an object
        \param[in] o object
        \return state command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* stateCommand(sad::db::Object* o) override
    {
        return new sad::animations::setstate::SetProperty<T>(o, m_property_name);
    }

    /*! Checks, whether animation is applicable to an object
        \param[in] o object
        \return whether animation is applicable to that object
     */
    virtual bool applicableTo(sad::db::Object* o) override
    {
        bool result = false;
        if (o && m_valid)
        {
            sad::Maybe<T> maybe_value = o->getProperty<T>(m_property_name);
            result = maybe_value.exists();
        }
        return result;
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
