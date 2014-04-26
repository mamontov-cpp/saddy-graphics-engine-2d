#include "resourcetreewidget/resourcetreewidget.h"
#include "resourcetreewidget/resourcecache.h"
#include "resourcetreewidget/celldelegate.h"

#include <QResizeEvent>
#include <QMoveEvent>
#include <QMessageBox>

#include <renderer.h>

ResourceTreeWidget::ResourceTreeWidget(QWidget * parent) 
: QWidget(parent), m_padding(6), m_tree_name("")
{
	m_tree_view = new QTreeWidget(parent);

	QStringList headerLabels;
	headerLabels << "";
	m_tree_view->setHeaderLabels(headerLabels);

	m_element_view = new QTableWidget(parent);

	CellDelegate* mydelegate = new CellDelegate();
	m_element_view->setItemDelegate(mydelegate);
	mydelegate->setParent(m_element_view);

	m_cache = new resourcetreewidget::ResourceCache();
	m_cache->setParent(this);

	resizeWidgets(this->geometry());

	connect(m_tree_view, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(treeItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
}

resourcetreewidget::ResourceCache * ResourceTreeWidget::cache()
{
	return m_cache;
}

ResourceTreeWidget::~ResourceTreeWidget()
{
	delete m_tree_view;
	delete m_element_view;
}

double ResourceTreeWidget::padding() const
{
	return m_padding;
}

void ResourceTreeWidget::setPadding(double padding)
{
	m_padding = padding;
	resizeWidgets(this->geometry());
}

void ResourceTreeWidget::setTree(const QString & name)
{
	m_tree_name = name;
	updateTree();
}

const QString & ResourceTreeWidget::tree() const
{
	return m_tree_name;
}

void ResourceTreeWidget::setFilter(const QString & filter)
{
	m_filter = filter;
	updateTree();
}

const QString & ResourceTreeWidget::filter() const
{
	return m_filter;
}

void ResourceTreeWidget::updateTree()
{
	m_tree_view->clear();
	m_element_view->clear();

	QTreeWidgetItem * root = new QTreeWidgetItem(QStringList("/"));
	m_tree_view->addTopLevelItem(root);

	sad::resource::Tree * tree = sad::Renderer::ref()->tree(m_tree_name.toStdString());
	if (tree)
	{
		sad::resource::Folder * folderroot = tree->root();
		populateTree(root, folderroot);
	}
}

sad::Maybe<sad::String> ResourceTreeWidget::pathToItemBySelection(const QString & name)
{
	sad::Maybe<sad::String> result;
	QList<QTreeWidgetItem *> items = m_tree_view->selectedItems();
	if (items.count())
	{
		sad::String path = this->selectedFolder(items[0]).value();
		if (path.length() != 0)
		{
			path += "/";
			path += name.toStdString();
			result.setValue(path);
		} 
		else
		{
			result.setValue(name.toStdString());
		}
	}
	return result;
}

void	ResourceTreeWidget::treeItemChanged(
	QTreeWidgetItem * current, 
	QTreeWidgetItem * previous
)
{
	if (current)
	{
		sad::Maybe<sad::String> v = selectedFolder(current);
		if (v.exists())
		{
			QMessageBox::warning(NULL, "1", v.value().c_str());
		}
	}
	
}

sad::Maybe<sad::String> ResourceTreeWidget::selectedFolder(QTreeWidgetItem * item)
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
		sad::String path = treepath.join("/").toStdString();
		value.setValue(path);
	}
	return value;
}

void ResourceTreeWidget::populateTree(
		QTreeWidgetItem * parentitem, 
		sad::resource::Folder * parentfolder
)
{
	sad::resource::FolderIterator it = parentfolder->folderListBegin();
	while(it != parentfolder->folderListEnd())
	{
		QTreeWidgetItem * item = new QTreeWidgetItem(QStringList(it.key().c_str()));
		parentitem->addChild(item);
		populateTree(item, it.value());
		++it;
	}
}

void ResourceTreeWidget::resizeEvent( QResizeEvent * e )
{
	QRect oldrect = this->geometry();
	QRect r(oldrect.x(), oldrect.y(), e->size().width(), e->size().height());
	resizeWidgets(r);
}

void ResourceTreeWidget::moveEvent( QMoveEvent * e )
{
	QRect oldrect = this->geometry();	
	QRect r(e->pos().x(), e->pos().y(), oldrect.width(), oldrect.height());
	resizeWidgets(r);	
}

void ResourceTreeWidget::resizeWidgets(const QRect & r)
{
	double halfsizenopad = r.width() / 2.0 - this->padding() / 2.0;
	m_tree_view->setGeometry(r.x(), r.y(), halfsizenopad, r.height());
	double element_view_x = r.x() + r.width() / 2.0 + this->padding() / 2.0;
	m_element_view->setGeometry(element_view_x, r.y(), halfsizenopad , r.height());
}
