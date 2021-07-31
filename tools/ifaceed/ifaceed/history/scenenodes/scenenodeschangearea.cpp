#include "scenenodeschangearea.h"

#include "../../core/editor.h"

#include "../../core/typeconverters/sadrect2dtoqrectf.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QRectF>

#include "../../blockedclosuremethodcall.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiscenenodeblock.h"

#include "../../gui/rectwidget/rectwidget.h"

history::scenenodes::ChangeArea::ChangeArea(
    sad::SceneNode* d,
    const sad::Rect2D& old_value,
    const sad::Rect2D& new_value

)
: history::scenenodes::ChangeProperty<sad::Rect2D>(
      d,
      "area",
      old_value,
      new_value
)
{
    m_affects_parent_grid = true;
}

history::scenenodes::ChangeArea::~ChangeArea()
{

}

void history::scenenodes::ChangeArea::updateUI(core::Editor* e, const sad::Rect2D& value)
{
    QRectF result;
    core::typeconverters::SadRect2DToQRectF::convert(value, result);
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiSceneNodeBlock()->rwSceneNodeRect,
            &gui::rectwidget::RectWidget::setValue,
            result
        )
    );
}
