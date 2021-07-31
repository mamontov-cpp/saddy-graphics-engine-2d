#include "layoutcelledit.h"
#include <QHeaderView>
// ReSharper disable once CppUnusedIncludeDirective
#include <QTableWidgetItem>
#include <QHBoxLayout>
#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QHash>
#include "../../blockedclosuremethodcall.h"

#include "../core/editor.h"

#ifndef HAVE_QT5
    #define HAVE_QT5 (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#endif

gui::layouts::LayoutCellEdit::LayoutCellEdit(QWidget* parent)
: QTableWidget(parent), Row(0), Col(0), m_children_provider(nullptr)
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
    this->setRowCount(14);
    // We need three columns:
    // 1. Label
    // 2. Widget
    // 3. Additional widget for width and height
    this->setColumnCount(3);

    int currentrow = 0;

    QTableWidgetItem* item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Row/Column: ");
    this->setItem(currentrow, 0, item);

    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText(QString::number(Row) + ";" + QString::number(Col));
    this->setItem(currentrow, 1, item);

    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("");
    this->setItem(currentrow, 2, item);

    
    // First row
    ++currentrow;
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Merge/Split: ");

    this->setItem(currentrow, 0, item);

    m_checked = new QCheckBox();
    m_checked->setText("Selected");
    m_checked->setMaximumWidth(200);
    this->setSpan(currentrow, 1, 1, 2);
    this->setCellWidget(currentrow, 1, m_checked);

    // Second row
    ++currentrow;
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Width: ");

    this->setItem(currentrow, 0, item);

    m_width_value = new QDoubleSpinBox();
    m_width_value->setMinimum(0.0);
    m_width_value->setMaximum(10000.0);
    m_width_value->setSingleStep(1.0);
    m_width_value->setEnabled(false);
    this->setCellWidget(currentrow,1, m_width_value);

    m_width_unit = new QComboBox();
    m_width_unit->addItem("auto");
    m_width_unit->addItem("px");
    m_width_unit->addItem("%");
    m_width_unit->setCurrentIndex(0);
    this->setCellWidget(currentrow,2, m_width_unit);

    // Third row
    ++currentrow;
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Height: ");

    this->setItem(currentrow, 0, item);

    m_height_value = new QDoubleSpinBox();
    m_height_value->setMinimum(0.0);
    m_height_value->setMaximum(10000.0);
    m_height_value->setSingleStep(1.0);
    m_height_value->setEnabled(false);
    this->setCellWidget(currentrow,1, m_height_value);

    m_height_unit = new QComboBox();
    m_height_unit->addItem("auto");
    m_height_unit->addItem("px");
    m_height_unit->addItem("%");
    m_height_unit->setCurrentIndex(0);
    this->setCellWidget(currentrow,2, m_height_unit);

    // Fourth row
    ++currentrow;
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Vertical Alignment: ");
    this->setItem(currentrow, 0, item);

    m_vertical_alignment = new QComboBox();
    m_vertical_alignment->addItem("Top");
    m_vertical_alignment->addItem("Middle");
    m_vertical_alignment->addItem("Bottom");
    m_vertical_alignment->setCurrentIndex(0);

    this->setSpan(currentrow, 1, 1, 2);
    this->setCellWidget(currentrow, 1, m_vertical_alignment);

    //Fifth row
    ++currentrow;
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Horizontal Alignment: ");
    this->setItem(currentrow, 0, item);

    m_horizontal_alignment = new QComboBox();
    m_horizontal_alignment->addItem("Left");
    m_horizontal_alignment->addItem("Middle");
    m_horizontal_alignment->addItem("Right");
    m_horizontal_alignment->setCurrentIndex(0);

    this->setSpan(currentrow, 1, 1, 2);
    this->setCellWidget(currentrow, 1, m_horizontal_alignment);

    //Sixth row
    ++currentrow;
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Stacking type: ");
    this->setItem(currentrow, 0, item);

    m_stacking_type = new QComboBox();
    m_stacking_type->addItem("Horizontal");
    m_stacking_type->addItem("Vertical");
    m_stacking_type->addItem("No Stacking");
    m_stacking_type->setCurrentIndex(0);

    this->setSpan(currentrow, 1, 1, 2);
    this->setCellWidget(currentrow, 1, m_stacking_type);

    const char* pitems[4] = {"Top padding: ", "Bottom padding:", "Left padding:", "Right padding:"};
    QDoubleSpinBox** dsbs[4] = {&m_padding_top, &m_padding_bottom, &m_padding_left, &m_padding_right };
    for(size_t i = 0; i < 4; i++)
    {
        ++currentrow;
        item = new QTableWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
        item->setText(pitems[i]);
        this->setItem(7 + i, 0, item);

        (*(dsbs[i])) = new QDoubleSpinBox();
        (*(dsbs[i]))->setMinimum(0.0);
        (*(dsbs[i]))->setMaximum(10000.0);
        (*(dsbs[i]))->setSingleStep(1.0);

        this->setSpan(currentrow, 1, 1, 2);
        this->setCellWidget(currentrow, 1, (*(dsbs[i])));
    }

    // 11. list of children
    ++currentrow;
    item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText("Children:");
    this->setItem(currentrow, 0, item);

    this->setSpan(currentrow, 1, 1, 2);
    m_children = new QComboBox();
    this->setCellWidget(currentrow, 1, m_children);

    // 12. move back/front
    ++currentrow;
    m_move_front = new QPushButton();
    m_move_front->setText("Move front");
    this->setCellWidget(currentrow, 0, m_move_front);

    m_move_back = new QPushButton();
    m_move_back->setText("Move back");
    m_move_back->setMaximumWidth(100);
    QVBoxLayout* movebacklt = new QVBoxLayout();
    movebacklt->setMargin(0);
    movebacklt->setAlignment(Qt::AlignRight);
    movebacklt->addWidget(m_move_back);
    QWidget* movebackwidget = new QWidget();
    movebackwidget->setLayout(movebacklt);
    this->setSpan(currentrow, 1, 1, 2);
    this->setCellWidget(currentrow, 1, movebackwidget);

    //13. add/remove child
    ++currentrow;
    m_add = new QPushButton();
    m_add->setText("Add");
    this->setCellWidget(currentrow, 0, m_add);

    m_remove = new QPushButton();
    m_remove->setText("Remove");
    this->setCellWidget(currentrow, 1, m_remove);


    m_clear = new QPushButton();
    m_clear->setText("Clear");
    this->setCellWidget(currentrow, 2, m_clear);


    this->resizeColumnsToContents();
    this->resizeRowsToContents();
    this->setMinimumSize(this->gui::layouts::LayoutCellEdit::sizeHint());
    
    connect(m_width_value, SIGNAL(valueChanged(double)), this, SLOT(widthValueChanged(double)));
    connect(m_width_unit, SIGNAL(currentIndexChanged(int)), this, SLOT(widthUnitChanged(int)));	
    connect(m_height_value, SIGNAL(valueChanged(double)), this, SLOT(heightValueChanged(double)));
    connect(m_height_unit, SIGNAL(currentIndexChanged(int)), this, SLOT(heightUnitChanged(int)));	
    connect(m_horizontal_alignment, SIGNAL(currentIndexChanged(int)), this, SLOT(horizontalAlignmentValueChanged(int)));	
    connect(m_vertical_alignment, SIGNAL(currentIndexChanged(int)), this, SLOT(verticalAlignmentValueChanged(int)));	
    connect(m_stacking_type, SIGNAL(currentIndexChanged(int)), this, SLOT(stackingTypeValueChanged(int)));	
    connect(m_padding_top, SIGNAL(valueChanged(double)), this, SLOT(topPaddingValueChanged(double)));
    connect(m_padding_bottom, SIGNAL(valueChanged(double)), this, SLOT(bottomPaddingValueChanged(double)));
    connect(m_padding_left, SIGNAL(valueChanged(double)), this, SLOT(leftPaddingValueChanged(double)));
    connect(m_padding_right, SIGNAL(valueChanged(double)), this, SLOT(rightPaddingValueChanged(double)));
    connect(m_add, SIGNAL(clicked()), this, SLOT(addChildClicked()));
    connect(m_remove, SIGNAL(clicked()), this, SLOT(removeChildClicked()));
    connect(m_clear, SIGNAL(clicked()), this, SLOT(clearClicked()));
    connect(m_move_back, SIGNAL(clicked()), this, SLOT(moveBackClicked()));
    connect(m_move_front, SIGNAL(clicked()), this, SLOT(moveFrontClicked()));
}

