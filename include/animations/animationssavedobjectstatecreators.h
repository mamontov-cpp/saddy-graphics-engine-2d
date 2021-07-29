/*! \file animations/animationssavedobjectstatecreators.h
    

    Defines a list of creators, which should be stored in animation
 */
#pragma once
#include "animationsabstractsavedobjectstatecreator.h"

#include "animationssavedobjectpropertycreator.h"
#include "animationssavedobjectstatecreator.h"

#include "../sadptrvector.h"

namespace sad
{

namespace animations
{
/*! Defines a list of creators for saved object states
 */
class SavedObjectStateCreators: public sad::PtrVector<sad::animations::AbstractSavedObjectStateCreator>
{
public:
    /*! Creates new empty container
     */
    SavedObjectStateCreators();
    /*! Copies other container
        \param[in] o copied container
     */
    SavedObjectStateCreators(const sad::animations::SavedObjectStateCreators& o);
    /*! Copies other container
        \param[in] o other container
        \return *this
     */
    sad::animations::SavedObjectStateCreators& operator=(const sad::animations::SavedObjectStateCreators& o);
    /*! Frees all stored creators
     */
    virtual ~SavedObjectStateCreators() override;
    /*! Pushes new saved property creator to container
        \param[in] name  a key to be stored in container
        \param[in] property_name a property name, which should be saved
     */
    template<typename T>
    void pushProperty(const sad::String& name, const sad::String& property_name)
    {
        this->push_back( new sad::animations::SavedObjectPropertyCreator<T>(name, property_name) );
    }
    /*! Pushes new saved object state creator for type T to container
        \param[in] name a name for creator to be stored in container
     */
    template<typename T>
    void pushCreator(const sad::String& name)
    {
        this->push_back(new sad::animations::SavedObjectStateCreator<T>(name));
    }
};

}

}
