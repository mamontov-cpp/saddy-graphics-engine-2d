/*! \file   mainpanel.h
 *  \author HiddenSeeker
 *
 *  Here described main editor window
 */
#ifndef MAINPANEL_H
#define MAINPANEL_H

#include <QtGui/QMainWindow>
#include "ui_mainpanel.h"


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

private:
	Ui::MainPanelClass ui;  //!< UI
protected:
	/**
	 * Whether panel is closed it must close a dialogs if present
	 */
	void closeEvent(QCloseEvent*);
protected slots:
	/**
	 * Cleanups a system fonts if presented
 	 */
	void ifCleanupSystemFonts(int);
};

#endif // MAINPANEL_H