gui::layouts::LayoutCellEdit::~LayoutCellEdit()
{

}

void gui::layouts::LayoutCellEdit::setRowAndColumn(size_t row, size_t col)
{
    this->Row = row;
    this->Col = col;
    QTableWidgetItem*  item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    item->setText(QString::number(Row) + ";" + QString::number(Col));
    this->setItem(0, 1, item);
}

QSize gui::layouts::LayoutCellEdit::sizeHint() const
{
    return QSize(
        this->horizontalHeader()->length() + 10, // 10 is a small bound
        this->verticalHeader()->length() + 10
    );
}

bool gui::layouts::LayoutCellEdit::checked() const
{
    return m_checked->checkState() == Qt::Checked;
}

void gui::layouts::LayoutCellEdit::set(sad::layouts::Cell* cell)
{
    this->setRowAndColumn(cell->Row, cell->Col);

    this->setWidth(cell->width());
    this->setHeight(cell->height());
    this->setHorizontalAlignment(cell->horizontalAlignment());
    this->setVerticalAlignment(cell->verticalAlignment());
    this->setStackingType(cell->stackingType());

    this->setPaddingTop(cell->paddingTop());
    this->setPaddingBottom(cell->paddingBottom());
    this->setPaddingLeft(cell->paddingLeft());
    this->setPaddingRight(cell->paddingRight());
    
    invoke_blocked(m_checked, &QCheckBox::setCheckState, Qt::Unchecked);

    m_children->clear();
    for(size_t i = 0; i < cell->childrenCount(); i++)
    {
        sad::SceneNode* node = cell->child(i);
        addChild(node);
    }
}

