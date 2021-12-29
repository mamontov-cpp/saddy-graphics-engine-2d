#include "gui/resourcetreewidget/resourcetreewidget.h"
#include "gui/resourcetreewidget/resourcecache.h"
#include "gui/resourcetreewidget/celldelegate.h"
#include "gui/resourcetreewidget/cell.h"
#include "gui/resourcetreewidget/defaultimage.h"

#include "../../qstdstring.h"

#include <QResizeEvent>
#include <QMoveEvent>
#include <QMessageBox>

#include <renderer.h>
#include <maybe.h>
#include <sadstring.h>

#include <texturemappedfont.h>

#include <QImage>

#if HAVE_QT5
    #include <QHeaderView>
#endif

Q_DECLARE_METATYPE(sad::String);

gui::resourcetreewidget::ResourceTreeWidget::ResourceTreeWidget(QWidget * parent) 
: QWidget(parent), m_padding(6), m_tree_name("")
{
    m_tree_view = new QTreeWidget(parent);

    // Cleanup header of tree view
    QStringList headerLabels;
    headerLabels << "";
    m_tree_view->setHeaderLabels(headerLabels);
    m_tree_view->header()->close();

    m_element_view = new QTableWidget(parent);
    m_element_view->horizontalHeader()->hide();
    m_element_view->verticalHeader()->hide();
    m_element_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_element_view->setSelectionBehavior(QAbstractItemView::SelectItems);

    gui::resourcetreewidget::CellDelegate* mydelegate = new gui::resourcetreewidget::CellDelegate();
    mydelegate->setParent(m_element_view);
    mydelegate->setResourceTreeWidget(this);
    m_element_view->setItemDelegate(mydelegate);

    m_cache = new gui::resourcetreewidget::ResourceCache();
    m_cache->setParent(this);

    resizeWidgets(this->geometry());

    connect(m_tree_view, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(treeItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(m_element_view, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)), this, SLOT(elementItemChanged(QTableWidgetItem*, QTableWidgetItem*)));
}

gui::resourcetreewidget::ResourceCache * gui::resourcetreewidget::ResourceTreeWidget::cache()
{
    return m_cache;
}

gui::resourcetreewidget::ResourceTreeWidget::~ResourceTreeWidget()
{
    delete m_tree_view;
    delete m_element_view;
}

double gui::resourcetreewidget::ResourceTreeWidget::padding() const
{
    return m_padding;
}

void gui::resourcetreewidget::ResourceTreeWidget::setPadding(double padding)
{
    m_padding = padding;
    resizeWidgets(this->geometry());
}

void gui::resourcetreewidget::ResourceTreeWidget::setTree(const QString & name)
{
    m_tree_name = name;
    updateTree();
}

const QString & gui::resourcetreewidget::ResourceTreeWidget::tree() const
{
    return m_tree_name;
}

void gui::resourcetreewidget::ResourceTreeWidget::setFilter(const QString & filter)
{
    m_filter = filter;
    updateTree();
}

const QString & gui::resourcetreewidget::ResourceTreeWidget::filter() const
{
    return m_filter;
}

void gui::resourcetreewidget::ResourceTreeWidget::updateTree()
{
    m_cache->clear();

    sad::Maybe<sad::String> path;
    QList<QTreeWidgetItem *> items = m_tree_view->selectedItems();
    if (items.size())
    {
        path =	this->selectedFolder(items[0]);
    } 

    sad::Maybe<sad::String> resourcename;
    if (path.exists())
    {
        resourcename = this->selectedLocalPathToResource();
    }

    m_tree_view->clear();
    m_element_view->clear();

    QTreeWidgetItem * root = new QTreeWidgetItem(QStringList("/"));
    m_tree_view->addTopLevelItem(root);

    sad::resource::Tree * tree = sad::Renderer::ref()->tree(Q2STDSTRING(m_tree_name));
    if (tree)
    {
        sad::resource::Folder * folderroot = tree->root();
        QHash<sad::resource::Folder*, bool> suitability;
        findSuitableFolders(folderroot, suitability);
        populateTree(root, folderroot, suitability);
    }
    if (items.size())
    {
        tryRestoreSelection(path, resourcename);
    }
    else
    {
        if (m_tree_view->topLevelItemCount())
        {
            m_tree_view->setCurrentItem(m_tree_view->topLevelItem(0));
        }
    }
}

