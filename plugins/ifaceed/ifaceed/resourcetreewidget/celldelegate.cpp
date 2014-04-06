#include "celldelegate.h"
#include <QStyleOptionViewItem>
#include <QImage>
#include <unused.h>

#define CELL_WIDTH (100)
#define CELL_HEIGHT (100)

#define MAX_IMAGE_WIDTH  (CELL_WIDTH)
#define MAX_IMAGE_HEIGHT (CELL_HEIGHT/4*3)

#define IMAGE_SPACE  (15)
#define CELL_TEXT_Y (MAX_IMAGE_HEIGHT + IMAGE_SPACE)

#define CELL_FONT_SIZE (12)


/** Paints a cell
	\param[in] painter painter
	\param[in] option  options for rendering
	\param[in] index   data to draw
 */
void CellDelegate::paint(QPainter * painter, 
						 const QStyleOptionViewItem & option, 
						 const QModelIndex & index ) const
{
	CellInfo info = index.data(Qt::UserRole).value<CellInfo>();
	if (info.group.size() == 0 && info.config.size() == 0)
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
	QImage img = info.image;
	if (img.width() > CELL_WIDTH || img.height() > MAX_IMAGE_HEIGHT) 
	{	
		img = img.scaled(QSize(CELL_WIDTH, MAX_IMAGE_HEIGHT), Qt::KeepAspectRatio);
	}
	painter->drawImage(option.rect.x() + option.rect.width()/2 - img.width()/2,
					   option.rect.y(),img);

	QString strGroup = info.group;
	QString strIndex = QString::number(info.index);

	
	QFont font = QFont();
	font.setPixelSize(CELL_FONT_SIZE);
	QFont oldFont = painter->font();
	painter->setFont(font);
	QFontMetrics fontMetrics(font);
	//Compute label
	QString label = getAcceptableString(strGroup, strIndex, option.rect.width(), fontMetrics);
	//Compute center
	QPoint center = QPoint(option.rect.x() + option.rect.width()/2 - fontMetrics.width(label)/2,
						   option.rect.y() + CELL_TEXT_Y);
	painter->drawText(center, label);

	painter->setFont(oldFont);

	
}

/** Returns a hints for size
	\param[in] option  options for drawing
	\param[in] index   index of model
	\return size hint
 */
QSize CellDelegate::sizeHint(UNUSED const QStyleOptionViewItem &option, UNUSED const QModelIndex &index) const
{
	return QSize(CELL_WIDTH,CELL_HEIGHT);
}
/** Returns half string
	\param[in] str source string
	\return half string
 */
QString halfString(QString str)
{
	QString res;

	int sz = str.size();
	if (sz > 1)
	{
		res = str.left(sz/2);
	}
	return res;
}
/** Returns half string. String may ends with "..."
	\param[in] str source string
	\return half string
 */
QString halfStringWith3Dots(QString str)
{
	QString res;

	if (str.endsWith("..."))
	{
		str.truncate(3);
	}
	res = halfString(str);
	res+="...";

	return res;
}

/** Returns acceptable string for cell geometry
	\param[in] in_group group number (row)
	\param[in] in_index index number (column)
	\param[in] in_width width of the target cell
	\return acceptable for current cell geometry string
 */
QString getAcceptableString(QString in_group, QString in_index, int in_width, QFontMetrics & metrics)
{
	QString group = in_group;
	QString index = in_index;
	QString str = QString("%1, %2").arg(group).arg(index);
	int w = metrics.width(str);
	bool cutted_totally = false;
	while(!cutted_totally && (w > in_width))
	{
		QFont font;
		font.setPixelSize(10);
		QFontMetrics fm(font);
        //int needWidth = fm.width(str);
		if (group.size() > index.size() || index == "...")
		{
			group = halfStringWith3Dots(group);
		}
		else
		{
			index = halfStringWith3Dots(index);
		}
		str = QString("%1, %2").arg(group).arg(index);
		w = metrics.width(str);
		cutted_totally = (group=="..." && index=="...") ;
	}

	return str;
}