void gui::layouts::LayoutCellEdit::setWidth(const sad::layouts::LengthValue& v) const
{
    invoke_blocked(m_width_value, &QDoubleSpinBox::setValue, v.Value);
    invoke_blocked(m_width_unit,  &QComboBox::setCurrentIndex, static_cast<int>(v.Unit));
    m_width_value->setEnabled(v.Unit != sad::layouts::Unit::LU_Auto);
}

void gui::layouts::LayoutCellEdit::setHeight(const sad::layouts::LengthValue& v) const
{
    invoke_blocked(m_height_value, &QDoubleSpinBox::setValue, v.Value);
    invoke_blocked(m_height_unit,  &QComboBox::setCurrentIndex, static_cast<int>(v.Unit));   
    m_height_value->setEnabled(v.Unit != sad::layouts::Unit::LU_Auto);
}

void gui::layouts::LayoutCellEdit::setHorizontalAlignment(sad::layouts::HorizontalAlignment v) const
{
    invoke_blocked(m_horizontal_alignment,  &QComboBox::setCurrentIndex, static_cast<int>(v));    
}

void gui::layouts::LayoutCellEdit::setVerticalAlignment(sad::layouts::VerticalAlignment v) const
{
    invoke_blocked(m_vertical_alignment,  &QComboBox::setCurrentIndex, static_cast<int>(v));  
}

void gui::layouts::LayoutCellEdit::setStackingType(sad::layouts::StackingType v) const
{
    invoke_blocked(m_stacking_type,  &QComboBox::setCurrentIndex, static_cast<int>(v));    
}

void gui::layouts::LayoutCellEdit::setPaddingTop(double v) const
{
    invoke_blocked(m_padding_top, &QDoubleSpinBox::setValue, v);    
}

void gui::layouts::LayoutCellEdit::setPaddingBottom(double v) const
{
    invoke_blocked(m_padding_bottom, &QDoubleSpinBox::setValue, v);    
}

void gui::layouts::LayoutCellEdit::setPaddingLeft(double v) const
{
    invoke_blocked(m_padding_left, &QDoubleSpinBox::setValue, v);    
}

void gui::layouts::LayoutCellEdit::setPaddingRight(double v) const
{
    invoke_blocked(m_padding_right, &QDoubleSpinBox::setValue, v);    
}

void gui::layouts::LayoutCellEdit::removeChild(size_t child) const
{
    if (child < m_children->count()) 
    {
        invoke_blocked(m_children, &QComboBox::removeItem, child);
    }
}

