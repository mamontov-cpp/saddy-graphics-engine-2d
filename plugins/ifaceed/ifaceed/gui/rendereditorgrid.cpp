#include "rendereditorgrid.h"

// ========================= PUBLIC METHODS =========================

gui::RenderEditorGrid::RenderEditorGrid() : Color(0, 255, 255), GridSpaceX(0), GridSpaceY(0)
{
    this->Step::disable();
}

gui::RenderEditorGrid::~RenderEditorGrid()
{
    
}

bool gui::RenderEditorGrid::isEnabled() const
{
    return m_enabled;
}

// ========================= PROTECTED METHODS =========================

void gui::RenderEditorGrid::_process()
{
    
}
