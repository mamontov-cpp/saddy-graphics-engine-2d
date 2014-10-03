/*! \file   editorbehaviourshareddata.h
	\author HiddenSeeker

	Defines an editor's data, shared between all editor's states
 */
#pragma once
#include <stdio.h>

#include <QTimer>

#include <sadstring.h>
#include <scenenode.h>
#include <sadrect.h>

#include <hfsm/hfsmshared.h>

#include <log/log.h>

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
};

}
