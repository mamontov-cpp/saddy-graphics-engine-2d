/*! \file classmetadatacontainer.h
    

    Defines a container for class meta data
 */
#pragma once
#include "classmetadata.h"
#include "sadhash.h"
#include "sadmutex.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "sadptrvector.h"

namespace sad
{

namespace db
{

namespace schema
{
class Schema;
}

}
/*! A container for class meta dat
 */
class ClassMetaDataContainer
{
public:
    /*! Returns a reference for container
        \return reference for  container
     */
    static sad::ClassMetaDataContainer* ref();
    /*! Returns a metadata for name. If metadata for name does not exists, creates it, with
        initialized class ame and inserts it to container. 
        \param[in] name name of class, which metadata is returned
        \param[out] created when metadata is created by container, this flag set to true 
        \param[in] lock whether we should lock getting metadata
        \return a  metadata
     */
    sad::ClassMetaData* get(const sad::String & name, bool & created, bool lock = true);
    /*! Returns a metadata for class with name and one parent.  name. If metadata for name does not exists, creates it, with
        initialized class ame and inserts it to container. 
        \param[in] name name of class, which metadata is returned
        \return a metadata
     */
    template<typename _Parent> 
    sad::ClassMetaData* getWithParent(const sad::String& name)
    {
        sad::ClassMetaData* parentMetaData = _Parent::globalMetaData();
        if (parentMetaData != nullptr)
        {
            m_lock.lock();
            bool created = false;
            sad::ClassMetaData* myMetaData = sad::ClassMetaDataContainer::ref()->get(name, created, false);
            myMetaData->addAncestor(parentMetaData);
            m_lock.unlock();
            return myMetaData;
        }
        return nullptr;
    }

    /*! Returns a metadata for class with name and one parent.  name. If metadata for name does not exists, creates it, with
        initialized class ame and inserts it to container.
        \param[in] name name of class, which metadata is returned
        \param[in] index an index
        \return a metadata
    */
    template<typename _Parent>
    sad::ClassMetaData* getWithParentAndIndex(const sad::String& name, unsigned int index)
    {
        sad::ClassMetaData* parentMetaData = _Parent::globalMetaData();
        if (parentMetaData != nullptr)
        {
            m_lock.lock();
            bool created = false;
            sad::ClassMetaData* myMetaData = sad::ClassMetaDataContainer::ref()->get(name, created, false);
            myMetaData->addAncestor(parentMetaData);
            myMetaData->setPrivateIndex(index);
            m_lock.unlock();
            return myMetaData;
        }
        return nullptr;
    }
    /*! Returns a metadata for class with name and two parents.  name. If metadata for name does not exists, creates it, with
        initialized class ame and inserts it to container.
        \param[in] name name of class, which metadata is returned
        \return a metadata
    */
    template<typename _Parent1, typename _Parent2>
    sad::ClassMetaData* getWithParent(const sad::String& name)
    {
        sad::ClassMetaData* parent1MetaData = _Parent1::globalMetaData();
        sad::ClassMetaData* parent2MetaData = _Parent2::globalMetaData();

        if (parent1MetaData != nullptr && parent2MetaData != nullptr)
        {
            m_lock.lock();
            bool created = false;
            sad::ClassMetaData* myMetaData = sad::ClassMetaDataContainer::ref()->get(name, created, false);
            myMetaData->addAncestor(parent1MetaData);
            myMetaData->addAncestor(parent2MetaData);
            m_lock.unlock();
            return myMetaData;
        }
        return nullptr;
    }
    /*! Returns a metadata for class with name and three parents.  name. If metadata for name does not exists, creates it, with
        initialized class ame and inserts it to container.
        \param[in] name name of class, which metadata is returned
        \return a metadata
     */
    template<typename _Parent1, typename _Parent2, typename _Parent3>
    sad::ClassMetaData* getWithParent(const sad::String& name)
    {
        sad::ClassMetaData* parent1MetaData = _Parent1::globalMetaData();
        sad::ClassMetaData* parent2MetaData = _Parent2::globalMetaData();
        sad::ClassMetaData* parent3MetaData = _Parent3::globalMetaData();

        if (parent1MetaData != nullptr && parent2MetaData != nullptr && parent3MetaData != nullptr)
        {
            m_lock.lock();
            bool created = false;
            sad::ClassMetaData* myMetaData = sad::ClassMetaDataContainer::ref()->get(name, created, false);
            myMetaData->addAncestor(parent1MetaData);
            myMetaData->addAncestor(parent2MetaData);
            myMetaData->addAncestor(parent3MetaData);
            m_lock.unlock();
            return myMetaData;
        }
        return nullptr;
    }