sad::Maybe<sad::String> gui::resourcetreewidget::ResourceTreeWidget::pathToItemBySelection(
    const QString & name
)
{
    sad::Maybe<sad::String> result;
    QList<QTreeWidgetItem *> items = m_tree_view->selectedItems();
    if (items.count())
    {
        sad::String path = this->selectedFolder(items[0]).value();
        if (path.length() != 0)
        {
            path += "/";
            path += Q2STDSTRING(name);
            result.setValue(path);
        } 
        else
        {
            result.setValue(Q2STDSTRING(name));
        }
    }
    return result;
}

void gui::resourcetreewidget::ResourceTreeWidget::setSelectedResourceName(
    const sad::String & name
)
{
    sad::StringList list = name.split("/");
    if (list.size() == 1)
    {
        this->tryRestoreSelection(sad::Maybe<sad::String>(""), sad::Maybe<sad::String>(name));
    }
    else
    {
        if (list.size() != 0) 
        {
            sad::String resourcename = list[list.size() - 1];
            list.removeAt(list.size() - 1);
            sad::String path = sad::join(list, "/");
            this->tryRestoreSelection(
                sad::Maybe<sad::String>(path), 
                sad::Maybe<sad::String>(resourcename)
            );
        }
        else
        {
            this->tryRestoreSelection(sad::Maybe<sad::String>(""), sad::Maybe<sad::String>(name));
        }
    }
}


sad::Maybe<sad::String> gui::resourcetreewidget::ResourceTreeWidget::selectedResourceName() const
{
    QList<QTreeWidgetItem *> items = m_tree_view->selectedItems();
    sad::Maybe<sad::String> result;
    if (items.size())
    {
        sad::Maybe<sad::String> path = this->selectedFolder(items[0]);
        if (path.exists())
        {
            sad::Maybe<sad::String> localresource = this->selectedLocalPathToResource();
            if (localresource.exists())
            {
                if (path.value().length() == 0)
                {
                    result = localresource;
                }
                else
                {
                    result.setValue(path.value() + "/" + localresource.value());
                }
            }
        }
    }
    return result;
}

sad::resource::Resource* gui::resourcetreewidget::ResourceTreeWidget::selectedResource() const
{
    sad::Maybe<sad::String> path = this->selectedResourceName();
    sad::resource::Resource * result = nullptr;

    sad::resource::Tree * tree = sad::Renderer::ref()->tree(Q2STDSTRING(m_tree_name));
    sad::resource::Folder * folder = tree->root();
    if (path.exists())
    {
        result = folder->resource(path.value());
    }
    return result;
}

