/*! \file gridactions.h

    Defines an actions for grids (sad::layouts::Grid)
 */
#pragma once
#include <QObject>
#include <QHash>

#include <input/controls.h>
#include "abstractactions.h"
#include "../childrenprovider.h"
#include "../layouts/layoutcelledit.h"

class MainPanel;

namespace gui
{
/*! A cell location in grid
 */ 
struct CellLocation
{
    sad::layouts::Grid* Grid; //!<  A grid
    size_t Row;               //!<  A row of related cell
    size_t Col;               //!<  A column of related cell

    /*! A default constructor
     */
    inline CellLocation()
    : Grid(NULL), Row(0), Col(0)
    {
        
    }
    /*! A default constructor
        \param[in] grid a grid
        \param[in] row a row
        \param[in] col a column
     */
    inline CellLocation(sad::layouts::Grid* grid, size_t row, size_t col)
    : Grid(grid), Row(row), Col(col)
    {
        
    }
    /*! Returns true if both arguments are equal
        \param[in] o other object
        \return whether two objects are equal
     */
    inline bool operator==(const gui::CellLocation& o) const
    {
        return Grid == o.Grid && Row == o.Row && Col == o.Col;
    }
};
/*! A node location in cell
 */
struct NodeLocationInCell
{
    sad::SceneNode* Node; //!< A node
    size_t Pos;  //!< A position of node in cell
    /*! A default constructor
        \param[in] node a node
        \param[in] pos a position
     */
    inline NodeLocationInCell(sad::SceneNode* node, size_t pos)
    : Node(node), Pos(pos)
    {
        
    }
    /*! Returns true if other object's position is greater than current
        \param[in] o object
        \return other
     */
    inline bool operator<(const NodeLocationInCell & o) const
    {
        return this->Pos < o.Pos;
    }
    /*! Returns true if other object's position is lesser than current
        \param[in] o object
        \return other
     */
    inline bool operator>(const NodeLocationInCell & o) const
    {
        return this->Pos > o.Pos;
    }
};
/*! A sorting bucket for inserting or erasing from list
 */
struct SortingBucket
{
    CellLocation Cell;
    sad::Vector<NodeLocationInCell> List;
};
/*! A list of sorting buckets
 */
typedef sad::Vector<SortingBucket> SortingBuckets; 
/*! A position of scene node in grids
 */
struct GridPosition
{
    sad::layouts::Grid* Grid; //!<  A grid
    size_t Row;               //!<  A row of related cell
    size_t Col;               //!<  A column of related cell
    size_t Pos;               //!<  A position of cell grid

