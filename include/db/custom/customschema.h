/*! \file customschema.h
    

    A custom schema for creating custom items for tree
 */
#pragma once
#include "../schema/schema.h"
#include "../dbstoredpropertyfactory.h"
#include "../../resource/resource.h"
#include "../../sadmutex.h"

namespace sad
{

namespace db
{
    
namespace custom
{
    
class Schema: public sad::db::schema::Schema, public sad::resource::Resource
{
SAD_OBJECT
public:
    /*! A default constructor for schema 
     */
    Schema();
    /*! A destructor for schema 
     */
    ~Schema();
    /*! Sets factory for schema. Factory is being cloned, when set, you may delete it after this call
        \param[in] factory a factory for schema
     */
    void setFactory(sad::db::StoredPropertyFactory * factory);
    /*! Returns factory for schema
        \return factory
     */
    sad::db::StoredPropertyFactory* factory() const;
    /*! Sets tree item name for schema
        \param[in] item an item for schema
     */ 
    void setTreeItemName(const sad::String & item);
    /*! Returns tree item name for schema
     */
    const sad::String& treeItemName() const;
    /*! Loads an options from specified file, using specified renderer for building mip maps.
        \param[in] file a file, via which a resource should be loaded
        \param[in] r  a renderer, which resource should be linked to (nullptr if global renderer)
        \param[in] options  an options for loading a resource
        \return whether loading was successfull
     */
    virtual bool load(
        const sad::resource::ResourceFile & file,
        sad::Renderer * r,
        const picojson::value& options
    );
    /*! Load an options from value
        \param[in] v an options to be loaded
        \return whether loading was successfull
     */
    virtual bool load(const picojson::value& v);
    /*! Returns all custom properties, needed to define an object
        \param[out] props a new property list
     */
    void getCustomProperties(sad::Hash<sad::String, sad::db::Property*>& props);
    /*!  Returns names for custom properties of schemas
     *   \param[out] names list of names of properties
     */
    void getNamesOfCustomProperties(sad::Vector<sad::String>& names);
protected:
    /*! A linked resource item for a schema
     */
    sad::String m_tree_item;
    /*! A default factory by default
     */
    sad::db::StoredPropertyFactory * m_factory;
private:
    /*! Creates a schema
        \param[in] s schema
     */
    Schema(const sad::db::custom::Schema& s);
    /*! Creates a schema
        \param[in] s schema
        \return an item
     */
    sad::db::custom::Schema& operator=(const sad::db::custom::Schema& s);
    /*! A properties lock
     */
    sad::Mutex m_properties_lock;
    /*! A factory lock for schema
     */
    sad::Mutex m_factory_lock;
};

}

}
    
}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::db::custom::Schema)
