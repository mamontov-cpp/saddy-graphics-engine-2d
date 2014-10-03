/*! \file selection.h
	\author HiddenSeeker

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
};

}