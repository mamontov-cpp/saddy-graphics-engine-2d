#include "layoutcelledit.h"
#include <QHeaderView>
#include <QTableWidgetItem>

#include <layouts/cell.h>

gui::layouts::LayoutCellEdit::LayoutCellEdit(QWidget* parent)
: QTableWidget(parent), Row(0), Col(0)
{
    this->horizontalHeader()->hide();
    this->verticalHeader()->hide();
    // We need 13 rows
    // 1. Selected
    // 2. Width
    // 3. Height
    // 4. Vertical alignment
    // 5. Horizontal alignmetn
    // 6. Stacking type
    // 7. top padding
    // 8. bottom padding
    // 9. left padding
    // 10. padding
    // 11. list of children
    // 12. Move back/move front
    // 13. Add/remove
    this->setRowCount(13);
    // We need three columns:
    // 1. Label
    // 2. Widget
    // 3. Additional widget for width and height
    this->setColumnCount(3);

    // First row
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Merge/Split: ");

    this->setItem(0, 0, item);

    m_checked = new QCheckBox();
    m_checked->setText("Selected");
    this->setSpan(0, 1, 1, 2);
    this->setCellWidget(0, 1, m_checked);

    //this->setRowHeight(0, item->sizeHint().height());
}

gui::layouts::LayoutCellEdit::~LayoutCellEdit()
{

}

