/*! \file resourcetreewidget\resourcetreewidget.h
	\author HiddenSeeker

	Describes a widget, which displays a resource tree from a renderer, by it's value
 */
#pragma once
#include <QWidget>
#include <QTreeWidget>
#include <QTableWidget>

class ResourceTreeWidget: public QWidget
{
	Q_OBJECT
public: 
	/*! Constructs new resource tree widget 
	 */
	ResourceTreeWidget(QWidget * parent = NULL);
	/*! Destroys data from a widget
	 */
	~ResourceTreeWidget();
protected:
	/*! A widget to view a tree, from a renderer of resources
	 */ 
	QTreeWidget  * m_tree_view;
	/*! A widget to view elements of specific tree folder
	 */
	QTableWidget * m_element_view;
	/*! Handles resize, resizing elements
		\param[in] e event
	 */
	virtual void resizeEvent( QResizeEvent * e );
	/*! Handles movement, moving elements
		\param[in] e event
	 */
	virtual void moveEvent( QMoveEvent * e );
protected slots:
	/*! Activated, when tree picked item is changed
		\param[in] current new item
		\param[in] previous previous item
	 */
	void  treeItemChanges(QTreeWidgetItem * current, QTreeWidgetItem * previous);
};
