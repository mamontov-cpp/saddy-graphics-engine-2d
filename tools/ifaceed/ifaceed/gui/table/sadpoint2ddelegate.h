/*! \file sadpoint2ddelegate.h
    

    A delegate for property, which contains sad::Point2D values
 */
#pragma once
#include "delegate.h"

#include "../tuplewidget/doubletuplewidget.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <sadpoint.h>

namespace gui
{
    
namespace table
{
/*! A delegate for editing values for sad::Point2D values
 */
class SadPoint2DDelegate: public gui::table::Delegate
{
Q_OBJECT
public:
    /*! Constructs new delegate
     */
    SadPoint2DDelegate();
    /*! Destroys delegate
     */
    virtual ~SadPoint2DDelegate() override;
    /*! Sets a value for a delegate. Used by commands to reset
        \param[in] v a value
     */
    virtual void set(const sad::db::Variant& v) override;
public slots:
    /*! When widget changed
        \param[in] f a first value
        \param[in] s a second value
     */
    void widgetChanged(double f, double s);
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
