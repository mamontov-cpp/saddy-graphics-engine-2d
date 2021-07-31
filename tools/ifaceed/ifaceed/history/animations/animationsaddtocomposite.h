/*! \file animationsaddtocomposite.h
 *  
 *
 * Describes a simple command for adding other animations to a list of composite animation
 */
#pragma once
#include "../command.h"
#include <animations/animationscomposite.h>

namespace history
{

namespace animations
{

/*! \class history::animations::AddToComposite
 *   A simple command for adding to composite animation
 */
class AddToComposite: public history::Command
{
public:
    /*! Creates new command
        \param[in] a animations
        \param[in] id id of animation
     */
    AddToComposite(sad::animations::Composite* a, unsigned long long id);
    /*! Could be inherited
     */
    virtual ~AddToComposite() override;
    /*! Makes animation active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Makes animation inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A current selected animation
     */
    sad::animations::Composite* m_animation;
    /*! An id of added animations
     */
    unsigned long long m_id;
};

}

}
