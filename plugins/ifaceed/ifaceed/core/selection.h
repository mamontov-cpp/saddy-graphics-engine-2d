/*! \file selection.h
	\author HiddenSeeker

	Defines a selection for an editor
 */
#pragma once
#include <QObject>

namespace core
{

class Editor;

/*! A selection for an editor
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
protected:
	/*! An editor, linked to selection
	 */
	core::Editor* m_editor;
};

}