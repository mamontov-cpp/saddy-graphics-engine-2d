/*! \file resourcetreewidget\resourcetreewidget.h
    

    Describes a widget, which displays a resource tree from a renderer, by it's value
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
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
    ResourceTreeWidget(QWidget * parent = nullptr);
    /*! Destroys data from a widget
     */
    ~ResourceTreeWidget();
    /*! A resource cache for a widget
        \return a resource cache for a widget
     */
    gui::resourcetreewidget::ResourceCache * cache();
    /*! Returns padding between two sub-widgets
        \return padding between two sub-widgets
     */
    double padding() const;
    /*! Sets padding between two sub-widgets
        \return padding between two sub-widgets
     */
    void setPadding(double padding);
    /*! Sets tree, which is being displayed in widget
     */
    void setTree(const QString & name);
    /*! Returns a tree, which is being displayed in widget
     */
    const QString & tree() const;
    /*! Sets filter for a resource tree widget
        \param[in] filter a filter for a tree
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
    /*! Populates tree with an items
        \param[in, out] parent_item a parent item for widget
        \param[in] parent_folder a folder
        \param[in] suitability a suitability map
     */
    void populateTree(
        QTreeWidgetItem* parent_item, 
        sad::resource::Folder* parent_folder,
        const  QHash<sad::resource::Folder*, bool>& suitability
    );
    /*! Seeks suitable folders, which has resources of specified types
        \param[in] current_folder a folder to be scanned
        \param[in] suitability a suitability map
        \return whether folder has suitable assets
     */ 
    bool findSuitableFolders(
        sad::resource::Folder* current_folder,
        QHash<sad::resource::Folder*, bool>& suitability
    );
    /*! Returns selected folder for item
        \param[in] item item to be selected
        \return value
     */
    sad::Maybe<sad::String> selectedFolder(QTreeWidgetItem * item) const;
    /*! Returns selected local path to a resource
     */ 
    sad::Maybe<sad::String> selectedLocalPathToResource() const;
    /*! Tries to restore selection in resource tree widget
        \param[in] folder a folder to restore
        \param[in] resource_local a local resource name
     */
    void tryRestoreSelection(
        const sad::Maybe<sad::String> & folder,
        const sad::Maybe<sad::String> & resource_local
    );
    /*! Handles resize, resizing elements
        \param[in] e event
     */
    virtual void resizeEvent( QResizeEvent * e ) override;
    /*! Handles movement, moving elements
        \param[in] e event
     */
    virtual void moveEvent( QMoveEvent * e ) override;
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