void	gui::resourcetreewidget::ResourceTreeWidget::treeItemChanged(
    QTreeWidgetItem * current, 
    QTreeWidgetItem * /*previous*/
)
{
    if (current)
    {
        sad::Maybe<sad::String> v = selectedFolder(current);
        if (v.exists())
        {
            m_element_view->clear();
            sad::resource::Tree * tree = sad::Renderer::ref()->tree(Q2STDSTRING(m_tree_name));
            sad::resource::Folder * folder = tree->root();
            if (v.value().length() != 0)
            {
                folder = tree->root()->folder(v.value());
            }
            sad::resource::ResourceIterator cur = folder->resourceListBegin();
            
            // Count rows in item
            unsigned int rows = 0;
            bool odd = false;
            QStringList list = m_filter.split("|");
            for(; cur != folder->resourceListEnd(); ++cur)
            {
                bool shouldshowresource = true;
                if (list.count())
                {
                    const sad::String & name = cur.value()->metaData()->name();
                    shouldshowresource = list.indexOf(STD2QSTRING(name)) != -1;
                }
                if (shouldshowresource)
                {
                    if (odd)
                    {
                        rows += 1;
                        odd = false;
                    }
                    else
                    {
                        odd = true;
                    }
                }
            }
            if (odd)
            {
                rows += 1;
            }
            m_element_view->setRowCount(rows);
            m_element_view->setColumnCount(2);

            // Fill table
            int row = 0;
            int column = 0;
            cur = folder->resourceListBegin();
            for(; cur != folder->resourceListEnd(); ++cur)
            {
                bool shouldshowitem = true;
                if (list.count())
                {
                    shouldshowitem = list.indexOf(STD2QSTRING(cur.value()->metaData()->name())) != -1;
                }
                if (shouldshowitem)
                {
                    QTableWidgetItem * item = new QTableWidgetItem(STD2QSTRING(cur.key()));
                    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                    item->setSizeHint(QSize(
                        gui::resourcetreewidget::Cell::Width, 
                        gui::resourcetreewidget::Cell::Height
                    ));
                    m_element_view->setItem(row, column, item);
                    m_element_view->resizeColumnsToContents();
                    m_element_view->resizeRowsToContents();
                    m_element_view->update();
                    if (column == 0)
                    {
                        column = 1;
                    }
                    else
                    {
                        column = 0;
                        row += 1;
                    }
                }
            }			
        }
    }
    else
    {
        m_element_view->clear();
    }
}

void gui::resourcetreewidget::ResourceTreeWidget::elementItemChanged(
    QTableWidgetItem * current, 
    QTableWidgetItem * /*previous*/
)
{
    if (current)
    {
        sad::String name = Q2STDSTRING(current->text());
        QList<QTreeWidgetItem *> list = m_tree_view->selectedItems();
        if (list.count())
        {
            sad::Maybe<sad::String> path = this->selectedFolder(list[0]);
            if (path.exists())
            {
                sad::String result = path.value();
                if (result.length())
                {
                    result += "/";
                    result += name;
                }
                else
                {
                    result = name;
                }
                emit selectionChanged(result);
            }
        }
    }
}

sad::Maybe<sad::String> gui::resourcetreewidget::ResourceTreeWidget::selectedFolder(
    QTreeWidgetItem * item
) const
{
    sad::Maybe<sad::String> value;
    if (item)
    {
        QStringList treepath;
        while(item)
        {
            treepath << item->text(0);
            item = item->parent();
        }
        treepath.removeAt(treepath.size() - 1);
        std::reverse(treepath.begin(), treepath.end());
        sad::String path = Q2STDSTRING(treepath.join("/"));
        value.setValue(path);
    }
    return value;
}

sad::Maybe<sad::String> gui::resourcetreewidget::ResourceTreeWidget::selectedLocalPathToResource() const
{
    QList<QTableWidgetItem *> elements = m_element_view->selectedItems();
    sad::Maybe<sad::String> result;
    if (elements.count())
    {
        result.setValue(Q2STDSTRING(elements[0]->text()));
    }
    return result;
}

