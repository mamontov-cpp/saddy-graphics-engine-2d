/*! \file animations/animationsdiscreteproperty.h
    

    An animations as animation of changing a discrete property
 */
#pragma once

#include "../fuzzyequal.h"
#include "../sadvector.h"
#include "../sadstring.h"
#include "../maybe.h"
#include "../sadmutex.h"

#include "../db/schema/schema.h"
#include "../db/dbproperty.h"
#include "../db/save.h"
#include "../db/load.h"
#include "../db/dbfield.h"
#include "../db/dbmethodpair.h"
#include "../db/dbtable.h"

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
public:
    /*! Returns global meta data for object
        \return object metadata
     */
    static sad::ClassMetaData* globalMetaData()
    {
        sad::db::TypeName<T>::init();
        sad::String typeofname = sad::db::TypeName<T>::baseName();
        unsigned int pointercount = sad::db::TypeName<T>::POINTER_STARS_COUNT;
        for(int i = 0; i < pointercount;i++)
        {
            typeofname += "*";
        }
        sad::String myname = "sad::animations::DiscreteProperty<";
        myname << typeofname;
        myname << ">";

        bool created = false;
        sad::ClassMetaData* m = sad::ClassMetaDataContainer::ref()->get(myname, created);
        if (created)
        {
            if (sad::animations::Animation::globalMetaData() == NULL) return NULL;
            m->addAncestor("sad::animations::Animation");
        }
        return m;
    }
    /*! Return object meta data
        \return object meta data
     */
    sad::ClassMetaData * metaData() const
    {
        return sad::animations::DiscreteProperty<T>::globalMetaData();
    }

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
    /*! A basic schema for object
        \return a schema
     */
    static sad::db::schema::Schema* basicSchema()
    {
        static sad::db::schema::Schema* Schema = NULL;
        static sad::Mutex SchemaMutex;
        if (Schema == NULL)
        {
            SchemaMutex.lock();
            if (Schema == NULL)
            {
                Schema = new sad::db::schema::Schema();
                Schema->addParent(sad::animations::Animation::basicSchema());

                Schema->add(
                    "list",
                    new sad::db::MethodPair<sad::animations::DiscreteProperty<T>, sad::Vector<T> >(
                        &sad::animations::DiscreteProperty<T>::list,
                        &sad::animations::DiscreteProperty<T>::setList
                    )
                );			
                Schema->add(
                    "property",
                    new sad::db::MethodPair<sad::animations::DiscreteProperty<T>, sad::String>(
                        &sad::animations::DiscreteProperty<T>::propertyName,
                        &sad::animations::DiscreteProperty<T>::setPropertyName
                    )
                );
            
                sad::ClassMetaDataContainer::ref()->pushGlobalSchema(Schema);
            }
            SchemaMutex.unlock();
        }
        return Schema;
    }
    /*! Returns schema for an object
        \return schema
     */
    sad::db::schema::Schema* schema() const
    {
        return sad::animations::DiscreteProperty<T>::basicSchema();
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
                setList(list.value());
                setPropertyName(propertyname.value());
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
        m_inner_valid = m_list.size() != 0;
        updateValidFlag();
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
            i->object()->setProperty(m_property_name, m_list[kvalue]);
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
