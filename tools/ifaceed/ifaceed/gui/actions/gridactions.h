/*! \file gridactions.h

    Defines an actions for grids (sad::layouts::Grid)
 */
#pragma once
#include <QObject>

// ReSharper disable once CppUnusedIncludeDirective
#include <QHash>

#include <input/controls.h>

#include <3rdparty/picojson/picojson.h>

#include <db/dbvariant.h>

#include "abstractactions.h"
#include "../childrenprovider.h"
#include "../layouts/layoutcelledit.h"

class MainPanel;
class QCheckBox;

namespace history
{
class Command;
}

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
    : Grid(nullptr), Row(0), Col(0)
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

/*! A cell region, as selection of cells
 */
// ReSharper disable once CppInconsistentNaming
struct CellRegion  // NOLINT(cppcoreguidelines-pro-type-member-init)
{
    /*! A flags for cell region
     */
    enum class Flags: int
    {
        GCRF_OK = 0,       //!< Region is valid
        GCRF_EMPTY = 1,    //!< Region is not selected
        GCRF_INVALID = 2   //!< Region is invalid - not all cells are picked
    } Flags;
    /*! A region selection
     */
    sad::Rect2I Region;
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
enum class GridUpdateOptions: int
{
    GGAUO_Name = 0, //!< Corresponds to name fields updating
    GGAUO_Area = 1, //!< Corresponds to area fields updating
    GGAUO_Rows = 2, //!< Corresponds to row fields updating
    GGAUO_Cols = 3, //!< Corresponds to column fields updating
    GGAUO_TopPadding = 4,     //!< Corresponds to top padding fields updating
    GGAUO_BottomPadding = 5,  //!< Corresponds to bottom padding fields updating
    GGAUO_LeftPadding = 6,    //!< Corresponds to left padding fields updating
    GGAUO_RightPadding = 7,   //!< Corresponds to right padding fields updating
    GGAUO_FixedWidth = 8,     //!< Corresponds to updating fixed width checkbox fields updating
    GGAUO_FixedHeight = 9,    //!< Corresponds to updating fixed height checkbox fields fields updating
    GGAUO_Cells = 10,         //!< Corresponds to updating cell browser
    GGAUO_None = 11,          //!< Corresponds to nothing for cases, when nothing should be updated (or everything in other case)
};

enum class CellUpdateOptions: int
{
    GCAUO_Width = 0,               //!< Corresponds to width field updating,
    GCAUO_Height = 1,              //!< Corresponds to height field updating,
    GCAUO_HorizontalAlignment = 2, //!< Corresponds to horizontal alignment field updating
    GCAUO_VerticalAlignment = 3,   //!< Corresponds to vertical alignment field updating
    GCAUO_StackingType = 4,        //!< Corresponds to stacking type field updating
    GCAUO_TopPadding = 5,          //!< Corresponds to top padding field updating
    GCAUO_BottomPadding = 6,       //!< Corresponds to bottom padding field updating
    GCAUO_LeftPadding = 7,         //!< Corresponds to left padding field updating
    GCAUO_RightPadding = 8         //!< Corresponds to right padding field updating
};
    /*! Creates new label actions
        \param[in] parent a parent object
     */
    GridActions(QObject* parent = nullptr);
    /*! Sets editor
        \param[in] e editor
     */
    virtual void setEditor(core::Editor* e) override;
    /*! This class could be inherited
     */
    virtual ~GridActions() override;
    /*! Returns selected grid, according to editor
        \return a current selected grid
     */
    sad::layouts::Grid* selectedGrid() const;
    /*! Returns list of active grids
        \param[out] grids a list of grids
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
    /*! Tries to insert child to current grid, thus updating ui
        \param[in] g grid
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position
        \param[in] majorid a major id of a node
     */
    void insertChildToGrid(sad::layouts::Grid* g,  size_t row, size_t col, size_t pos, size_t majorid);
    /*! Tries to insert child to current grid, thus updating ui
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
    /*! Updates cell browser's UI from current viewed grid
        \param[in] immediate whether it should be invoked now, or postponed via closure
     */
    void updateCellBrowser(bool immediate = false);
    /*! Updates region, when grid is picked
        \param[in] immediate whether it should be invoked now, or postponed via closure
     */
    void updateRegion(bool immediate = false);
    /*! Update current grid properties in UI, only specified in parameter
        \param[in] group a widget group for grid properties
        \param[in] immediate whether it should be invoked now, or postponed
     */
    void updateOnlyGridPropertiesInUI(
        gui::actions::GridActions::GridUpdateOptions group,
        bool immediate = false
    );
    /*! Update current grid properties in UI, except for those, specified in parameter
        \param[in] group a widget group for grid properties
        \param[in] immediate whether it should be invoked now, or postponed
     */
    void updateGridPropertiesInUIExcept(
        gui::actions::GridActions::GridUpdateOptions group,
        bool immediate = false
    );
    /*! Update current grid properties
        \param[in] immediate whether it should be invoked now, or postponed
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
    void highlightAddingState() const;
    /*! Highlights state, when first point of grid should be placed.
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
    void highlightMovingState() const;
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
        \param[in] parent_pairs a pairs of node, and their positions in grids
     */
    void insertNodesToGrids(const sad::Vector<sad::Pair<sad::SceneNode*, gui::GridPosition> >& parent_pairs);
    /*! Removes grid from editor
        \param grid a grid to be removed (nullptr for selected)
        \param from_editor whether we are removing this from editor
     */
    void scriptableRemoveGrid(sad::layouts::Grid* grid, bool from_editor);
    /*! Tries to recursively update parent grids for grids
        \param[in] grid a grid
        \param[in] immediate whether it should be performed immediately
     */
    void updateParentGridsRecursively(sad::layouts::Grid* grid, bool immediate = true);
    /*! Tries to update regions in children if selected
        \param[in] v list
     */
    void tryUpdateRegionsInChildren(const sad::Vector<sad::SceneNode*>& v);
    /*! Tries to update regions in children if selected
        \param[in] grid a grid
     */
    void tryUpdateRegionsInChildren(sad::layouts::Grid* grid);
    /*! Tries to update regions in children if selected
        \param[in] cell a cell, which contains children to be updated
     */
     // ReSharper disable once CppInconsistentNaming
     void tryUpdateRegionsInChildren(sad::layouts::Cell* cell);
    /*! Updates cell part in UI
        \param[in] row a row of cell
        \param[in] col a column
        \param[in] opts an options
        \param[in] v value
        \param[in] immediate whether it should be performed immediately
     */
    void updateCellPartInUI(
        size_t row,
        size_t col,
        gui::actions::GridActions::CellUpdateOptions opts,
        const sad::db::Variant& v,
        bool immediate = false
    );
    /*! Tries to set cell property in UI if options match specified in template parameters and also
        property could be set
        \param[in] opts options
        \param[in] edit widget to be changed
        \param[in] setter setter value
        \param[in] v variant
     */
    template<
        gui::actions::GridActions::CellUpdateOptions _Opts,
        typename _PropType 
    >
    static void setCellPropertyInUIIf(
        gui::actions::GridActions::CellUpdateOptions opts,
        gui::layouts::LayoutCellEdit* edit,
        void (gui::layouts::LayoutCellEdit::*setter)(_PropType),
        const sad::db::Variant& v
    )
    {
        if (opts == _Opts)
        {
            sad::Maybe< _PropType > mv = v.get< _PropType >();
            if (mv.exists())
            {
                (edit->*setter)(mv.value());
            }
        }
    }
    /*! Tries to set cell property in UI if options match specified in template parameters and also
        property could be set
        \param[in] opts options
        \param[in] edit widget to be changed
        \param[in] setter setter value
        \param[in] v variant
     */
    template<
        gui::actions::GridActions::CellUpdateOptions _Opts,
        typename _PropType 
    >
    static void setCellPropertyInUIIf(
        gui::actions::GridActions::CellUpdateOptions opts,
        gui::layouts::LayoutCellEdit* edit,
        void (gui::layouts::LayoutCellEdit::*setter)(const _PropType&),
        const sad::db::Variant& v
    )
    {
        if (opts == _Opts)
        {
            sad::Maybe< _PropType > mv = v.get< _PropType >();
            if (mv.exists())
            {
                (edit->*setter)(mv.value());
            }
        }
    }
    /*! Tries to set cell property in UI if options match specified in template parameters and also
        property could be set
        \param[in] opts options
        \param[in] edit widget to be changed
        \param[in] setter setter value
        \param[in] v variant
     */
    template<
        gui::actions::GridActions::CellUpdateOptions _Opts,
        typename _PropType 
    >
    static void setCellPropertyInUIIf(
        gui::actions::GridActions::CellUpdateOptions opts,
        gui::layouts::LayoutCellEdit* edit,
        void (gui::layouts::LayoutCellEdit::*setter)(_PropType) const,
        const sad::db::Variant& v
    )
    {
        if (opts == _Opts)
        {
            sad::Maybe< _PropType > mv = v.get< _PropType >();
            if (mv.exists())
            {
                (edit->*setter)(mv.value());
            }
        }
    }
    /*! Tries to set cell property in UI if options match specified in template parameters and also
        property could be set
        \param[in] opts options
        \param[in] edit widget to be changed
        \param[in] setter setter value
        \param[in] v variant
     */
    template<
        gui::actions::GridActions::CellUpdateOptions _Opts,
        typename _PropType 
    >
    static void setCellPropertyInUIIf(
        gui::actions::GridActions::CellUpdateOptions opts,
        gui::layouts::LayoutCellEdit* edit,
        void (gui::layouts::LayoutCellEdit::*setter)(const _PropType&) const,
        const sad::db::Variant& v
    )
    {
        if (opts == _Opts)
        {
            sad::Maybe< _PropType > mv = v.get< _PropType >();
            if (mv.exists())
            {
                (edit->*setter)(mv.value());
            }
        }
    }
    /*! Handles attempt to change area for a grid
        \param[in] grid a grid to be changed
        \param[in] v value for an area
        \param[in] from_editor whether an area is changed from editor
     */
    void tryChangeAreaForGrid(
        sad::layouts::Grid* grid,
        const sad::Rect2D& v,
        bool from_editor = true
    );
    /*! Handles attempt to change name for a grid
        \param[in] grid a grid to be changed
        \param[in] name value for a name
        \param[in] from_editor whether an name is changed from editor
     */
    void tryChangeNameForGrid(
        sad::layouts::Grid* grid,
        const sad::String& name,
        bool from_editor = true
    );
    /*! Handles attempt to change fixed width for a grid
        \param[in] grid a grid to be changed
        \param[in] fixed_width new flag value
        \param[in] from_editor whether a flag is changed from editor
     */
    void tryChangeFixedWidthForGrid(
        sad::layouts::Grid* grid,
        bool fixed_width,
        bool from_editor = true        
    );
    /*! Handles attempt to change fixed height for a grid
        \param[in] grid a grid to be changed
        \param[in] fixed_height new flag value
        \param[in] from_editor whether a flag is changed from editor
     */
    void tryChangeFixedHeightForGrid(
        sad::layouts::Grid* grid,
        bool fixed_height,
        bool from_editor = true
    );
    /*! Handles attempt to change row count for a grid
        \param[in] grid a grid to be changed
        \param[in] rows new amount of rows
        \param[in] from_editor whether a value is changed from editor
     */
    void tryChangeRowCountForGrid(
        sad::layouts::Grid* grid,
        size_t rows,
        bool from_editor = true
    );
    /*! Handles attempt to change column count for a grid
        \param[in] grid a grid to be changed
        \param[in] columns new amount of columns
        \param[in] from_editor whether a value is changed from editor
     */
    void tryChangeColumnCountForGrid(
        sad::layouts::Grid* grid,
        size_t columns,
        bool from_editor = true
    );
    /*! Returns propagation checkbox for padding
        \param[in] opts option for grid
        \return related checkbox
     */
    QCheckBox* propagateCheckboxForPadding(gui::actions::GridActions::GridUpdateOptions opts);
    /*! Applies change in padding to specified grid
        \param[in] opts options for grid
        \param[in] new_value a new value for padding
        \param[in] grid grid to be changed
        \param[in] propagate whether we propagated changes for grid to it's cells
     */
    void applyPaddingChangeToGrid(
        gui::actions::GridActions::GridUpdateOptions opts,
        sad::layouts::Grid* grid,
        double new_value,
        bool propagate
    );
    /*! Handles attempt to change padding for a grid
        \param[in] opts options for grid
        \param[in] grid a grid to be changed
        \param[in] value a new value
        \param[in] propagate whether we propagated changes for grid to it's cells
        \param[in] from_editor whether this was called from editor
     */
    void tryChangePaddingForGrid(
        gui::actions::GridActions::GridUpdateOptions opts,
        sad::layouts::Grid* grid,
        double value,
        bool propagate,
        bool from_editor = true
    );
    /*! Describes actions, which should be performed when some kind of
        padding, specified by options is changed
        \param opts options
        \param new_value new value of padding
     */
    void paddingChanged(
        gui::actions::GridActions::GridUpdateOptions opts,
        double new_value
    );
    /*! Tries to perform merging cells with specified parameters
        \param[in] merge whether we are merging cells (splitting otherwise)
        \param[in] grid a changed grid
        \param[in] row a row for stating region
        \param[in] col a column for stating region
        \param[in] row_span amount of spanning rows of region
        \param[in] col_span amount of spanning columns of region
        \param[in] from_editor whether it's being performed from editor
        \return whether it was successfull
     */
    bool tryPerformMergeOrSplit(
        bool merge,
        sad::layouts::Grid* grid,
        int row,
        int col,
        int row_span,
        int col_span,
        bool from_editor = true
    );
    /*! Tries to move selected grid by keyboard
        \param[in] ev event
     */
    void tryMoveSelectedGridByKeyboard(const sad::input::KeyPressEvent& ev);
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
    /*! Called, when user changes area of grid
        \param[in] new_area assumed new area for a grid
     */
    void areaChanged(QRectF new_area);
    /*! Called, when user wants to navigate to specified cell row
        \param[in] new_value row position
     */
    void navigateCellRow(int new_value);
    /*! Called, when user wants to navigate to specified cell column
        \param[in] new_value column position
    */
    void navigateCellColumn(int new_value);
    /*! Called, when user changes row count for a grid
        \param[in] new_value a new row count for a grid
     */
    void rowCountChanged(int new_value);
    /*! Called, when user changes column count for a grid
        \param[in] new_value a new column count for a grid
     */
    void columnCountChanged(int new_value);
    /*! Called, when user changes whether width is fixed for a grid
        \param[in] new_value a new value for fixed with 
     */
    void fixedWidthClicked(bool new_value);
    /*! Called, when user changes whether height is fixed for a grid
        \param[in] new_value a new value for fixed with 
     */
    void fixedHeightClicked(bool new_value);
    /*! Called, when user changes top padding for a grid
        \param[in] new_value a new top padding value
     */
    void topPaddingChanged(double new_value);
    /*! Called, when user changes bottom padding for a grid
        \param[in] new_value a new bottom padding value
     */
    void bottomPaddingChanged(double new_value);
    /*! Called, when user changes left padding for a grid
        \param[in] new_value a new left padding value
     */
    void leftPaddingChanged(double new_value);
    /*! Called, when user changes right padding for a grid
        \param[in] new_value a new right padding value
     */
    void rightPaddingChanged(double new_value);
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
        \param[in] new_value a new value for width
     */
    void cellWidthChanged(size_t row, size_t col, sad::layouts::LengthValue new_value);
    /*! Called, when cell height is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value for width
     */
    void cellHeightChanged(size_t row, size_t col, sad::layouts::LengthValue new_value);
    /*! Called, when cell horizontal alignment is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value for horizontal alignment
     */
    void cellHorizontalAlignmentChanged(size_t row, size_t col, sad::layouts::HorizontalAlignment new_value);
    /*! Called, when cell vertical alignment is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value for vertical alignment
     */
    void cellVerticalAlignmentChanged(size_t row, size_t col, sad::layouts::VerticalAlignment new_value);
    /*! Called, when cell stacking type is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value for stacking type
     */
    void cellStackingTypeChanged(size_t row, size_t col, sad::layouts::StackingType new_value);
    /*! Called, when cell top padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value
     */
    void cellTopPaddingChanged(size_t row, size_t col, double new_value);
    /*! Called, when cell bottom padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value
     */
    void cellBottomPaddingChanged(size_t row, size_t col, double new_value);
    /*! Called, when cell left padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value
     */
    void cellLeftPaddingChanged(size_t row, size_t col, double new_value);
    /*! Called, when cell right padding is changed
        \param[in] row a row
        \param[in] col a column
        \param[in] new_value a new value
     */
    void cellRightPaddingChanged(size_t row, size_t col, double new_value);
    /*! Called, when cell child is added
        \param[in] row a row
        \param[in] col a column
        \param[in] majorid a major id
        \param[in] node_name a name for a node
     */
    void cellChildAdded(size_t row, size_t col, unsigned long long majorid, QString node_name);
    /*! Called, when cell child is removed
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position, where child is added
     */
    void cellChildRemoved(size_t row, size_t col, size_t pos);
    /*! Called, when cell is cleared
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
    /*! Emitted, when user clicks "Merge" button with intent of merging cell range into one
     */
    void mergeButtonClicked();
    /*! Emitted, when user clicks "Split" button with intent of splitting cell range
     */
    void splitButtonClicked();
private:
    /*! Tries to navigate to specified cell, or merged cell, if this node is part of it
        \param[in] row a row of target cell
        \param[in] column a column of target cell
     */
    void tryNavigateToCell(size_t row, size_t column);
    /*! Returns current cell region for cells
        \return selected cell region
     */
    gui::CellRegion getSelectedCellRegion() const;
    /*! Prepares grid for adding into form
        \return created grid
     */
    sad::layouts::Grid* prepareGridForAdding();
    /*! Makes buckets for sorting
        \param[in] parent_pairs a parent pairs
        \param[out] buckets a buckets list
     */
    void makeBuckets(const sad::Vector<sad::Pair<sad::SceneNode*, gui::GridPosition> >& parent_pairs, gui::SortingBuckets& buckets) const;
    /*! Makes new command, related to changing padding of grid, or nullptr
        if opts is not related to padding
        \param[in] opts options
        \param[in] g grid
        \param[in] old_state an old state of grid
        \param[in] new_state a new state of grid
        \param[in] children a children of grid
        \param[in] propagate whether we propagated changes to grid
        \return created command
     */
    history::Command* makePaddingChangeCommand(
        gui::actions::GridActions::GridUpdateOptions opts,
        sad::layouts::Grid* g,
        const picojson::value& old_state,
        const picojson::value& new_state,
        const sad::Vector<sad::SceneNode*>& children,
        bool propagate
    );
    /*! Makes new command, related to changing padding of grid
        \param[in] g grid
        \param[in] old_state an old state of grid
        \param[in] new_state a new state of grid
        \param[in] children a children of grid
        \param[in] propagate whether we propagated changes to grid
        \return created command
     */
    template<
        gui::actions::GridActions::GridUpdateOptions _Opts
    >
    history::Command* makePaddingChangeCommand(
        sad::layouts::Grid* g,
        const picojson::value& old_state,
        const picojson::value& new_state,
        const sad::Vector<sad::SceneNode*>& children,
        bool propagate
    );
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
