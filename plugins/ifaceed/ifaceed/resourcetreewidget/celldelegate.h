/*!  \file celldelegate.h
	 \author freakyblast
 */
#ifndef CELLDELEGATE_H
#define CELLDELEGATE_H

#include <QtCore>
#include <QtGui>
#include "../../core/spritedatabase.h"
#include <QFontMetrics>

class CellDelegate: public QItemDelegate
{
	/** Paints a cell
		\param[in] painter painter
		\param[in] option  options for rendering
		\param[in] index   data to draw
	 */
	void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	/** Returns a hints for size
		\param[in] option  options for drawing
		\param[in] index   index of model
		\return size hint
     */
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
	
};
/** Returns half string
	\param[in] str source string
	\return half string
 */
QString halfString(QString str);
/** Returns half string. String may ends with "..."
	\param[in] str source string
	\return half string
 */
QString halfStringWith3Dots(QString str);
/** Returns acceptable string for cell geometry
	\param[in] in_group group number (row)
	\param[in] in_index index number (column)
	\param[in] in_width width of the target cell
	\param[in] metrics  metrics data
	\return acceptable for current cell geometry string
 */
QString getAcceptableString(QString in_group, QString in_index,  int in_width, QFontMetrics & metrics);

#endif