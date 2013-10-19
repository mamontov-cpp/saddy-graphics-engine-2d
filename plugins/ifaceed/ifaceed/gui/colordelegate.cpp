#include "colordelegate.h"
#include <QPainter>
#include <QStyleOptionViewItem>

Q_DECLARE_METATYPE(QColor);

/** A width of white frame on rect
 */
#define COLORDELEGATE_WIDTH 3

void ColorDelegate::paint(QPainter * painter, 
						 const QStyleOptionViewItem & option, 
						 const QModelIndex & index ) const
{
	QString text = index.data(Qt::DisplayRole).toString();
	QColor   color = index.data(Qt::UserRole).value<QColor>();
	
	QRect r = option.rect;
	// Draw selected state
	if (option.state & QStyle::State_Selected) {
        painter->save();
        painter->setBrush(option.palette.highlight());
        painter->setPen(Qt::NoPen);
        painter->drawRect(option.rect);
        painter->setPen(QPen(option.palette.highlightedText(), 0));
		painter->restore();
    }
	QBrush brush = painter->brush();
	brush.setColor(color);
	brush.setStyle(Qt::SolidPattern);
	painter->fillRect(r.x() + COLORDELEGATE_WIDTH, 
		              r.y() + COLORDELEGATE_WIDTH, 
					  r.width() - 2 * COLORDELEGATE_WIDTH, 
					  r.height() - 2 * COLORDELEGATE_WIDTH,
					  brush);
      
}

QSize ColorDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QString text = index.data(Qt::DisplayRole).toString();
    QFontMetrics fontMetrics(option.font);
    return QSize(fontMetrics.width(text), fontMetrics.height());
}