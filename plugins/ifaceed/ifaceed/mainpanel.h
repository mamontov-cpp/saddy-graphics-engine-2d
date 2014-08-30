/*! \file   mainpanel.h
 *  \author HiddenSeeker
 *
 *  Here described main editor window
 */
#ifndef MAINPANEL_H
#define MAINPANEL_H


#include <QtGui/QMainWindow>
#include "ui_mainpanel.h"

#include <sadstring.h>
#include <sadptrhash.h>

#include "gui/objectlist.h"
#include "gui/table/delegatefactory.h"

#include <db/dbvariant.h>
#include <db/dbstoredpropertyfactory.h>
#pragma once

namespace core
{
class Editor;
}
class AbstractScreenObject;


/*! A main window to edit level objects
 */
class MainPanel : public QMainWindow
{
Q_OBJECT
public:
	/*! Creates new window
		\param[in] parent a parent widget
		\param[in] flags a panel flags
	 */
	MainPanel(QWidget *parent = 0, Qt::WFlags flags = 0);
	/*! Currently does nothing
	 */
	~MainPanel();
	/*! Sets enabled property for editing buttons on panel to specified property
		\param[in] enabled whether buttons are enabled
	 */
	void toggleEditingButtons(bool enabled);
	/*! Determines, whether editing is enabled in main panel
	 */
	bool isEditingEnabled() const;
	/** Sets an editor to work with
		\param[in] editor editor
	 */
	void setEditor(core::Editor * editor);
	/*! Check database for consistency and set palettes, after database was loaded
	 */
	void viewDatabase();
	/*! Returns palette for main palette
		\return palette
	 */
	QList<QList<QColor> > colorPalette() const;
	/*! Sets palette for node's palette
	 */
	void setColorPalette(const QList<QList<QColor> >& palette);
	/*! Takes delegate ownership by propert name
		\param[in] name a name of delegate by property
		\return if ownership is taken - returns true, if panel does not own it - false
	 */
	bool takeDelegateByPropertyName(const QString & name);
	/*! Change region parameters for data
	 */
	void setRegionParameters();
	/*! Updates list of objects
	 */
	void updateList();	
	/*! Synchronizes database with an editor
	 */
	void synchronizeDatabase();
	/*! Sets labels, that performed on mouse move
		\param[in] x x coordinates
		\param[in] y y coordinates
	 */
	void setMouseMovePosView(float x, float y);
	/*! Hints current editor state for highlighting
	 */
	void highlightState(const sad::String & hints);
	/*! Sets, whether adding or removing is enabled
		\param[in] enabled enabled flag
	 */
	void setAddingEnabled(bool enabled);
	/*! Sets, sprite changing enabled
		\param[in] enabled enabled flag
	 */
	void setSpriteChangingEnabled(bool enabled);
	/*! Enables angle changes. If false, also sets angle to zero
		\param[in] enabled 
	 */
	void setAngleChangingEnabled(bool enabled);	
	/*! Tries to set current propertit of active object, otherwise  - of selected
		if property exists
		\param[in] prop property name
		\param[in] v value
	 */
	template<typename T> void trySetProperty(const sad::String & prop, T v);
	/*! Tries to set current propertit of active object, otherwise  - of selected
		if property exists. In a first place this for angle, that is believed as float
		\param[in] prop property name
		\param[in] v value
	 */
	void trySetProperty(const sad::String & prop, float v);	
	inline Ui::MainPanelClass * myUI() { return &ui; }

	void updateObjectStats(AbstractScreenObject * o);
private:
	Ui::MainPanelClass ui;          //!< UI
	core::Editor    *   m_editor;   //!< Editor to work with
	/*! Fills combo box with database property types
	 */
	void fillDatabasePropertyTypesCombo();
protected:
	/*!
	 * An utility flag to prevent events, from self-changing 
	 */
	bool m_selfchanged;
	/*! A property delegates, which belongs only to a loaded database and were here
		before user added any property
	 */
	sad::PtrHash<sad::String, gui::table::Delegate> m_property_delegates;
	/*! A delegate factory for creating rows in db
	 */
	gui::table::DelegateFactory m_dbdelegate_factory;
	/*! A factory for creating propertis in database
	 */
	sad::db::StoredPropertyFactory m_property_factory;
	/*!
	 * Whether panel is closed it must close a dialogs if present
	 */
	void closeEvent(QCloseEvent*);
	/*! Fixes database scenes and scene nodes tables and palette if need to
	 */
	void fixDatabase();
protected slots:
	/*! Adds a property to database slot
	 */
	void addDatabaseProperty();
	/*! TODO: Remove
	 */
	void selected(sad::String item);
	/*! A slot, when sprite rectangle changed
	 */
	void spriteRectChanged();

	void spriteSelected(QString config, QString group, int index);
	/** Implementation of setMouseMovePosView
	 */
	void setMouseMovePosViewImpl();
	/** Implementation of highlightState
	 */
	void highlightStateImpl();
	/*! Adds a new font object
	 */
	void addFontObject();
	/*! Adds a sprite object
	 */
	void addSpriteObject();
	/*! Changed an angle in data
		\param[in] angle angle
	 */
	void angleChanged(double angle);
	/*! Whether color is changed
		\param[in] index of color
	 */
	void colorChanged(int index);
	/*! Whether label object text changed
	 */
	void textChanged();
	/*! Emitted when object in list is changed
	 */
	void selectedObjectChanged(int index);
	/*! Moves object back to layer
	 */
	void moveObjectBack();
	/*! Moves object back to layer
	 */
	void moveObjectFront();
	/*! Called, when name is changed
		\param[in] name new name of element
	 */
	void nameChanged(const QString & name);
	/*! Makes selected item a background
	 */
	void makeBackground();
	/*! Clears a screen template
	 */
	void clearScreenTemplate();
	/*! Repeats history
	 */
	void repeatHistoryChange();
	/*! Rollbacks history
	 */
	void rollbackHistoryChange();

	/*! Handled, when color changed
		\param[in] c color
	 */
	void colorChanged(QColor c);
private:
	/** Data for moving x
	 */
	float m_tmp_mov_x;
	/** Data for moving y
	 */
	float m_tmp_mov_y;
	/** State of main panel (temporary for slot)
	 */
	QString m_tmp_state;
	/** An object list for working with information
	 */
	gui::ScreenObjectList m_list;
};

#endif // MAINPANEL_H
