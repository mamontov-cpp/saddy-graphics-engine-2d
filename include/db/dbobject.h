/*! \file db/object.h
    
    
    Contains definition of basic serializable class Object.
 */
#pragma once
#include "../refcountable.h"
#include "../3rdparty/picojson/valuetotype.h"
#include "dbproperty.h"
#include "dbcanbecastedfromto.h"
#include "dbvariant.h"

namespace sad
{

class Renderer;

namespace db
{

namespace schema
{
class Schema;
}

class Property;
class Table;
/*! \class Object
    
    Defines a basic serializable object
 */
class Object: public sad::RefCountable
{
public:	
    /*! A default object constructor
     */
    Object();
    /*! This class can be inherited 
     */
    virtual ~Object();
    /*! Saves object to a picojson::value
        \param[in] v a value for object
     */
    virtual void save(picojson::value & v);
    /*! Loads object from picojson object
        \param[in] v a picojson object
        \return  whether it as successfull
     */
    virtual bool load(const picojson::value& v);
    /*! Resets object's non-serialized state, when restoring snapshot to 
        ensure idempotency of restoring objects
     */
    virtual void reset();
    /*! Returns a table, where object belongs
        \return table
     */
    virtual sad::db::Table* table() const;
    /*! Sets a table
        \param[in] t a table
     */
    virtual void setTable(sad::db::Table* t);
    /*! A basic schema for object
        \return a schema 
     */
    static sad::db::schema::Schema* basicSchema(); 
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;
    /*! A name, under which object could be serialized
        \return name, under which object will be serialized
     */
    virtual const sad::String& serializableName() const;
    /*! Returns current name for an objects
        \return name for an object
     */
    const sad::String& objectName() const;
    /*! Sets new name for an object
        \param[in] newname a name for object
     */
    void setObjectName(const sad::String & newname);
    /*! Tries to fetch property value from an object
        \param[in] s name of property
        \return property value if it could be fetched
     */
    template<
        typename T
    >
    sad::Maybe<T> getProperty(const sad::String & s) const
    {
        sad::db::Property * prop = this->getObjectProperty(s);
        sad::Maybe<T> result;
        if (prop)
        {
            sad::db::TypeName<T>::init();
            bool canbecasted = sad::db::can_be_casted_from_to(
                prop->baseType(), 
                prop->typeIsKindOfSadObject(),
                prop->pointerStarsCount(),
                sad::db::TypeName<T>::baseName(),
                sad::db::TypeName<T>::isSadObject(),
                sad::db::TypeName<T>::POINTER_STARS_COUNT
            );
            if (canbecasted)
            {
                sad::db::Variant v;
                prop->get(this, v);
                result = v.get<T>();
            }
        }
        return result;
    }
    /*! Sets a property for an object
        \param[in] s a name of property
        \param[in] o value for property
        \return whether property is successfully set
     */ 
    template<
        typename T
    >
    bool setProperty(const sad::String & s, const T & o)
    {
        sad::db::Property * prop = this->getObjectProperty(s);
        bool result = false;
        if (prop)
        {
            sad::db::TypeName<T>::init();			
            bool canbecasted = sad::db::can_be_casted_from_to(
                sad::db::TypeName<T>::baseName(),
                sad::db::TypeName<T>::isSadObject(),
                sad::db::TypeName<T>::POINTER_STARS_COUNT,
                prop->baseType(), 
                prop->typeIsKindOfSadObject(),
                prop->pointerStarsCount()
            );
            if (canbecasted)
            {
                sad::db::Variant v(o);
                result = prop->set(this, v);
            }
        }
        return result;
    }
    /*! Called, when loading an object. Here, object must make all resource path links depend on specified tree.
        By default, does nothing
        \param[in] renderer a renderer
        \param[in] treename a tree name
     */
    virtual void setTreeName(
        sad::Renderer* renderer,
        const sad::String& treename
    );
    /*! Fetches property for an object with specified game
        \param[in] s string
        \return s string
     */
    sad::db::Property* getObjectProperty(const sad::String& s) const;
    /*! A basic introspection capability. Checks, whether object has specified type
        \param[in] name name of class
        \return in basic implementation - false
     */
    virtual bool isInstanceOf(const sad::String& name);
    /*! A major id for object
     */
    unsigned long long MajorId;
    /*! A minor id for a table
     */
    unsigned long long MinorId;
    /*! Whether object is active. Non-active objects are not saved in database
     */
    bool Active;
protected:
    /*! A table for object, null by default
     */
    sad::db::Table* m_table;
    /*! An identifying name for object
     */
    sad::String m_name;
};

}

}
