#include "layoutcelledit.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QHBoxLayout>

#ifndef HAVE_QT5
    #define HAVE_QT5 (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#endif

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
    // 5. Horizontal alignment
    // 6. Stacking type
    // 7. top padding
    // 8. bottom padding
    // 9. left padding
    // 10. right padding
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
    m_checked->setMaximumWidth(200);
    this->setSpan(0, 1, 1, 2);
    this->setCellWidget(0, 1, m_checked);

    // Second row
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Width: ");

    this->setItem(1, 0, item);

    m_width_value = new QDoubleSpinBox();
    m_width_value->setMinimum(0.0);
    m_width_value->setMaximum(10000.0);
    m_width_value->setSingleStep(1.0);
    m_width_value->setEnabled(false);
    this->setCellWidget(1,1, m_width_value);

    m_width_unit = new QComboBox();
    m_width_unit->addItem("auto");
    m_width_unit->addItem("px");
    m_width_unit->addItem("%");
    m_width_unit->setMaximumWidth(50);
    this->setCellWidget(1,2, m_width_unit);

    // Third row
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Height: ");

    this->setItem(2, 0, item);

    m_height_value = new QDoubleSpinBox();
    m_height_value->setMinimum(0.0);
    m_height_value->setMaximum(10000.0);
    m_height_value->setSingleStep(1.0);
    m_height_value->setEnabled(false);
    this->setCellWidget(2,1, m_height_value);

    m_height_unit = new QComboBox();
    m_height_unit->addItem("auto");
    m_height_unit->addItem("px");
    m_height_unit->addItem("%");
    m_height_unit->setCurrentIndex(0);
    this->setCellWidget(2,2, m_height_unit);

    // Fourth row
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Vertical Alignment: ");
    this->setItem(3, 0, item);

    m_vertical_alignment = new QComboBox();
    m_vertical_alignment->addItem("Top");
    m_vertical_alignment->addItem("Middle");
    m_vertical_alignment->addItem("Bottom");
    m_vertical_alignment->setCurrentIndex(0);

    this->setSpan(3, 1, 1, 2);
    this->setCellWidget(3, 1, m_vertical_alignment);

    //Fifth row
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Horizontal Alignment: ");
    this->setItem(4, 0, item);

    m_horizontal_alignment = new QComboBox();
    m_horizontal_alignment->addItem("Left");
    m_horizontal_alignment->addItem("Middle");
    m_horizontal_alignment->addItem("Right");
    m_horizontal_alignment->setCurrentIndex(0);

    this->setSpan(4, 1, 1, 2);
    this->setCellWidget(4, 1, m_horizontal_alignment);

    //Sixth row
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Stacking type: ");
    this->setItem(5, 0, item);

    m_stacking_type = new QComboBox();
    m_stacking_type->addItem("Horizontal");
    m_stacking_type->addItem("Vertical");
    m_stacking_type->setCurrentIndex(0);

    this->setSpan(5, 1, 1, 2);
    this->setCellWidget(5, 1, m_stacking_type);

    const char* pitems[4] = {"Top padding: ", "Bottom padding:", "Left padding:", "Right padding:"};
    QDoubleSpinBox** dsbs[4] = {&m_padding_top, &m_padding_bottom, &m_padding_left, &m_padding_right };
    for(size_t i = 0; i < 4; i++)
    {
        item = new QTableWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
        item->setText(pitems[i]);
        this->setItem(6 + i, 0, item);

        (*(dsbs[i])) = new QDoubleSpinBox();
        (*(dsbs[i]))->setMinimum(0.0);
        (*(dsbs[i]))->setMaximum(10000.0);
        (*(dsbs[i]))->setSingleStep(1.0);

        this->setSpan(6+i, 1, 1, 2);
        this->setCellWidget(6+i, 1, (*(dsbs[i])));
    }

    // 11. list of children
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Children:");
    this->setItem(10, 0, item);

    this->setSpan(10, 1, 1, 2);
    m_children = new QComboBox();
    this->setCellWidget(10, 1, m_children);

    // 12. move back/front
    m_move_front = new QPushButton();
    m_move_front->setText("Move front");
    this->setCellWidget(11, 0, m_move_front);

    m_move_back = new QPushButton();
    m_move_back->setText("Move back");
    m_move_back->setMaximumWidth(100);
    QVBoxLayout* movebacklt = new QVBoxLayout();
    movebacklt->setMargin(0);
    movebacklt->setAlignment(Qt::AlignRight);
    movebacklt->addWidget(m_move_back);
    QWidget* movebackwidget = new QWidget();
    movebackwidget->setLayout(movebacklt);
    this->setSpan(11, 1, 1, 2);
    this->setCellWidget(11, 1, movebackwidget);

    //13. add/remove child
    m_add = new QPushButton();
    m_add->setText("Add");
    this->setCellWidget(12, 0, m_add);

    m_remove = new QPushButton();
    m_remove->setText("Remove");
    m_remove->setMaximumWidth(100);
    QVBoxLayout* removelt = new QVBoxLayout();
    removelt->setMargin(0);
    removelt->setAlignment(Qt::AlignRight);
    removelt->addWidget(m_remove);
    QWidget* removewidget = new QWidget();
    removewidget->setLayout(removelt);
    this->setSpan(12, 1, 1, 2);
    this->setCellWidget(12, 1, removewidget);

    this->resizeColumnsToContents();
    this->resizeRowsToContents();
    /*this->setMinimumSize(
        this->horizontalHeader()->length() + 5,
        this->verticalHeader()->length() + 5
    );*/
}

QSize gui::layouts::LayoutCellEdit::sizeHint() const
{
    return QSize(
        this->horizontalHeader()->length() + 200,
        this->verticalHeader()->length() + 200
    );
}

gui::layouts::LayoutCellEdit::~LayoutCellEdit()
{

}