void gui::layouts::LayoutCellEdit::addChild(sad::SceneNode* node) const
{
    QString name;
    unsigned long long id = 0;
    if (node)
    {
        id = node->MajorId;
    }
    if (m_children_provider)
    {
        name = m_children_provider->name(node);
    }
    m_children->addItem(name, QVariant(id));
}

void gui::layouts::LayoutCellEdit::insertChild(sad::SceneNode* node, size_t pos) const
{
    if (pos >= m_children->count())
    {
        addChild(node);
    }
    else
    {
        QString name;
        unsigned long long id = 0;
        if (node)
        {
            id = node->MajorId;
        }
        m_children->insertItem(pos, name, QVariant(id));
    }
}

void gui::layouts::LayoutCellEdit::clearChildren() const
{
    m_children->clear();
}

void gui::layouts::LayoutCellEdit::removeChild(size_t pos)
{
    if (pos <= m_children->count())
    {
        m_children->removeItem(pos);
    }
}

void gui::layouts::LayoutCellEdit::swapChildren(size_t pos1, size_t pos2) const
{
    if (pos1 < m_children->count() && pos2 < m_children->count())
    {
        QVariant cached_variant = m_children->itemData(pos1, Qt::UserRole);
        QString cached_name = m_children->itemText(pos1);

        m_children->setItemData(pos1, m_children->itemData(pos2, Qt::UserRole));
        m_children->setItemText(pos1, m_children->itemText(pos2));

        m_children->setItemData(pos2, cached_variant);
        m_children->setItemText(pos2, cached_name);
    }
}

void gui::layouts::LayoutCellEdit::updateChildName(size_t pos, const QString& name) const
{
    if (pos < m_children->count())
    {
        m_children->setItemText(pos, name);
    }
}

void gui::layouts::LayoutCellEdit::setChildrenProvider(gui::layouts::LayoutCellEdit::ChildrenProvider* p)
{
    delete m_children_provider;
    m_children_provider = p;
}

void gui::layouts::LayoutCellEdit::setChildEditingEnabled(bool enabled)
{
    m_add->setEnabled(enabled);
    m_remove->setEnabled(enabled);
    m_clear->setEnabled(enabled);
    m_move_back->setEnabled(enabled);
    m_move_front->setEnabled(enabled);
}

void gui::layouts::LayoutCellEdit::widthValueChanged(double new_value)
{
    sad::layouts::LengthValue value;
    value.Value  = new_value;
    value.Unit = static_cast<sad::layouts::Unit>(m_width_unit->currentIndex());

    emit widthChanged(Row, Col, value);
}

void gui::layouts::LayoutCellEdit::widthUnitChanged(int unit)
{
    if (unit >  -1) 
    {
        sad::layouts::LengthValue value;
        value.Value  = m_width_value->value();
        value.Unit = static_cast<sad::layouts::Unit>(unit);
        if (value.Unit != sad::layouts::Unit::LU_Auto) {
            m_width_value->setEnabled(true);
        } else {
            m_width_value->setEnabled(false);
        }
        if (value.Unit == sad::layouts::Unit::LU_Percents) {
            if (value.Value > 100) {
                value.Value = 100;
            }
        }

        emit widthChanged(Row, Col, value);	
    }
}

void gui::layouts::LayoutCellEdit::heightValueChanged(double new_value)
{
    sad::layouts::LengthValue value;
    value.Value  = new_value;
    value.Unit = static_cast<sad::layouts::Unit>(m_height_unit->currentIndex());

    emit heightChanged(Row, Col, value);	
}

void gui::layouts::LayoutCellEdit::heightUnitChanged(int unit)
{
    if (unit >  -1) 
    {
        sad::layouts::LengthValue value;
        value.Value  = m_height_value->value();
        value.Unit = static_cast<sad::layouts::Unit>(unit);
        if (value.Unit != sad::layouts::Unit::LU_Auto) {
            m_height_value->setEnabled(true);
        } else {
            m_height_value->setEnabled(false);
        }
        if (value.Unit == sad::layouts::Unit::LU_Percents) {
            if (value.Value > 100) {
                value.Value = 100;
            }
        }

        emit heightChanged(Row, Col, value);	
    }
}

