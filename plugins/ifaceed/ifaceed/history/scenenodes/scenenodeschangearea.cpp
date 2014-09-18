#include "scenenodeschangearea.h"

#include "../../core/editor.h"

#include "../../core/typeconverters/sadrect2dtoqrectf.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::scenenodes::ChangeArea::ChangeArea(
    sad::SceneNode* d,
    const sad::Rect2D& oldvalue,
    const sad::Rect2D& newvalue

)
: history::scenenodes::PropertyChanged<sad::Rect2D>(
      d,
      "area",
      oldvalue,
      newvalue
)
{
}

history::scenenodes::ChangeArea::~ChangeArea()
{

}

void history::scenenodes::ChangeArea::updateUI(core::Editor* e, const sad::Rect2D& value)
{
    QRectF result;
    core::typeconverters::SadRect2DToQRectF::convert(value, result);
    e->emitClosure( blocked_bind(
            e->panel()->UI()->rwSceneNodeRect,
            &gui::rectwidget::RectWidget::setValue,
            result
        )
    );
}