void gui::resourcetreewidget::ResourceTreeWidget::tryRestoreSelection(
    const sad::Maybe<sad::String> & folder,
    const sad::Maybe<sad::String> & resource_local
)
{
    if (folder.exists())
    {
        sad::String pathvalue = folder.value();
        bool found = false;
        QTreeWidgetItem * current = nullptr;
        if (pathvalue.length() == 0)
        {
            current = m_tree_view->topLevelItem(0);
            m_tree_view->setCurrentItem(current, 0);
            found = true;
        }
        else
        {
            sad::StringList list = pathvalue.split("/");
            current = m_tree_view->topLevelItem(0);
            for(int i = 0; i < list.count() && (current != nullptr); i++)
            {
                bool has_specified_item = false;
                for(int j = 0; j < current->childCount() && !has_specified_item; j++)
                {
                    QTreeWidgetItem* child_item = current->child(j);
                    if (child_item)
                    {
                        if (Q2STDSTRING(child_item->text(0)) == list[i].c_str())
                        {
                            current = child_item;
                            has_specified_item = true;
                        }
                    }
                }
                if (!has_specified_item)
                {
                    current = nullptr;
                }
            }
            if (current)
            {
                m_tree_view->setCurrentItem(current, 0);
                found = true;
            }
        }

        if (found)
        {
            this->treeItemChanged(current, nullptr);
            if (resource_local.exists())
            {
                // Try set selection for local
                QList<QTableWidgetItem *> items = m_element_view->findItems(STD2QSTRING(resource_local.value()), Qt::MatchFixedString | Qt::MatchCaseSensitive);
                if (items.count())
                {
                    QTableWidgetItem* item = items[0];
                    bool b = m_element_view->blockSignals(true);
                    m_element_view->setCurrentItem(item);
                    m_element_view->blockSignals(b);
                    m_element_view->update();
                }
            }
        }
    }
}

void gui::resourcetreewidget::ResourceTreeWidget::populateTree(
        QTreeWidgetItem * parent_item, 
        sad::resource::Folder * parent_folder,
        const  QHash<sad::resource::Folder*, bool>& suitability
)
{
    sad::resource::FolderIterator it = parent_folder->folderListBegin();
    while(it != parent_folder->folderListEnd())
    {
        if (suitability.contains(it.value()))
        {
            if (suitability[it.value()])
            {
                QTreeWidgetItem * item = new QTreeWidgetItem(QStringList(STD2QSTRING(it.key())));
                parent_item->addChild(item);
                populateTree(item, it.value(), suitability);
            }
        }
        ++it;
    }
}

 bool gui::resourcetreewidget::ResourceTreeWidget::findSuitableFolders(
        sad::resource::Folder* current_folder,
        QHash<sad::resource::Folder*, bool>& suitability
)
{
    bool result = false;
    sad::resource::FolderIterator it = current_folder->folderListBegin();
    while(it != current_folder->folderListEnd())
    {
        bool suitabilityValue = findSuitableFolders(it.value(), suitability);
        result = result || suitabilityValue;
        ++it;
    }

    sad::resource::ResourceIterator cur = current_folder->resourceListBegin();
            
    QStringList list = m_filter.split("|");
    for(; cur != current_folder->resourceListEnd(); ++cur)
    {
        if (list.count())
        {
            const sad::String & name = cur.value()->metaData()->name();
            bool hasSuitableItem = list.indexOf(STD2QSTRING(name)) != -1;
            result = result || hasSuitableItem;
        }		
    }

    suitability.insert(current_folder, result);
    return result;
}

void gui::resourcetreewidget::ResourceTreeWidget::resizeEvent( QResizeEvent * e )
{
    QRect oldrect = this->geometry();
    QRect r(oldrect.x(), oldrect.y(), e->size().width(), e->size().height());
    resizeWidgets(r);
}

void gui::resourcetreewidget::ResourceTreeWidget::moveEvent( QMoveEvent * e )
{
    QRect oldrect = this->geometry();	
    QRect r(e->pos().x(), e->pos().y(), oldrect.width(), oldrect.height());
    resizeWidgets(r);	
}

void gui::resourcetreewidget::ResourceTreeWidget::resizeWidgets(const QRect & r)
{
    double halfsizenopad = r.width() / 2.0 - this->padding() / 2.0;
    m_tree_view->setGeometry(r.x(), r.y(), halfsizenopad, r.height());
    double element_view_x = r.x() + r.width() / 2.0 + this->padding() / 2.0;
    m_element_view->setGeometry(element_view_x, r.y(), halfsizenopad , r.height());
}
