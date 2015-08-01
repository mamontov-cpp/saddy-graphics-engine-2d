/*! \file resourcetreewidget\resourcetreewidget.h
	

	Describes a widget, which displays a resource tree from a renderer, by it's value
 */
#pragma once
#include <QWidget>
#include <QTreeWidget>
#include <QTableWidget>

#include <resource/folder.h>

namespace gui
{

namespace resourcetreewidget
{
class ResourceCache;


class ResourceTreeWidget: public QWidget
{
	Q_OBJECT
signals:
	/*! Emitted, when user changed selection
		\param[in] s selected resources
	 */
	void selectionChanged(sad::String s);
public: 
	/*! Constructs new resource tree widget 
	 */
	ResourceTreeWidget(QWidget * parent = NULL);
	/*! Destroys data from a widget
	 */
	~ResourceTreeWidget();
	/*! A resource cache for a widget
		\return a resource cache for a widget
	 */
	gui::resourcetreewidget::ResourceCache * cache();
	/*! Returns padding between two subwidgets
		\return padding between two subwidgets
	 */
	double padding() const;
	/*! Sets padding between two subwidgets
		\return padding between two subwidgets
	 */
	void setPadding(double padding);
	/*! Sets tree, which is being displayed in widget
	 */
	void setTree(const QString & name);
	/*! Returns a tree, which is beign displayed in widget
	 */
	const QString & tree() const;
	/*! Sets filter for a resource tree widget
		\param[in] filter a flter for a tree
	 */
	void setFilter(const QString & filter);
	/*! Returns a filter for a widget
		\return filter for items of a widget
	 */
	const QString & filter() const;
	/*! Updates a resource tree widget
	 */
	virtual void updateTree();
	/*! Describes path to tree widget item, by selection
		\param[in] name a name of resource 
		\return path to resource if can be reconstructed
	 */
	sad::Maybe<sad::String> pathToItemBySelection(const QString & name);
	/*! Sets selected resource name
		\param[in] name a name for selected resource
	 */
	void setSelectedResourceName(const sad::String & name);
	/*! Returns selected resource name, if any resource was selected
		\return selected resource name
	 */
	sad::Maybe<sad::String> selectedResourceName() const;
	/*! Returns selected resource
		\return selected resource
	 */
	sad::resource::Resource* selectedResource() const;
protected slots:
	/*! Activated, when selected item in tree is changed
		\param[in] current new item
		\param[in] previous previous item
	 */
	void  treeItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);
	/*! Activated, when selected item in table is changed
		\param[in] current new item
		\param[in] previous previous item
	 */
	void  elementItemChanged(QTableWidgetItem * current, QTableWidgetItem * previous);
protected:
	void populateTree(
		QTreeWidgetItem * parentitem, 
		sad::resource::Folder * parentfolder
	);
	/*! Returns seletected folder for item
		\param[in] item item to be selected
		\return value
	 */
	sad::Maybe<sad::String> selectedFolder(QTreeWidgetItem * item) const;
	/*! Returns selected local path to a resource
	 */ 
	sad::Maybe<sad::String> selectedLocalPathToResource() const;
	/*! Tries to restore selection in resource tree widget
		\param[in] folder a folder to restore
		\param[in] resourcelocal a local resource name
	 */
	void tryRestoreSelection(
		const sad::Maybe<sad::String> & folder,
		const sad::Maybe<sad::String> & resourcelocal
	);
	/*! Handles resize, resizing elements
		\param[in] e event
	 */
	virtual void resizeEvent( QResizeEvent * e );
	/*! Handles movement, moving elements
		\param[in] e event
	 */
	virtual void moveEvent( QMoveEvent * e );
	/*! Resizes widgets, making them fit to tree
		\param[in] r a rectangle
	 */
	void resizeWidgets(const QRect & r);

	/*! A widget to view a tree, from a renderer of resources
	 */ 
	QTreeWidget  * m_tree_view;
	/*! A widget to view elements of specific tree folder
	 */
	QTableWidget * m_element_view;
	/*! A spacing between two widget
	 */
	double m_padding;
	/*! A name for a tree, which is being displayed in widget
	 */
	QString m_tree_name;
	/*! A filter for getting elements. Must contain possible types or and use "|"
		as delimiter
	 */
	QString m_filter;
	/*! A cache for resource images inside of widget
	 */
	gui::resourcetreewidget::ResourceCache * m_cache;
};

}

}