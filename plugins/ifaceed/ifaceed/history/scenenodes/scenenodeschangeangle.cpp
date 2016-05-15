#include "scenenodeschangeangle.h"

#include "../../core/editor.h"

#include "../../core/typeconverters/sadrect2dtoqrectf.h"


// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiscenenodeblock.h"

#include "../../gui/anglewidget/anglewidget.h"
#include "../../gui/rectwidget/rectwidget.h"

#include "../../blockedclosuremethodcall.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/gridactions.h"

history::scenenodes::ChangeAngle::ChangeAngle(
    sad::SceneNode* d,
    float oldvalue,
    float newvalue
) : history::scenenodes::ChangeProperty<float>(d, "angle", oldvalue, newvalue)
{
    
}

history::scenenodes::ChangeAngle::~ChangeAngle()
{
    
}

void history::scenenodes::ChangeAngle::updateUI(core::Editor* e, const float& value)
{
    gui::anglewidget::AngleWidget* w = e->uiBlocks()->uiSceneNodeBlock()->awSceneNodeAngle;
    e->emitClosure( blocked_bind(
        w, 
        &gui::anglewidget::AngleWidget::setValue, 
        static_cast<double>(value)
    ) );
}

void history::scenenodes::ChangeAngle::tryUpdateUI(core::Editor* e, const float& value)
{
	if (e->isNodeSelected(m_node))
    {
        this->updateUI(e, value);
    }

    gui::actions::GridActions* ga = e->actions()->gridActions();
    sad::layouts::Grid* parent = ga->parentGridFor(m_node);
    if (parent)
    {
        parent->update();
        if (e->isNodeSelected(m_node))
        {
			sad::Maybe<sad::Rect2D> mbarea = m_node->getProperty<sad::Rect2D>("area");
			if (mbarea.exists())
			{
				QRectF result;
				core::typeconverters::SadRect2DToQRectF::convert(mbarea.value(), result);
				e->emitClosure( blocked_bind(
						e->uiBlocks()->uiSceneNodeBlock()->rwSceneNodeRect,
						&gui::rectwidget::RectWidget::setValue,
						result
					)
				);
			}
        }
        if (e->shared()->selectedGrid() == parent)
        {
            ga->updateRegion();
        }
    }
}
