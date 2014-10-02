/*!  \file celldelegate.h
	 \author HiddenSeeker
 */
#ifndef CELLDELEGATE_H
#define CELLDELEGATE_H

#include <QtCore>
#include <QtGui>
#include <QFontMetrics>



namespace gui
{

namespace resourcetreewidget
{

class ResourceTreeWidget;

/*! A delegate for rendering items in resource treewidget
 */
class CellDelegate: public QItemDelegate
{
public:
	inline CellDelegate() : QItemDelegate(), m_widget(NULL)
	{

	}
	/*! Set new widget for resources
		\param[in] w widget resources
	 */
	inline void setResourceTreeWidget(ResourceTreeWidget * w)
	{
		m_widget = w;
	}
	/*! Paints a cell
		\param[in] painter painter
		\param[in] option  options for rendering
		\param[in] index   data to draw
	 */
	void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	/*! Returns a hints for size
		\param[in] option  options for drawing
		\param[in] index   index of model
		\return size hint
     */
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
	/*! A widget, where delegate is belong
	 */
	gui::resourcetreewidget::ResourceTreeWidget * m_widget;
};

/** Returns acceptable string for cell geometry
	\param[in] string   source string
	\param[in] in_width width of the target cell
	\param[in] metrics  metrics data
	\return acceptable for current cell geometry string
 */
QString getAcceptableString(
	const QString& string,
	int in_width, 
	QFontMetrics & metrics
);

}

}

#endif