    /*! Returns a metadata for class with name and three parents.  name. If metadata for name does not exists, creates it, with
        initialized class ame and inserts it to container.
        \param[in] name name of class, which metadata is returned
        \return a metadata
    */
    template<typename _Parent1, typename _Parent2, typename _Parent3, typename _Parent4>
    sad::ClassMetaData* getWithParent(const sad::String& name)
    {
        sad::ClassMetaData* parent1MetaData = _Parent1::globalMetaData();
        sad::ClassMetaData* parent2MetaData = _Parent2::globalMetaData();
        sad::ClassMetaData* parent3MetaData = _Parent3::globalMetaData();
        sad::ClassMetaData* parent4MetaData = _Parent4::globalMetaData();

        if (parent1MetaData != nullptr && parent2MetaData != nullptr && parent3MetaData != nullptr && parent4MetaData != nullptr)
        {
            m_lock.lock();
            bool created = false;
            sad::ClassMetaData* myMetaData = sad::ClassMetaDataContainer::ref()->get(name, created, false);
            myMetaData->addAncestor(parent1MetaData);
            myMetaData->addAncestor(parent2MetaData);
            myMetaData->addAncestor(parent3MetaData);
            myMetaData->addAncestor(parent4MetaData);

            m_lock.unlock();
            return myMetaData;
        }
        return nullptr;
    }

    /*! Returns a metadata for class with name and one parent, and cast function to cast_class via method.  name. If metadata for name does not exists, creates it, with
        initialized class ame and inserts it to container.
        \param[in] name name of class, which metadata is returned
        \param[in] cast_class a class for cast
        \param[in] f method for casting
        \return metadata
    */
    template<
        typename _Parent
    >
    sad::ClassMetaData* getWithParentAndCast(const sad::String& name, const sad::String cast_class, sad::AbstractClassMetaDataCastFunction* f)
    {
        sad::ClassMetaData* parentMetaData = _Parent::globalMetaData();
        if (parentMetaData != nullptr)
        {
            m_lock.lock();
            bool created = false;
            sad::ClassMetaData* myMetaData = sad::ClassMetaDataContainer::ref()->get(name, created, false);
            myMetaData->addAncestor(parentMetaData);
            myMetaData->addCast(cast_class, f);
            m_lock.unlock();
            return myMetaData;
        }
        return nullptr;
    }
    /*! Returns true if class meta data container contains specified type
        \param[in] name name of class
        \return whether meta data is contained in type
     */
    bool contains(const sad::String & name) const;
    /*! Adds new schema
        \param[in] s schema
     */
    void pushGlobalSchema(sad::db::schema::Schema* s);
    /*! Erases a container
     */
    ~ClassMetaDataContainer();
private:
    typedef sad::Hash<sad::String, sad::ClassMetaData *> ClassMetaDataHash;
    /*! A global schemas to be deleted on app shutdown
     */
    sad::Vector<sad::db::schema::Schema*> m_global_schemas;
    /*! A hash with all of meta data
     */
    ClassMetaDataHash m_container;
    /*! A lock for mutex
     */
    ::sad::Mutex m_lock;
    /*! A lock for stored schemas
     */
    ::sad::Mutex m_schemas_lock;
    /*! Locked, for singleton usage
     */
    ClassMetaDataContainer();
    /*! Locked for singleton usage
     */
    ClassMetaDataContainer(const ClassMetaDataContainer & c);
    /*! Locked for singleton usage
        \param[in] c container
        \return self-reference
     */ 
    sad::ClassMetaDataContainer& operator=(const sad::ClassMetaDataContainer& c);
    /*! An instance for meta data container
     */
    static sad::ClassMetaDataContainer * m_instance;
    /*! Destroys instance of container
     */
    static void destroyInstance();
};

}
