#include "labelactions.h"

#include "../blockedclosuremethodcall.h"

#include "../mainpanel.h"
#include "../qstdstring.h"

#include "../core/editor.h"

#include "../core/typeconverters/qcolortosadacolor.h"

#include "../gui/scenenodeactions.h"

#include "../history/scenenodes/scenenodesnew.h"

#include "../history/label/changefontname.h"
#include "../history/label/changefontsize.h"
#include "../history/label/changetext.h"
#include "../history/label/changelinespacing.h"

#include <label.h>
#include <geometry2d.h>



// ===============================  PUBLIC METHODS ===============================

gui::LabelActions::LabelActions(QObject* parent) : QObject(parent), m_panel(NULL)
{

}

gui::LabelActions::~LabelActions()
{
	
}

void gui::LabelActions::setPanel(MainPanel* e)
{
	m_panel = e;
}

MainPanel* gui::LabelActions::panel() const
{
	return m_panel;
}

void gui::LabelActions::cancelAddLabel()
{
	core::Shared* s = this->m_panel->editor()->shared();
	sad::SceneNode* label = s->activeObject();
	sad::Renderer::ref()->lockRendering();
	s->setActiveObject(NULL);
	label->scene()->remove(label);
	sad::Renderer::ref()->unlockRendering();
	
	sad::hfsm::Machine* m = m_panel->editor()->machine();
	m->enterState(m->previousState());
	if (m->isInState("adding") || (m->isInState("selected") && s->selectedObject() == NULL))
	{
		m->enterState("idle");
	}
	else
	{
		m_panel->updateUIForSelectedItem();
	}
}

void gui::LabelActions::moveLabel(const sad::input::MouseMoveEvent & e)
{
	const sad::Point2D& p = e.pos2D();
	sad::SceneNode* node = m_panel->editor()->shared()->activeObject();
	if (node)
	{
		sad::Maybe<sad::Rect2D> mayberect = node->getProperty<sad::Rect2D>("area");
		if (mayberect.exists())
		{
			sad::Rect2D r = mayberect.value();
			double width = (r[1].x() - r[0].x()) / 2.0;
			double height = (fabs(r[2].y() - r[0].y())) / 2.0;

			r = sad::Rect2D(p.x() - width, p.y() + height, p.x() + width, p.y() - height);
			node->setProperty("area", r);
			this->m_panel->sceneNodeActions()->updateRegionForNode();
		}
	}
}

void gui::LabelActions::commitLabelAdd(const sad::input::MousePressEvent&)
{
	core::Shared* s = m_panel->editor()->shared();
	sad::SceneNode* node = s->activeObject();
	sad::Renderer::ref()->database("")->table("scenenodes")->add(node);
	history::Command* c = new history::scenenodes::New(node);
	s->setActiveObject(NULL);
	s->setSelectedObject(node);
	m_panel->editor()->history()->add(c);
	c->commit(m_panel->editor());
	m_panel->editor()->machine()->enterState("selected");

	m_panel->editor()->emitClosure(blocked_bind(
		m_panel,
		&MainPanel::selectLastSceneNode
	));
}

// ===============================  PUBLIC SLOTS METHODS ===============================

void gui::LabelActions::addLabel()
{
	bool valid = true;
	valid = valid && m_panel->currentScene() != NULL;
	valid = valid && m_panel->UI()->txtLabelText->toPlainText().length() != 0;
	valid = valid && m_panel->UI()->rtwLabelFont->selectedResourceName().exists();
	valid = valid && m_panel->editor()->machine()->isInState("adding") == false;
	if (valid)
	{
		// Cleanup last adding item if was in that state
		bool blocked = m_panel->UI()->cbSceneNodeVisible->blockSignals(true);
		m_panel->UI()->cbSceneNodeVisible->setCheckState(Qt::Checked);
		m_panel->UI()->cbSceneNodeVisible->blockSignals(blocked);

		m_panel->editor()->cleanupBeforeAdding();

		// Create and initialize label
		sad::Label* label = new sad::Label();
		label->setTreeName("");
		label->setFontName(m_panel->UI()->rtwLabelFont->selectedResourceName().value());

		const sad::Settings & settings = sad::Renderer::ref()->settings();
		label->setPoint(settings.width() / 2.0, settings.height() / 2.0);
		label->setString(Q2STDSTRING(m_panel->UI()->txtLabelText->toPlainText()));
		label->setSize(m_panel->UI()->fswLabelFontSize->value());
		label->setLineSpacingRatio(m_panel->UI()->dsbLineSpacingRatio->value());
		label->setAngle(m_panel->UI()->awSceneNodeAngle->value());

		sad::AColor clr;
		core::typeconverters::QColorToSadAColor::convert(m_panel->UI()->clpSceneNodeColor->selectedColor(), clr);
		label->setColor(clr);

		QString name = m_panel->UI()->txtObjectName->text();
		if (name.length())
		{
			label->setObjectName(Q2STDSTRING(name));
		}

		// Actually add label
		m_panel->currentScene()->add(label);
		m_panel->editor()->shared()->setActiveObject(label);
		m_panel->editor()->shared()->toggleActiveBorder(true);

		m_panel->editor()->machine()->enterState("adding/label");
		m_panel->highlightState("Click, where you want label to be placed");

		m_panel->sceneNodeActions()->updateRegionForNode();
	}
}

