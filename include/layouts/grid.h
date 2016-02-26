/*! \file grid.h
	
	Describes a table, which contains cells with child nodes, arranging them in table.
	Note, that cells can contains grids inside them, allowing nesting inside of grid
 */
#pragma once
#include "../scenenode.h"
#include "../sadptrvector.h"
#include "../sadhash.h"
#include "cell.h"

namespace sad
{

namespace layouts
{
/*! Describes a table, which contains cells with child nodes, arranging them in table.
	Note, that cells can contains grids inside them, allowing nesting inside of grid.
	
	Note, that grid is expanded automatically, as new cells are added, so area property
	may change, due to large inner nodes.
 */ 
class Grid: public sad::SceneNode
{
SAD_OBJECT	
public:
	/*! Constructs a default grid with no cells
	 */
	Grid();
	/*! Grid could be inherited
	 */
	virtual ~Grid();
	/*! Allows rendering of a grid
	 */ 
	virtual void render();
    /*! Sets links resources to a treename and renderer
        \param[in] r renderer
        \param[in] treename a name for tree
     */
    virtual void setTreeName(sad::Renderer* r, const sad::String& treename);
	/*! Loads grid from picojson object. Used to recompute all cells, after grid is loaded
        \param[in] v a picojson object
        \return  whether it as successfull
     */
    virtual bool load(const picojson::value& v);
    /*! Fills vector of regions with exactly one region, describing an area of grid
        \param[out] r a vector of regions
     */
    virtual void regions(sad::Vector<sad::Rect2D> & r);
    /*! A basic schema for object
        \return a schema 
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;	
    /*! Called, when renderer for scene is changed
     */
    virtual void rendererChanged();
    /*! Sets an area for table
        \param[in] r area for table
     */
    void setArea(const sad::Rect2D & r);
    /*! Returns area for label
        \return area for label
     */
    sad::Rect2D area() const;	
	/*! Forces grid to recompute all items for cells
	 */ 
	void update();	
private:
	/*! The grid is not copyable
		\param[in] grid a grid		
	 */
	Grid(const sad::layouts::Grid& grid);
	/*! The grid is not copyable
		\param[in] grid a grid
		\return self-reference
	 */
	sad::layouts::Grid& operator=(const sad::layouts::Grid& grid);
	
	/*! An area for a grid
	 */
	sad::Rect2D m_area;
	/*! An amount of rows
	 */
	unsigned int m_rows;
	/*! An amount of columns
	 */
	unsigned int m_cols;
	/*! A list of cells
	 */
	sad::PtrVector<sad::layouts::Cell> m_cells;
	/*! A map, which maps rows to columns, to indexes of cells		
	 */ 
	sad::Hash<unsigned int, sad::Hash<unsigned int, unsigned int> > m_map;
	/*! A default top padding for all cells
	 */
	double m_padding_top;
	/*! A default bottom padding for all cells
	 */
	double m_padding_bottom;
	/*! A default left padding for all cells
	 */
	double m_padding_left;
	/*! A default right padding for all cells
	 */
	double m_padding_right;	
};

}	
	
}


DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::layouts::Grid);