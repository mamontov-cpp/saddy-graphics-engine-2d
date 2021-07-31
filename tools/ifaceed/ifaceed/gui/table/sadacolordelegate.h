/*! \file sadacolordelegate.h
    

    A delegate for property, which contains sad::AColor values
 */
#pragma once
#include "delegate.h"

namespace gui
{
    
namespace table
{
/*! A delegate for sad::AColor values
 */
class SadAColorDelegate: public gui::table::Delegate
{
Q_OBJECT
public:
    /*! Constructs new delegate
     */
    SadAColorDelegate();
    /*! Destroys delegate
     */
    virtual ~SadAColorDelegate() override;
    /*! Sets a value for a delegate. Used by commands to reset
        \param[in] v a value
     */
    virtual void set(const sad::db::Variant& v) override;
public slots:
    /*! User asked to change a color
     */
    void clicked();
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
