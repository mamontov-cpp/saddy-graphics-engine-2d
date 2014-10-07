/*! \file   editorbehaviourshareddata.h
	\author HiddenSeeker

	Defines an editor's data, shared between all editor's states
 */
#pragma once
#include <stdio.h>

#include <QTimer>
#include <QString>

#include <sadstring.h>
#include <scenenode.h>
#include <sadrect.h>

#include <hfsm/hfsmshared.h>

#include <log/log.h>

#include <p2d/vector.h>

namespace core
{

class Editor;

/*! An editor's data. shared between all editor's state. Contains active and selected objects.
	Active object is an object, which will be placed (in case of state, of object adding)
 */
class Shared: public sad::hfsm::Shared
{
public:	 
	/*! Creates a default empty shared data
	 */
	Shared();
	/*! This class could be inherited
	 */
	virtual ~Shared();
	/*! Sets editor, which data belong to
		\param[in] e editor
	 */
	void setEditor(core::Editor* e);
	/*! Returns an editor, which data belons to
		\return editor
	 */
	core::Editor* editor() const;
	/*! Sets selected object
		\param[in] o object
	 */
	void setSelectedObject(sad::SceneNode* o);
	/*! Sets active object
		\param[in] o object
	 */
	void setActiveObject(sad::SceneNode* o);
	/*! Returns selected object
		\return selected object
	 */
	sad::SceneNode* selectedObject() const;
	/*! Returns active object
		\return active object
	 */
	sad::SceneNode* activeObject() const;
	/*! Whether we can show active border
		\return whether we can show active border;
	 */
	bool canShowActiveBorder() const;
	/*! Could force hiding active border if set to false. You can disable force hiding by setting it to
		true
		\param[in] flag a flag value
	 */
	void toggleActiveBorder(bool flag);
	/*! Sets pivot point for moving or resizing element
		\param[in] p pivot point
     */
	void setPivotPoint(const sad::Point2D& p);
	/*! Returns a pivot point for moving or resizing element
		\return pivot point
	 */
	const sad::Point2D& pivotPoint() const;
	/*! Sets old area for  moving or resizing elements
		\param[in] r rectangle
	 */
	void setOldArea(const sad::Rect2D& r);
	/*! Returns old area of element for moving or resizing elements
		\return rectangle
	 */
	const sad::Rect2D& oldArea() const;
	/*! Whether selected object is resizeable
		\return whether it can be resized
	 */
	bool isSelectionResizeable() const;
	/*! Sets indexes of source area for resizing element
		\param[in] indexes an indexes to be resized
	 */
	void setResizingIndexes(const sad::Pair<int, int>& indexes);
	/*! Returns indexes of source area for resizing element
		\return indexes of resizing elements
	 */
	const sad::Pair<int, int>& resizingIndexes() const;
	/*! Sets a direction for resizing element
		\param[in] direction a direction for resizing
	 */
	void setResizingDirection(const sad::p2d::Vector& direction);
	/*! Returns direction for resizing element
		\return direction for resizing element
	 */
	const sad::p2d::Vector& resizingDirection() const;
	/*! Sets a normalized direction for resizing element
		\param[in] direction a direction for resizing
	 */
	void setNormalizedResizingDirection(const sad::p2d::Vector& direction);
	/*! Returns normalized direction for resizing element
		\return direction for resizing element
	 */
	const sad::p2d::Vector& normalizedResizingDirection() const;
	/*! Sets file name for current database
		\param[in] name a name for editor
	 */ 
	void setFileName(const QString& name);
	/*! Returns file name for saved database
		\return file name
	 */
	const QString& fileName() const;
	/*! Sets old region
		\param[in] r region
	 */
	void setOldRegion(const sad::Rect2D& r);
	/*! Returns old region for object
		\return old region
	 */
	const sad::Rect2D& oldRegion() const;
private:
	/*! A scene node, selected by user
	 */ 
	sad::SceneNode* m_selected;
	/*! An active scene node
	 */
	sad::SceneNode* m_active;
	/*! A linked editor, which shared data belongs to
	 */
	core::Editor* m_editor;
	/*! Should we show or hide active border
	 */
	bool  m_show_active_border;  
	/*! A pivot point for moving or resizing items
	 */
	sad::Point2D m_pivot_point;
	/*! An old area for moving or resizing items
	 */
	sad::Rect2D m_old_area;
	/*! An old region for object
	 */
	sad::Rect2D m_old_region;
	/*! A node types, which can't be resized
	 */
	sad::Vector<sad::String> m_nonresizeable_nodetypes;
	/*! An indexes, which should be used for resizing objects 
	 */
	sad::Pair<int, int> m_resizing_indexes;
	/*! A direction for resizing indexes
	 */	 
	sad::p2d::Vector m_resizing_direction;
	/*! A normalized resizing direction
	 */
	sad::p2d::Vector m_normalized_resizing_direction;
	/*! A file name
	 */
	QString m_filename;
};

}
