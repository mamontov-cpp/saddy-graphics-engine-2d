#include "resourcetreewidget/resourcetreewidget.h"
#include <QResizeEvent>
#include <QMoveEvent>
#include <QMessageBox>

// A pdding is padding for elements
#define PADDING 6

ResourceTreeWidget::ResourceTreeWidget(QWidget * parent) : QWidget(parent)
{
	m_tree_view = new QTreeWidget(parent);
	QRect r = this->geometry();
	double halfsizenopad = r.width() / 2.0 - PADDING / 2.0;
	m_tree_view->setGeometry(r.x(), r.y(), halfsizenopad, r.height());

	m_tree_view->addTopLevelItem( new QTreeWidgetItem(QStringList("22")) );
	m_tree_view->addTopLevelItem( new QTreeWidgetItem(QStringList("44")) );

	m_element_view = new QTableWidget(parent);
	m_element_view->setGeometry(r.x() + r.width() / 2.0 + PADDING / 2.0, r.y(), halfsizenopad , r.height());

	connect(m_tree_view, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(treeItemChanges(QTreeWidgetItem*, QTreeWidgetItem*)));
}

ResourceTreeWidget::~ResourceTreeWidget()
{
	delete m_tree_view;
	delete m_element_view;
}


void ResourceTreeWidget::resizeEvent( QResizeEvent * e )
{
	QRect r = this->geometry();
	QSize size = e->size();
	double halfsizenopad = size.width() / 2.0 - PADDING / 2.0;
	m_tree_view->setGeometry(r.x(), r.y(), halfsizenopad, size.height());
	m_element_view->setGeometry(r.x() + size.width() / 2.0 + PADDING / 2.0, r.y(), halfsizenopad , size.height());
}

void ResourceTreeWidget::moveEvent( QMoveEvent * e )
{
	QRect oldrect = this->geometry();	
	QRect r(e->pos().x(), e->pos().y(), oldrect.width(), oldrect.height());
	double halfsizenopad = r.width() / 2.0 - PADDING / 2.0;
	m_tree_view->setGeometry(r.x(), r.y(), halfsizenopad, r.height());
	m_element_view->setGeometry(r.x() + r.width() / 2.0 + PADDING / 2.0, r.y(), halfsizenopad , r.height());
}

void	ResourceTreeWidget::treeItemChanges(
	QTreeWidgetItem * current, 
	QTreeWidgetItem * previous
)
{
	QMessageBox::warning(NULL, "1", "2");
}