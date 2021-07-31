/*! \file sadstringdelegate.h
    

    A delegate for property, which contains sad::String values
 */
#pragma once
#include "delegate.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <sadstring.h>

namespace gui
{
    
namespace table
{
/*! A delegate for editing values for sad::String values
 */
class SadStringDelegate: public gui::table::Delegate
{
Q_OBJECT
public:
    /*! Constructs new delegate
     */
    SadStringDelegate();
    /*! Destroys delegate
     */
    virtual ~SadStringDelegate() override;
    /*! Sets a value for a delegate. Used by commands to reset
        \param[in] v a value
     */
    virtual void set(const sad::db::Variant& v) override;
public slots:
    /*! Called, when widget is changed
     */
    void widgetChanged();
    /*! Called, when more button is clicked
     */
    void moreClicked();
protected:
    /*! At this point delegate must create own editor, set it to table and connect delegate slots
        to it.
     */
    virtual void makeEditor() override;
    /*! Disconnects slots  for own editor here
     */ 
    virtual void disconnect() override;
};

}

}
