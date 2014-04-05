#include "resourcetreewidget/resourcetreewidget.h"
#include <QResizeEvent>
#include <QMoveEvent>
#include <QMessageBox>

#include <renderer.h>

ResourceTreeWidget::ResourceTreeWidget(QWidget * parent) 
: QWidget(parent), m_padding(6), m_tree_name("")
{
	m_tree_view = new QTreeWidget(parent);
	m_element_view = new QTableWidget(parent);

	resizeWidgets(this->geometry());

	connect(m_tree_view, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(treeItemChanges(QTreeWidgetItem*, QTreeWidgetItem*)));
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
}

const QString & ResourceTreeWidget::tree() const
{
	return m_tree_name;
}

void ResourceTreeWidget::update()
{
	this->QWidget::update();	
}

void	ResourceTreeWidget::treeItemChanges(
	QTreeWidgetItem * current, 
	QTreeWidgetItem * previous
)
{
	QMessageBox::warning(NULL, "1", "2");
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