    /*! Makes new grid position
        \param[in] g grid
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position
     */
    inline GridPosition(sad::layouts::Grid* g, size_t row, size_t col,  size_t pos)
    : Grid(g), Row(row), Col(col), Pos(pos)
    {
        
    }
};

namespace actions
{

/*! A group of actions, linked to grids (sad::layout::Grid)
 */
class GridActions: public QObject, public gui::actions::AbstractActions
{
Q_OBJECT
public:
/*! A options for including for excluding GUI elements for 
    grids to a batch update actions
 */
enum UpdateOptions
{
    GAUO_Name = 0, //!< Corresponds to name fields updating
    GAUO_Area = 1, //!< Corresponds to area fields updating
    GAUO_Rows = 2, //!< Corresponds to row fields updating
    GAUO_Cols = 3, //!< Corresponds to column fields updating
    GAUO_TopPadding = 4,     //!< Corresponds to top padding fields updating
    GAUO_BottomPadding = 5,  //!< Corresponds to bottom padding fields updating
    GAUO_LeftPadding = 6,    //!< Corresponds to left padding fields updating
    GAUO_RightPadding = 7,   //!< Corresponds to right padding fields updating
    GAUO_FixedWidth = 8,     //!< Corresponds to updating fixed width checkbox fields updating
    GAUO_FixedHeight = 9,    //!< Corresponds to updating fixed height checkbox fields fields updating
    GAUO_Cells = 10,         //!< Corresponds to updating cell browser
    GAUO_None = 11,          //!< Corresponds to nothing for cases, when nothing should be updated (or everything in other case)
};
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
    /*! Returns list of active grids
        \param[out] list a list of grids
     */
    static void activeGrids(sad::Vector<sad::layouts::Grid*>& grids);
    /*! Adds a grid to list of grids
        \param[in] grid a grid to be added
     */
    void addGridToGridList(sad::layouts::Grid* grid) const;
    /*! Insert a grid to list of grids
        \param[in] grid a grid to be added
        \param[in] position a position of grid
     */
    void insertGridToGridList(sad::layouts::Grid* grid, int position) const;
    /*! Removes last grid from list of grids, updating UI
     */
    void removeLastGrid();
    /*! Tries to insert child to curent grid, thus updating ui
        \param[in] g grid
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position
        \param[in] node a node
     */
    void insertChildToGrid(sad::layouts::Grid* g,  size_t row, size_t col, size_t pos, size_t majorid);
    /*! Tries to insert child to curent grid, thus updating ui
        \param[in] g grid
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position
        \param[in] node a node
     */
    void insertChildToGrid(sad::layouts::Grid* g,  size_t row, size_t col, size_t pos, sad::SceneNode* node);
    /*! Tries to remove child from grid, affecting UI
        \param[in] g grid
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position
     */
    void removeChildFromGridInUI(sad::layouts::Grid* g, size_t row, size_t col, size_t pos);
    /*! Removes child from a grid, affecting UI. Note, that this does not affect history.
        \param[in] g grid
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position
     */
    void removeChildFromGrid(sad::layouts::Grid* g, size_t row, size_t col, size_t pos);
    /*! Inserts new cell editor
        \param[in] row a row
        \param[in] col a column
        \param[in] cell a cell editor
     */
    void insertCellEditor(size_t row,  size_t col, gui::layouts::LayoutCellEdit* cell);
    /*! Tries to update child name
        \param[in] g grid
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position
        \param[in] name a new name
     */
    void updateChildName(sad::layouts::Grid* g,  size_t row, size_t col, size_t pos, const QString& name);
    /*! Updates cell browser's UI from current vieewed grid
        \param[in] immediate whether it should be invoed now, or postponed via closure
     */
    void updateCellBrowser(bool immediate = false);
    /*! Updates region, when grid is picked
        \param[in] immediate whether it should be invoked now, or postponed via closure
     */
    void updateRegion(bool immediate = false);
    /*! Update current grid properties in UI, only specified in parameter
        \param[in] group a widget group for grid properties
        \param[in] immediate whether it should be invoked now, or postoponed
     */
    void updateOnlyGridPropertiesInUI(
        gui::actions::GridActions::UpdateOptions group,
        bool immediate = false
    );
    /*! Update current grid properties in UI, except for those, specified in parameter
        \param[in] group a widget group for grid properties
        \param[in] immediate whether it should be invoked now, or postoponed
     */
    void updateGridPropertiesInUIExcept(
        gui::actions::GridActions::UpdateOptions group,
        bool immediate = false
    );
    /*! Update current grid properties
        \param[in] immediate whether it should be invoked now, or postoponed
     */
    void updateGridPropertiesInUI(bool immediate = false);
    /*! Returns a cell editor
        \param[in] row a row
        \param[in] col a column
        \return a cell editor
     */
    gui::layouts::LayoutCellEdit* cellEditor(size_t row,  size_t col);
    /*! Seeks for related grids
        \param[in] node a node
        \return a grid position
     */
    static sad::Vector<gui::GridPosition> findRelatedGrids(sad::SceneNode* node);
    /*! Should clear browser for cells
     */
    void clearGridCellsBrowser();
    /*! Highlights adding state in main panel status box
     */
    void higlightAddingState() const;
    /*! Higlights state, when first point of grid should be placed.
        This should occur when user presses "Add By Stretching" button
     */
    void highlightPlaceFirstPointState() const;
    /*! Highlights state, when second point of grid should be placed.
        This should occur when user pressed "Add By Stretching" button
        and already placed top-left corner of screen
     */
    void highlightPlaceSecondPointState() const;
    /*! Highlights adding state in main panel status box
     */
    void higlightMovingState() const;
    /*! Highlights resizing state in main panel status box
     */
    void highlightResizingState() const;
    /*! Cancels adding grid, entering previous state
     */
    void cancelAddGrid();
    /*! Moves center of active or selected node to a mouse position.
        Note, that this will not preserve history, so use it only for
        active node.
        \param[in] e event
     */
    void moveByCenter(const sad::input::MouseMoveEvent& e);
    /*! Moves pivot point selected node to a mouse position.
        \param[in] e event
     */
    void moveByPivotPoint(const sad::input::MouseMoveEvent& e);
    /*! Commits moving grid to a mouse position
        \param[in] e event
     */ 
    void commitMovingGrid(const sad::input::MouseReleaseEvent& e);
    /*! Resizes current grid, according to current hotspot
        \param[in] e event
     */
    void resizeGridUsingHotspot(const sad::input::MouseMoveEvent& e);
    /*! Commits grid resizing to history, using current hotspot
        \param[in] e event
     */
    void commitGridResizingUsingHotspot(const sad::input::MouseReleaseEvent& e);
    /*! Moves bottom-right corner of active grid to a mouse position.
        Note, that this will not preserve history, so use it only for
        active node.
        \param[in] e event
     */
    void moveByBottomRightCorner(const sad::input::MouseMoveEvent& e);
    /*! Removes grid from list of grids, flushing selection if need to
        \param[in] g grid
     */
    void removeGridFromGridList(sad::layouts::Grid* g);
    /*! Commits grid adding to a scene
        \param[in] e event
     */
    void commitGridAdd(const sad::input::MousePressEvent& e);
    /*! Commits picking top left point for a new grid, when
        picking "Add By Stretching"
        \param[in] e event
     */
    void commitTopLeftPoint(const sad::input::MousePressEvent& e);
    /*! Sets children editing enabled for a grid
        \param[in] enabled whether it's enabled or not
     */
    void setChildEditingEnabled(bool enabled);
    /*! Forces editor to enable editing area controls
     */
    void enableEditingAreaControls();
    /*! Cancels selection of node, wiping selected widgets
     */
    void cancelSelection();
    /*! Returns true if object is within grid
        \param[in] node a node
        \return true if object is within grid
     */
    bool isInGrid(sad::SceneNode* node);
    /*! Returns a parent grid for node, or nothing if not found
        \param node a scene node
        \return parent grid
     */
    sad::layouts::Grid* parentGridFor(sad::SceneNode* node);
    /*! Inserts node to grid entry
        \param node a node
        \param g a parent grid
     */
    void insertNodeToGridEntry(sad::SceneNode* node, sad::layouts::Grid* g);
    /*! Removes node to grid entry
        \param node a node, which link should be removed
     */
    void eraseNodeToGridEntry(sad::SceneNode* node);
    /*! Clears cache grid from grid to parent
     */
    void clearNodeToGridCache();
    /*! Tries to update node name in grid if parent grid for node is selected
        \param[in] node a node
     */
    void tryUpdateNodeNameInGrid(sad::SceneNode* node);
    /*! Finds parent grids for specified node list
        \param[in] list a list of nodes
        \param[out] parent_pairs list of parent pairs
     */
    void findParentGrids(
        const sad::Vector<sad::SceneNode*>& list,
        sad::Vector<sad::Pair<sad::SceneNode*, gui::GridPosition> >& parent_pairs
    ) const;
    /*! Erases nodes from their grids, updating them if needed
        \param[in] parent_pairs a pairs of node, and their positions in grids
     */
    void eraseNodesFromGrids(const sad::Vector<sad::Pair<sad::SceneNode*, gui::GridPosition> >& parent_pairs);
    /*! Inserts nodes to their grids, updating them if needed
        \param[in] parent_apairs a pairs of node, and their positions in grids
     */
    void insertNodesToGrids(const sad::Vector<sad::Pair<sad::SceneNode*, gui::GridPosition> >& parent_pairs);
    /*! Removes grid from editor
        \param grid a grid to be removed (NULL for selected)
        \param from_editor whether we are removing this from editor
     */
    void scriptableRemoveGrid(sad::layouts::Grid* grid, bool from_editor);
public slots:
    /*! Called, when user clicks on "Add" button for grids
     */
    void addGridClicked();
    /*! Called, when user clicked on "Remove" button for grids
     */
    void removeGridClicked();
    /*! Called, when user clicks on "Add By Stretching" button for grids
     */
    void addGridByStretchingClicked();
    /*!
       Called, when user changes area of grid
       \param newarea
     */
    void areaChanged(QRectF newarea);
    /*! Called, when user toggles show or hide grids
        \param[in] state a new state for a checkbox
     */
    void showGridsClicked(bool state);
    /*! Called, when user changes current grid
        \param[in] row a row of current grid
     */
    void currentGridChanged(int row);
    /*! Called, when user changes current text name of node
        \param[in] text a new name
     */
    void nameChanged(const QString &text);
    /*! Called, when cell width is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value for width
     */
    void cellWidthChanged(size_t row, size_t col, sad::layouts::LengthValue newvalue);
    /*! Called, when cell height is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value for width
     */
    void cellHeightChanged(size_t row, size_t col, sad::layouts::LengthValue newvalue);
    /*! Called, when cell horizontal alignment is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value for horizontal alignment
     */
    void cellHorizontalAlignmentChanged(size_t row, size_t col, sad::layouts::HorizontalAlignment newvalue);
    /*! Called, when cell vertical alignment is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value for vertical alignment
     */
    void cellVerticalAlignmentChanged(size_t row, size_t col, sad::layouts::VerticalAlignment newvalue);
    /*! Called, when cell stacking type is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value for stacking type
     */
    void cellStackingTypeChanged(size_t row, size_t col, sad::layouts::StackingType newvalue);
    /*! Called, when cell top padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value
     */
    void cellTopPaddingChanged(size_t row, size_t col, double newvalue);
    /*! Called, when cell bottom padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value
     */
    void cellBottomPaddingChanged(size_t row, size_t col, double newvalue);
    /*! Called, when cell left padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value
     */
    void cellLeftPaddingChanged(size_t row, size_t col, double newvalue);
    /*! Called, when cell right padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] newvalue a new value
     */
    void cellRightPaddingChanged(size_t row, size_t col, double newvalue);
    /*! Called, when cell child is added
        \param[in] row a row
        \param[in] col a column
        \param[in] majorid a major id
        \param[in] nodename a name for a node
     */
    void cellChildAdded(size_t row, size_t col, unsigned long long majorid, QString nodename);
    /*! Called, when cell child is removed
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position, where child is added
     */
    void cellChildRemoved(size_t row, size_t col_t, size_t pos);
    /*! Called, when cell is clearead
        \param[in] row a row
        \param[in] col a column
     */
    void cellCleared(size_t row, size_t col);
    /*! Emitted, when cell children are swapped
        \param[in] row a row
        \param[in] col a column
        \param[in] pos1 a first position of item
        \param[in] pos2 a second position of item
     */
    void cellChildrenSwapped(size_t row, size_t col, size_t pos1, size_t pos2);
private:
    /*! Prepares grid for adding into form
        \return created grid
     */
    sad::layouts::Grid* prepareGridForAdding();
    /*! Makes buckets for sorting
        \param[in] parent_pairs
        \param[out] buckets a buckets list
     */
    void makeBuckets(const sad::Vector<sad::Pair<sad::SceneNode*, gui::GridPosition> >& parent_pairs, gui::SortingBuckets& buckets) const;
    /*! A provider for grid cells
     */
    gui::ChildrenProvider* m_provider;
    /*! A hash table from actions to local editors for provider
     */
    QHash<size_t, QHash<size_t, gui::layouts::LayoutCellEdit*> > m_cell_editors; 
    /*! A hash, which connects node to parent grid
     */
    QHash<sad::SceneNode*, sad::layouts::Grid*> m_grid_to_parent;
    /*! A previous machine state for main editor's state machine. This is used
        to store previous state in case of multi-state transitions.
        "Add By Stretching" process is the only one now.
     */
    sad::String m_previous_machine_state;
    /*! Whether we adding a grid by stretching now. Must be used in case, if editor
        is in adding state for grids.
     */
    bool m_is_stretching;
};

}

}
