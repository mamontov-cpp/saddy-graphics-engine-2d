/*! \file animationsswapincomposite.h
 *  
 *
 * Describes a simple command for swapping anim
 */
#pragma once
#include "../command.h"
#include <animations/animationscomposite.h>

#include <QListWidget>

namespace history
{

namespace animations
{

/*! \class history::animations::SwapInComposite
 *   A simple command for swap in list of composite animation
 */
class SwapInComposite: public history::Command
{
public:
    /*! Creates new command
        \param[in] a animations
        \param[in] pos1 first position
        \param[in] pos2 second position
     */
    SwapInComposite(sad::animations::Composite* a, int pos1, int pos2);
    /*! Could be inherited
     */
    virtual ~SwapInComposite();
    /*! Makes animation active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Makes animation inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! Swaps items with positions with list
        \param[in] w widget
     */
    void swapItemsInList(QListWidget* w);
    /*! A current selected animation
     */
    sad::animations::Composite* m_animation;
    /*! A first position
     */
    int m_position1;
    /*! A second position
     */
    int m_position2;
};

}

}
