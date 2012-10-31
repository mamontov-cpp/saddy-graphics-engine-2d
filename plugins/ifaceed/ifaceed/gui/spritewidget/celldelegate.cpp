#include "celldelegate.h"
#include <QStyleOptionViewItem>
#include <QImage>

#define CELL_WIDTH (100)
#define CELL_HEIGHT (100)

#define MAX_IMAGE_WIDTH  (66)
#define MAX_IMAGE_HEIGHT (66)

#define IMAGE_SPACE  (5)
#define CELL_TEXT_Y (MAX_IMAGE_HEIGHT + IMAGE_SPACE)
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

	QString str = getAcceptableString(strGroup, strIndex, option.rect.width());
	
	QFont font = QFont();
	font.setPixelSize(10);
	QFont oldFont = painter->font();
	painter->setFont(font);

	painter->setFont(oldFont);
	//emit static_cast<const QAbstractItemDelegate*>(this)->sizeHintChanged(index);
	

	
	//QPen pen(QColor(255,255,255));
	//painter->setPen(pen);
	
	QFontMetrics fontMetrics(font);
	//QRect rectText = QRect(QPoint(), fontMetrics.size(Qt::TextSingleLine, str) );
	QPoint center = QPoint(img.width() - fontMetrics.width(str),
							101);
	painter->drawText(center, str);
	//painter->drawText(1, 101, str);
}

/** Returns a hints for size
	\param[in] option  options for drawing
	\param[in] index   index of model
	\return size hint
 */
QSize CellDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
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
QString getAcceptableString(QString in_group, QString in_index, int in_width)
{
	QString res;
	QString group = in_group;
	QString index = in_index;
	QString str;
	while(group!="..." && index!="...")
	{
		str = QString("%1, %2").arg(group).arg(index);
		QFont font;
		font.setPixelSize(10);
		QFontMetrics fm(font);
		int needWidth = fm.width(str);

		if (needWidth > in_width)
		{
			if (group.size() > index.size())
			{
				group = halfStringWith3Dots(group);
			}else
			{
				index = halfStringWith3Dots(index);
			}
		}else
		{
			res = str;
			break;
		}
	}
	return res;
}