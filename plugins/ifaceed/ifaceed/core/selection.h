/*! \file selection.h
	

	Defines a selection for an editor
 */
#pragma once
#include <QObject>
#include <QTimer>

#include <sadvector.h>
#include <scenenode.h>

#include <input/events.h>

namespace core
{

namespace borders
{
class ResizeHotspot;
}

class Editor;

/*! A selection algorithm for an editor
 */
class Selection: public QObject
{
Q_OBJECT
public:
	/*! Creates new unlinked delection
	 */
	Selection();
	/*! Can be inherited
	 */
	virtual ~Selection();
	/*! Sets editor for selection
		\param[in] e editor
	 */
	void setEditor(core::Editor* e);
	/*! Returns editor
		\return editor
	 */
	core::Editor* editor() const;
	/*! Defines a timeout for finishing selecting object in milliseconds
	 */
	static const int TIMEOUT;
	/*! Tries to select an object on mouse press event
		\param[in] e event object
	 */
	void trySelect(const sad::input::MousePressEvent& e);
	/*! Navigates selection on mouse wheel
		\param[in] e event object
	 */
	void navigateSelection(const sad::input::MouseWheelEvent& e);
    /*! Tries to enter to moving state, with selected object
        \param[in] e event object
     */
    void tryEnterToMovingStateWithWayObject(const sad::input::MousePressEvent& e);
	/*!  Is selection chain pending
	 */
	bool isSelectionPending() const;
protected slots:
	/*! Disables navigation on selection
	 */
	void disableSelectionNavigation();
	/*! Starts/restart timer for selection
	 */
	void startTimer();
	/*! Forces editor to enter moving state
		\param[in] e event object
		\return whether we should stop handling selection
	 */
	bool forceEditorEnterMovingState(const sad::input::MousePressEvent& e);
	/*! Forces editor to enter resizing state
		\param[in] h hotspot for resizing
		\param[in] e event object
	 */
	void forceEditorEnterResizingState(
		core::borders::ResizeHotspot* h,
		const sad::input::MousePressEvent& e
	);
	/*! Removes item from selection
	 */
	void removeItem();
protected:
	/*! An editor, linked to selection
	 */
	core::Editor* m_editor;
	/*! A timer for handling some results 
	 */
	QTimer m_timer;
	/*! Whether, we should change selection on wheel rotate
	 */
	bool m_selection_change;
	/*! A selection chain for editing items
	 */
	sad::Vector<sad::SceneNode*> m_selection_chain;
	/*! A current position of element in navigation chain
	 */
	int m_current_position;

    /*! Tries to select an object on mouse press event
        \param[in] e event object
     */
    void trySelectObject(const sad::input::MousePressEvent& e);
    /*! Tries to select a way on mouse press event
        \param[in] e event object
     */
    void trySelectWay(const sad::input::MousePressEvent& e);
    /*! Commits selection of way
        \param[in] i index of way
        \param[in] j  index of way point
     */
    void commitWaySelection(int i, int j);
    /*! Commits idle way selection
     */
    void commitIdleWaySelection();
};

}
