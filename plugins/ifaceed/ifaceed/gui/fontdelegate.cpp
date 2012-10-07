#include "fontdelegate.h"
#include <QPainter>

void FontDelegate::paint(QPainter * painter, 
						 const QStyleOptionViewItem & option, 
						 const QModelIndex & index ) const
{
	QString text = index.data(Qt::DisplayRole).toString();
	QFont   font = index.data(Qt::UserRole).value<QFont>();
	
	QRect r = option.rect;
	// Draw selected state
	if (option.state & QStyle::State_Selected) {
        painter->save();
        painter->setBrush(option.palette.highlight());
        painter->setPen(Qt::NoPen);
        painter->drawRect(option.rect);
        painter->setPen(QPen(option.palette.highlightedText(), 0));
    }
	
	QFont old = painter->font();
    painter->setFont(font);
    painter->drawText(r, Qt::AlignVCenter|Qt::AlignLeading|Qt::TextSingleLine, text);
	painter->setFont(old);
	
	if (option.state & QStyle::State_Selected)
        painter->restore();
}

QSize FontDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QString text = index.data(Qt::DisplayRole).toString();
    QFont font   = index.data(Qt::UserRole).value<QFont>();
    QFontMetrics fontMetrics(font);
    return QSize(fontMetrics.width(text), fontMetrics.height());
}