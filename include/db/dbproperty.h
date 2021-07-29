/*! \file dbproperty.h
    

    Describes a basic property, which objects are consists of
 */
#pragma once
#include "../sadstring.h"
#include "dbtypename.h"
#include "../3rdparty/picojson/valuetotype.h"

namespace sad
{

namespace db
{
class Object;
class Variant;

/*! A basic property, which objects are consists of
 */
class Property
{
public:
    /*! Constructs new property
     */
    Property();
    /*! Can be inherited
     */
    virtual ~Property();
    /*! Returns name of type of property
        \return type of property
     */
    virtual const sad::String & baseType() const;
    /*! Returns whether type of property is kind of sad::Object
        \return whether type of property is kind of sad::Object
     */
    virtual bool typeIsKindOfSadObject() const;
    /*! Returns count for pointer stars count
        \return pointer stars count data
     */
    virtual int pointerStarsCount() const;
    /*! Clones a property
        \return a property clone
     */
    virtual sad::db::Property* clone() const = 0;
    /*! Sets a value for a property
        \param[in] o an object
        \param[in] v a variant for property
        \return whether value is set successfully
     */
    virtual bool set(sad::db::Object * o, const sad::db::Variant & v) = 0;
    /*! Sets a value for a property, trying to copy it if needed
        \param[in] o an object
        \param[in] v a variant for property
        \return whether value is set successfully
     */
    virtual bool copyAndSet(sad::db::Object * o, const sad::db::Variant & v);
    /*! Gets a value for a property
        \param[in] o an object
        \param[in] v a value for a property
     */
    virtual void get(sad::db::Object const* o, sad::db::Variant & v) const = 0;
    /*! Checks, whether value has property type in key field
        \param[in] key a key of field to check
        \param[in] v value
        \return whether field has following type
     */
    virtual bool check(const sad::String& key, const picojson::value& v);
    /*! Whether properties has totally equal type
        \param[in] o other type
        \return whether types are equal
     */
    bool hasEqualTypeAs(sad::db::Property * o) const;
    /*! A type, under which property could be serialized
     */
    sad::String serializableType() const;
    /*! Makes a property non-required with default value
        \param[in] default_value a default value for property
        \return true if successfull, false in case property could not be set from
                     specified type or default_value is nullptr.
     */
    bool makeNonRequiredWithDefaultValue(
        sad::db::Variant* default_value
    );
    /*! Makes a property required
        \return always true
     */
    bool makeRequired();
    /*! Returns true, if property value could be set from specified value
        \param[in] v value
        \return whether it could be set from value
     */ 
    virtual bool couldBeSetFrom(const sad::db::Variant& v) = 0;
    /*! Returns true, if property has a default value
        \return true, if default value is presented
     */
    bool hasDefaultValue() const;
    /*! A default value for property
        \return default value for property
     */
    virtual sad::db::Variant* defaultValue() const;
protected:
    /*! Amount of pointers for type
     */
    int m_pointer_stars_count;
    /*! True if type of property is kind of sad object
     */
    bool m_type_is_kind_of_sad_object;
    /*! A base name of type of property (a name of type without a pointers)
     */
    sad::String m_base_type;
    /*! A default value for property, which should be set in case, if it's absent in JSON
     */
    sad::db::Variant* m_default_value;
};

}

}
