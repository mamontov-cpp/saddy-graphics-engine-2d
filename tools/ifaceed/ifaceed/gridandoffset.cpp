#include "gridandoffset.h"

#include "gui/mainpanelproxy.h"
#include "gui/rendereditorgrid.h"

#include "core/editor.h"

#include "core/typeconverters/sadcolortoqcolor.h"
#include "core/typeconverters/qcolortosadacolor.h"

#include <maybe.h>

#include <renderer.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/dbdatabase.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/load.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/dbvariant.h>


// =============================== PUBLIC METHODS ===============================

GridAndOffsets::GridAndOffsets(QWidget* parent) : QWidget(parent)
{
    m_ui.setupUi(this);
    m_ui.clrGridColor->setBackgroundColor(QColor(0, 255, 255));

    connect(m_ui.dsbOffsetX, SIGNAL(valueChanged(double)), this, SLOT(offsetXChanged(double)));
    connect(m_ui.dsbOffsetY, SIGNAL(valueChanged(double)), this, SLOT(offsetYChanged(double)));
    connect(m_ui.cbGridShow, SIGNAL(clicked(bool)), this, SLOT(enabledCheckboxChanged(bool)));
    connect(m_ui.spbGridX, SIGNAL(valueChanged(int)), this, SLOT(gridXChanged(int)));
    connect(m_ui.spbGridY, SIGNAL(valueChanged(int)), this, SLOT(gridYChanged(int)));
    connect(m_ui.clrGridColor, SIGNAL(clicked()), this, SLOT(changeGridColorClicked()));

    connect(this, SIGNAL(globalOffsetChanged(QPointF)), this, SLOT(handleChangeOffset(QPointF)));
    connect(this, SIGNAL(gridEnabledChanged(bool)), this, SLOT(handleChangeGridEnabled(bool)));
    connect(this, SIGNAL(gridSettingsChanged(QPointF)), this, SLOT(handleChangeGridSettings(QPointF)));
    connect(this, SIGNAL(gridColorChanged(sad::AColor)), this, SLOT(handleChangeGridColor(sad::AColor)));
}


void GridAndOffsets::updateGlobalOffsetInUI(double x, double y) const
{
    m_ui.dsbOffsetX->setValue(x);
    m_ui.dsbOffsetY->setValue(y);
}

void GridAndOffsets::updateGlobalOffsetInUI(const sad::Point3D& p) const
{
    this->updateGlobalOffsetInUI(p.x(), p.y());
}

sad::Point3D GridAndOffsets::globalOffset() const
{
    return sad::Point3D(m_ui.dsbOffsetX->value(), m_ui.dsbOffsetY->value(), 0);
}

void GridAndOffsets::updateGridEnabled(bool enabled) const
{
    m_ui.cbGridShow->setCheckState((enabled) ? Qt::Checked : Qt::Unchecked);
}

bool GridAndOffsets::gridEnabled() const
{
    return m_ui.cbGridShow->checkState() == Qt::Checked;
}

void GridAndOffsets::updateGridSettings(int x, int y) const
{
    m_ui.spbGridX->setValue(x);
    m_ui.spbGridY->setValue(y);    
}

void GridAndOffsets::updateGridSettings(const sad::Point2I& p) const
{
    this->updateGridSettings(p.x(), p.y());
}

sad::Point2I GridAndOffsets::gridSettings() const
{
    return sad::Point2I(m_ui.spbGridX->value(), m_ui.spbGridY->value());
}

GridAndOffsets::~GridAndOffsets()
{
    
}

void GridAndOffsets::setProxy(gui::MainPanelProxy* proxy)
{
    m_proxy = proxy;
}


sad::AColor GridAndOffsets::gridColor() const
{
    sad::AColor clr;
    core::typeconverters::QColorToSadAColor::convert(m_ui.clrGridColor->backgroundColor(), clr);
    return clr;
}

void GridAndOffsets::setGridColor(const sad::AColor& clr) const
{
    QColor result;
    sad::Color color(clr.r(), clr.g(), clr.b());
    core::typeconverters::SadColorToQColor cvt; 
    cvt.convert(&color, &result);
    m_ui.clrGridColor->setBackgroundColor(result);
}


sad::db::Database* GridAndOffsets::database()
{
    return sad::Renderer::ref()->database("");
}

// =============================== PUBLIC SLOT METHODS ===============================

void GridAndOffsets::offsetXChanged(double new_value)
{
    emit globalOffsetChanged(QPointF(new_value, m_ui.dsbOffsetY->value()));
}

void GridAndOffsets::offsetYChanged(double new_value)
{
    emit globalOffsetChanged(QPointF(m_ui.dsbOffsetX->value(), new_value));
}

void GridAndOffsets::enabledCheckboxChanged(bool new_value)
{
    emit gridEnabledChanged(new_value);    
}

void GridAndOffsets::gridXChanged(int new_value)
{
    emit gridSettingsChanged(QPointF(new_value, m_ui.spbGridY->value()));
}

void GridAndOffsets::gridYChanged(int new_value)
{
    emit gridSettingsChanged(QPointF(m_ui.spbGridX->value(), new_value));
}

void GridAndOffsets::changeGridColorClicked()
{
    sad::Maybe<QColor> clr = m_proxy->trySelectColor(m_ui.clrGridColor->backgroundColor());
    if (clr.exists())
    {
        m_ui.clrGridColor->setBackgroundColor(clr.value());
        sad::AColor aclr;
        core::typeconverters::QColorToSadAColor::convert(m_ui.clrGridColor->backgroundColor(), aclr);
        emit gridColorChanged(aclr);
    }
}


void GridAndOffsets::handleChangeOffset(QPointF p)
{
    sad::Point3D pnt(p.x(), p.y(), 0);
    GridAndOffsets::database()->setProperty("global_renderer_offset", pnt);
    sad::Renderer::ref()->setGlobalTranslationOffset(pnt);
}

void GridAndOffsets::handleChangeGridEnabled(bool v)
{
    GridAndOffsets::database()->setProperty("global_renderer_grid_enabled", v);
    gui::RenderEditorGrid* regrid = m_proxy->editor()->renderEditorGrid();
    if (v)
    {
        regrid->enable();
    }
    else
    {
        regrid->disable();
    }
}

void GridAndOffsets::handleChangeGridSettings(QPointF p)
{
    sad::Point2I pnt(p.x(), p.y());
    GridAndOffsets::database()->setProperty("global_renderer_grid_settings", pnt);
    gui::RenderEditorGrid* regrid = m_proxy->editor()->renderEditorGrid();
    regrid->GridSpaceX = pnt.x();
    regrid->GridSpaceY = pnt.y();
}

void GridAndOffsets::handleChangeGridColor(sad::AColor clr)
{
    GridAndOffsets::database()->setProperty("global_renderer_grid_color", clr);
    gui::RenderEditorGrid* regrid = m_proxy->editor()->renderEditorGrid();
    regrid->Color = clr;
}