void gui::LabelActions::labelFontChanged(sad::String s)
{
	if (m_panel->editor()->shared()->activeObject() != NULL)
	{
		sad::Renderer::ref()->lockRendering();
		m_panel->editor()->shared()->activeObject()->setProperty("font", s);
		sad::Renderer::ref()->unlockRendering();
	}
	else
	{
		sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
		if (node)
		{
			sad::Maybe<sad::String> oldvalue = node->getProperty<sad::String>("font");
			if (oldvalue.exists())
			{
				if (oldvalue.value() != s)
				{
					sad::Renderer::ref()->lockRendering();
					node->setProperty("font", s);
					sad::Renderer::ref()->unlockRendering();
					m_panel->editor()->history()->add(new history::label::ChangeFontName(node, oldvalue.value(), s));
				}
			}
		}
	}
}

void gui::LabelActions::labelSizeChanged(unsigned int s)
{
    if (m_panel->editor()->shared()->activeObject() != NULL)
    {
		sad::Renderer::ref()->lockRendering();
        m_panel->editor()->shared()->activeObject()->setProperty("fontsize", s);
        sad::Renderer::ref()->unlockRendering();

		m_panel->sceneNodeActions()->updateRegionForNode();
    }
    else
    {
        sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<unsigned int> oldvalue = node->getProperty<unsigned int>("fontsize");
            if (oldvalue.exists())
            {
                if (oldvalue.value() != s)
                {
					sad::Renderer::ref()->lockRendering();
                    node->setProperty("fontsize", s);
					sad::Renderer::ref()->unlockRendering();
                    m_panel->sceneNodeActions()->updateRegionForNode();
                    m_panel->editor()->history()->add(new history::label::ChangeFontSize(node, oldvalue.value(), s));
                }
            }
        }
    }
}


void gui::LabelActions::labelTextChanged()
{
    sad::String newvalue = Q2STDSTRING(m_panel->UI()->txtLabelText->toPlainText());
    if (m_panel->editor()->shared()->activeObject() != NULL)
    {
        m_panel->editor()->shared()->activeObject()->setProperty("text", newvalue);
        m_panel->sceneNodeActions()->updateRegionForNode();
    }
    else
    {
        sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<sad::String> oldvalue = node->getProperty<sad::String>("text");
            if (oldvalue.exists())
            {
                if (oldvalue.value() != newvalue)
                {
                    node->setProperty("text", newvalue);
                    m_panel->sceneNodeActions()->updateRegionForNode();
                    m_panel->editor()->history()->add(new history::label::ChangeText(node, oldvalue.value(), newvalue));
                }
            }
        }
    }
}

void gui::LabelActions::labelLineSpacingChanged(double newvalue)
{
	if (m_panel->editor()->shared()->activeObject() != NULL)
    {
        m_panel->editor()->shared()->activeObject()->setProperty("linespacing", newvalue);
        m_panel->sceneNodeActions()->updateRegionForNode();
    }
    else
    {
        sad::SceneNode* node = m_panel->editor()->shared()->selectedObject();
        if (node)
        {
            sad::Maybe<float> oldvalue = node->getProperty<float>("linespacing");
            if (oldvalue.exists())
            {
				if (sad::is_fuzzy_equal(oldvalue.value(), newvalue, 0.0001) == false)
                {
                    node->setProperty("linespacing", newvalue);
                    m_panel->sceneNodeActions()->updateRegionForNode();
                    m_panel->editor()->history()->add(new history::label::ChangeLineSpacing(node, oldvalue.value(), newvalue));
                }
            }
        }
    }
}
