#include "gui/spritewidget/qspritetablewidget.h"
#include "celldelegate.h"
#include "resourcetreewidget/cell.h"
#include "resourcetreewidget/resourcetreewidget.h"
#include "resourcetreewidget/resourcecache.h"

#include <QStyleOptionViewItem>
#include <QImage>
#include <unused.h>

void CellDelegate::paint(QPainter * painter, 
						 const QStyleOptionViewItem & option, 
						 const QModelIndex & index ) const
{
	if (this->parent() == NULL)
	{
		return;
	}

	ResourceTreeWidget * widget = (ResourceTreeWidget*)(this->parent());
	QString resourcetext = index.data(Qt::DisplayRole).value<QString>();
	sad::Maybe<sad::String> resourcepath = widget->pathToItemBySelection(resourcetext);

	if (resourcepath.exists() == false || resourcetext.length() == 0)
		return;
	
	if (option.state & QStyle::State_Selected) {
        painter->save();
        painter->setBrush(option.palette.highlight());
        painter->setPen(Qt::NoPen);
        painter->drawRect(option.rect);
        painter->setPen(QPen(option.palette.highlightedText(), 0));
		painter->restore();
    }
	// Draw centered image
	const QImage & img = widget->cache()->imageForResource(resourcepath.value().data());
	painter->drawImage(option.rect.x() + option.rect.width()/2 - img.width()/2,
					   option.rect.y(),img);

	
	QFont font = QFont();
	font.setPixelSize(resourcetreewidget::Cell::FontSize);
	QFont oldFont = painter->font();
	painter->setFont(font);
	QFontMetrics fontMetrics(font);
	//Compute label
	QString label = getAcceptableString(resourcetext, option.rect.width(), fontMetrics);
	//Compute center
	int labelpointx = option.rect.x() 
					+ option.rect.width() / 2 
					- fontMetrics.width(label) / 2;
	int labelpointy = option.rect.y() 
				   + resourcetreewidget::Cell::ImageHeight
				   + resourcetreewidget::Cell::SpaceBetweenImageAndText;
	QPoint center = QPoint(labelpointx, labelpointy);
	painter->drawText(center, label);

	painter->setFont(oldFont);

	
}

QSize CellDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
	return QSize(resourcetreewidget::Cell::Width, resourcetreewidget::Cell::Height);
}

QString halfString(const QString & str)
{
	QString res;

	int sz = str.size();
	if (sz > 1)
	{
		res = str.left(sz/2);
	}
	return res;
}

QString halfStringWith3Dots(const QString & str)
{
	QString res;

	res = str;
	if (res.endsWith("..."))
	{
		res.truncate(3);
	}
	res = halfString(res);
	res+="...";

	return res;
}

QString getAcceptableString(
	const QString& string, 
	int in_width, 
	QFontMetrics & metrics
)
{
	
	QString part1 = string.mid(0, string.length() / 2);
	QString part2 = string.mid(string.length() / 2);
	QString str = QString("%1%2").arg(part1).arg(part2);
	int w = metrics.width(str);
	bool cutted_totally = false;
	while(!cutted_totally && (w > in_width))
	{
		QFont font;
		font.setPixelSize(10);
		QFontMetrics fm(font);
        //int needWidth = fm.width(str);
		if (part1.size() > part2.size() || part1 == "...")
		{
			part1 = halfStringWith3Dots(part1);
		}
		else
		{
			part2 = halfStringWith3Dots(part2);
		}
		str = QString("%1%2").arg(part1).arg(part2);
		w = metrics.width(str);
		cutted_totally = (part1=="..." && part2=="...") ;
	}

	return str;
}
