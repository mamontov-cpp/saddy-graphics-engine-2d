/*! \file gridactions.h

    Defines an actions for grids (sad::layouts::Grid)
 */
#pragma once
#include <QObject>
#include <QRectF>

#include <input/controls.h>
#include "abstractactions.h"
#include "../childrenprovider.h"
#include "../layouts/layoutcelledit.h"

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
    /*! Sets editor
        \param[in] e editor
     */
    virtual void setEditor(core::Editor* e);
    /*! This class could be inherited
     */
    virtual ~GridActions();
	/*! Returns selected grid, according to editor
		\param[in] grid a current grid
	 */
	sad::layouts::Grid* selectedGrid() const;
	/*! Inserts new cell editor
		\param[in] row a row
		\param[in] col a column
		\param[in] cell a cell editor
	 */
	void insertCellEditor(size_t row,  size_t col, gui::layouts::LayoutCellEdit* cell);
	/*! Returns a cell editor
		\param[in] row a row
		\param[in] col a column
		\return a cell editor
	 */
	gui::layouts::LayoutCellEdit* cellEditor(size_t row,  size_t col);
private:
	/*! A provider for grid action
	 */
	gui::ChildrenProvider* m_provider;
	/*! A hash table from actions to local editors for provider
	 */
	QHash<size_t, QHash<size_t, gui::layouts::LayoutCellEdit*> > m_cell_editors; 
};

}

}