void gui::layouts::LayoutCellEdit::horizontalAlignmentValueChanged(int v)
{
    if (v > -1)
    {
        emit horizontalAlignmentChanged(Row, Col, static_cast<sad::layouts::HorizontalAlignment>(v));
    }
}

void gui::layouts::LayoutCellEdit::verticalAlignmentValueChanged(int v)
{
    if (v > -1)
    {
        emit verticalAlignmentChanged(Row, Col, static_cast<sad::layouts::VerticalAlignment>(v));
    }
}

void gui::layouts::LayoutCellEdit::stackingTypeValueChanged(int v)
{
    if (v > -1)
    {
        emit stackingTypeChanged(Row, Col, static_cast<sad::layouts::StackingType>(v));	
    }
}


void gui::layouts::LayoutCellEdit::topPaddingValueChanged(double new_value)
{
    emit topPaddingChanged(Row, Col, new_value);
}

void gui::layouts::LayoutCellEdit::bottomPaddingValueChanged(double new_value)
{
    emit bottomPaddingChanged(Row, Col, new_value);
}

void gui::layouts::LayoutCellEdit::leftPaddingValueChanged(double new_value)
{
    emit leftPaddingChanged(Row, Col, new_value);
}

void gui::layouts::LayoutCellEdit::rightPaddingValueChanged(double new_value)
{
    emit rightPaddingChanged(Row, Col, new_value);
}


void gui::layouts::LayoutCellEdit::addChildClicked()
{
    QVector<QPair<QString, unsigned long long> > pairs;
    if (m_children_provider)
    {
        pairs = m_children_provider->possibleChildren();
        // Strip all children, that are already in editor
        QHash<unsigned long long, bool> children;
        for(size_t i = 0; i < m_children->count(); i++)
        {
            children.insert(m_children->itemData(i, Qt::UserRole).value<unsigned long long>(), true);
        }
        for(size_t i = 0; i < pairs.size(); i++)
        {
            if (children.contains(pairs[i].second))
            {
                pairs.remove(i);
                --i;
            }
        }
    }
    if (pairs.size() != 0)
    {
        core::Editor::NodePickingDialog dlg = core::Editor::dialogForSelectingNode(
            this,
            "Please, pick a child to add into cell",
            pairs
        );
        if (dlg.Dialog->exec() == QDialog::Accepted)
        {
            int current_item = dlg.NodeList->currentRow();
            if (current_item > -1)
            {
                QString name = dlg.NodeList->item(current_item)->text();
                unsigned long long id = dlg.NodeList->item(current_item)->data(Qt::UserRole).value<unsigned long long>();
                emit childAdded(Row, Col, id, name);
                m_children->addItem(name, QVariant(id));
                m_children->setCurrentIndex(m_children->count() - 1);
            }
        }
        delete dlg.Dialog;
    } 
    else
    {
        QMessageBox::warning(this, "Warning", "No child can be added into cell.\nPlease add at least one scene object, before adding a child");
    }
}

void gui::layouts::LayoutCellEdit::removeChildClicked()
{
    if (m_children->count() > 0 && m_children->currentIndex() > -1)
    {
        size_t index = m_children->currentIndex(); 
        emit childRemoved(Row, Col, index);
        removeChild(index);
    }
}

void gui::layouts::LayoutCellEdit::clearClicked()
{
    if (m_children->count())
    {
        emit cleared(Row, Col);
        clearChildren();
    }
}

void gui::layouts::LayoutCellEdit::moveBackClicked()
{
    if ((m_children->count() > 0) && (m_children->currentIndex() < (m_children->count() - 1)))
    {
        size_t pos1 = m_children->currentIndex();
        size_t pos2 = pos1 + 1;
        emit childrenSwapped(Row, Col, pos1, pos2);
        swapChildren(pos1, pos2);
        m_children->setCurrentIndex(pos2);
    }
    
}

void gui::layouts::LayoutCellEdit::moveFrontClicked()
{
    if ((m_children->count() > 0) && (m_children->currentIndex() > 0))
    {
        size_t pos1 = m_children->currentIndex();
        size_t pos2 = pos1 - 1;
        emit childrenSwapped(Row, Col, pos1, pos2);
        swapChildren(pos1, pos2);
        m_children->setCurrentIndex(pos2);
    }		
}

gui::layouts::LayoutCellEdit::ChildrenProvider::~ChildrenProvider()
{

}

