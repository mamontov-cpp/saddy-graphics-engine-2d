#include "gui/resourcetreewidget/celldelegate.h"
#include "gui/resourcetreewidget/cell.h"
#include "gui/resourcetreewidget/resourcetreewidget.h"
#include "gui/resourcetreewidget/resourcecache.h"

#include "../../qstdstring.h"

#include <QStyleOptionViewItem>
#include <QImage>
#include <unused.h>

void gui::resourcetreewidget::CellDelegate::paint(
    QPainter * painter, 
    const QStyleOptionViewItem & option, 
    const QModelIndex & index 
) const
{
    gui::resourcetreewidget::ResourceTreeWidget * const parent = m_widget;
    if (parent == nullptr)
    {
        return;
    }
    QString resourcetext = index.data(Qt::DisplayRole).value<QString>();
    if (resourcetext.length() == 0)
    {
        return;
    }
    sad::Maybe<sad::String> resourcepath = parent->pathToItemBySelection(resourcetext);

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
    const QImage & img = parent->cache()->imageForResource(STD2QSTRING(resourcepath.value()));
    painter->drawImage(
        option.rect.x() + option.rect.width()/2 - img.width()/2,
        option.rect.y() + gui::resourcetreewidget::Cell::ImageHeight/2 - img.height()/2,
        img
    );

    QFont font = QFont();
    font.setPixelSize(gui::resourcetreewidget::Cell::FontSize);
    QFont oldFont = painter->font();
    painter->setFont(font);
    QFontMetrics fontMetrics(font);
    //Compute label
    QString label = resourcetreewidget::getAcceptableString(
        resourcetext, 
        option.rect.width(), 
        fontMetrics
    );
    //Compute center
    int labelpointx = option.rect.x() 
                    + option.rect.width() / 2 
                    - fontMetrics.width(label) / 2;
    int labelpointy = option.rect.y() 
                   + gui::resourcetreewidget::Cell::ImageHeight
                   + gui::resourcetreewidget::Cell::SpaceBetweenImageAndText;
    QPoint center = QPoint(labelpointx, labelpointy);
    painter->drawText(center, label);

    painter->setFont(oldFont);

    
}

QSize gui::resourcetreewidget::CellDelegate::sizeHint(
    const QStyleOptionViewItem &, 
    const QModelIndex &
) const
{
    return QSize(gui::resourcetreewidget::Cell::Width, 
                 gui::resourcetreewidget::Cell::Height
                );
}


QString gui::resourcetreewidget::getAcceptableString(
    const QString& string, 
    int in_width, 
    QFontMetrics & metrics
)
{
    int lengthpart1 = string.length() / 2;
    int lengthpart2 = lengthpart1;
    int w = metrics.width(string);
    bool cutted_totally = false;
    bool changed = false;
    while(!cutted_totally && (w > in_width))
    {
        changed = true;
        if (lengthpart1 > 3 && lengthpart1 > (string.length () - lengthpart2))
        {
            lengthpart1 -= 3;
        }
        else
        {
            lengthpart2 += 3;
        }
        
        QFont font;
        font.setPixelSize(10);
        QFontMetrics fm(font);
        QString str = string.mid(0, lengthpart1) + "..." + string.mid(lengthpart2);
        w = metrics.width(str);
        cutted_totally = (lengthpart1 <= 0) && (lengthpart2 >= string.length()) ;
    }
    // Resulting string data
    QString result = string;
    if (changed)
    {
        result = string.mid(0, lengthpart1) + "..." + string.mid(lengthpart2);
    }
    return result;
}
