/*! \file unsignedshortdelegate.h
    

    A delegate for property, which contains unsigned short values
 */
#pragma once
#include "delegate.h"

namespace gui
{
    
namespace table
{
/*! A delegate for editing values for unsigned short values
 */
class UnsignedShortDelegate: public gui::table::Delegate
{
Q_OBJECT
public:
    /*! Constructs new delegate
     */
    UnsignedShortDelegate();
    /*! Destroys delegate
     */
    virtual ~UnsignedShortDelegate() override;
    /*! Sets a value for a delegate. Used by commands to reset
        \param[in] v a value
     */
    virtual void set(const sad::db::Variant& v) override;
public slots:
    /*! When widget changed
        \param[in] i a widget value
     */
    void widgetChanged(int i);
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
