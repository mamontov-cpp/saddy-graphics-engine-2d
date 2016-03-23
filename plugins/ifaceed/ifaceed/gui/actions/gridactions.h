/*! \file gridactions.h

    Defines an actions for grids (sad::layouts::Grid)
 */
#pragma once
#include <QObject>
#include <QRectF>

#include <input/controls.h>
#include "abstractactions.h"

class MainPanel;

namespace gui
{

namespace actions
{

/*! A group of actions, linked to grids (sad::layout::Grid)
 */
class GridActions: public QObject, public gui::actions::AbstractActions
{
Q_OBJECT
public:
    /*! Creates new label actions
        \param[in] parent a parent object
     */
    GridActions(QObject* parent = NULL);
    /*! This class could be inherited
     */
    virtual ~GridActions();
};

}

}
