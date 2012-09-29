/*! \file   mainpanel.h
 *  \author HiddenSeeker
 *
 *  Here described main editor window
 */
#ifndef MAINPANEL_H
#define MAINPANEL_H

#include <QtGui/QMainWindow>
#include "ui_mainpanel.h"


class IFaceEditor;

/*! A main window to edit level objects
 */
class MainPanel : public QMainWindow
{
	Q_OBJECT

public:
	/*! Constructor, which inits all of callbacks
	 */
	MainPanel(QWidget *parent = 0, Qt::WFlags flags = 0);
	/*! Currently does nothing
	 */
	~MainPanel();

	/** Sets an editor to work with
		\param[in] editor editor
	 */
	inline void setEditor(IFaceEditor * editor) {  m_editor = editor; }

	/** Synchronizes database with an editor
	 */
	void synchronizeDatabase();
private:
	Ui::MainPanelClass ui;         //!< UI
	IFaceEditor    *   m_editor;   //!< Editor to work with
protected:
	/**
	 * Whether panel is closed it must close a dialogs if present
	 */
	void closeEvent(QCloseEvent*);
};

#endif // MAINPANEL_H
